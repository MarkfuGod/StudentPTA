#include "tui.h"

#include <poll.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

#include <algorithm>
#include <cctype>
#include <cerrno>
#include <chrono>
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <thread>
#include <unordered_map>

namespace tui {

const char* const kReset = "\033[0m";
const char* const kBold = "\033[1m";
const char* const kNormal = "\033[22m";

namespace {

termios gSaved;
bool gActive = false;
bool gTrueColor = false;
Canvas gPrevious;  // what the terminal currently shows

void writeAll(const char* p, size_t n) {
    while (n > 0) {
        ssize_t w = ::write(STDOUT_FILENO, p, n);
        if (w < 0) {
            if (errno == EINTR) continue;
            return;
        }
        p += w;
        n -= static_cast<size_t>(w);
    }
}

void writeAll(const std::string& s) { writeAll(s.data(), s.size()); }

const char kLeaveScreen[] = "\033[0m\033[?25h\033[?1049l";

void restoreTerminal() {
    if (!gActive) return;
    writeAll(kLeaveScreen, sizeof(kLeaveScreen) - 1);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &gSaved);
    gActive = false;
}

void onFatalSignal(int sig) {
    restoreTerminal();
    std::signal(sig, SIG_DFL);
    std::raise(sig);
}

void onResize(int) {}  // only here so read() is interrupted and the screen redraws

// Nearest xterm-256 colour (cube or grey ramp), cached.
int nearest256(Rgb c) {
    static std::unordered_map<int, int> cache;
    int key = (c.r << 16) | (c.g << 8) | c.b;
    auto it = cache.find(key);
    if (it != cache.end()) return it->second;
    static const int levels[] = {0, 95, 135, 175, 215, 255};
    int best = 16;
    long bestDist = -1;
    for (int i = 16; i < 256; ++i) {
        int r, g, b;
        if (i < 232) {
            r = levels[(i - 16) / 36];
            g = levels[(i - 16) / 6 % 6];
            b = levels[(i - 16) % 6];
        } else {
            r = g = b = 8 + 10 * (i - 232);
        }
        long d = (long)(r - c.r) * (r - c.r) * 3 + (long)(g - c.g) * (g - c.g) * 4 + (long)(b - c.b) * (b - c.b) * 2;
        if (bestDist < 0 || d < bestDist) {
            bestDist = d;
            best = i;
        }
    }
    cache[key] = best;
    return best;
}

std::string terminalColor(Rgb c, bool background) {
    char buf[40];
    if (gTrueColor)
        std::snprintf(buf, sizeof(buf), "\033[%d;2;%d;%d;%dm", background ? 48 : 38, c.r, c.g, c.b);
    else
        std::snprintf(buf, sizeof(buf), "\033[%d;5;%dm", background ? 48 : 38, nearest256(c));
    return buf;
}

size_t utf8Length(unsigned char b) { return b < 0x80 ? 1 : (b >> 5) == 0x6 ? 2 : (b >> 4) == 0xE ? 3 : 4; }

std::vector<std::string> codePoints(const std::string& s) {
    std::vector<std::string> out;
    for (size_t i = 0; i < s.size();) {
        size_t len = utf8Length(static_cast<unsigned char>(s[i]));
        out.push_back(s.substr(i, len));
        i += len;
    }
    return out;
}

Rgb from256(int n) {
    static const int levels[] = {0, 95, 135, 175, 215, 255};
    if (n >= 232) {
        int v = 8 + 10 * (n - 232);
        return {v, v, v};
    }
    if (n >= 16) return {levels[(n - 16) / 36], levels[(n - 16) / 6 % 6], levels[(n - 16) % 6]};
    return {192, 192, 192};
}

bool sameCell(const Cell& a, const Cell& b) {
    return a.ch == b.ch && a.fg == b.fg && a.bg == b.bg && a.bold == b.bold;
}

}  // namespace

bool available() {
    const char* term = std::getenv("TERM");
    return isatty(STDIN_FILENO) && isatty(STDOUT_FILENO) && !std::getenv("STUDENTPTA_PLAIN") &&
           !(term && std::strcmp(term, "dumb") == 0);
}

void begin() {
    if (gActive) return;
    tcgetattr(STDIN_FILENO, &gSaved);
    termios raw = gSaved;
    raw.c_lflag &= ~static_cast<tcflag_t>(ICANON | ECHO);
    raw.c_cc[VMIN] = 1;
    raw.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
    gActive = true;

    const char* ct = std::getenv("COLORTERM");
    gTrueColor = ct && (std::strcmp(ct, "truecolor") == 0 || std::strcmp(ct, "24bit") == 0);

    std::signal(SIGINT, onFatalSignal);
    std::signal(SIGTERM, onFatalSignal);
    std::signal(SIGHUP, onFatalSignal);
    struct sigaction sa;
    std::memset(&sa, 0, sizeof(sa));
    sa.sa_handler = onResize;  // no SA_RESTART: we want read() to return EINTR
    sigaction(SIGWINCH, &sa, nullptr);
    std::atexit(restoreTerminal);

    writeAll("\033[?1049h\033[?25l\033[2J");  // alternate screen, hide cursor
    invalidate();
}

void end() { restoreTerminal(); }

bool trueColor() { return gTrueColor; }

bool waitKey(int timeoutMs) {
    pollfd p = {STDIN_FILENO, POLLIN, 0};
    return ::poll(&p, 1, timeoutMs) > 0;
}

KeyEvent readKey(int timeoutMs) {
    if (timeoutMs >= 0) {
        pollfd p = {STDIN_FILENO, POLLIN, 0};
        int r = ::poll(&p, 1, timeoutMs);
        if (r < 0 && errno == EINTR) return {Key::Resize, 0};
        if (r <= 0) return {Key::None, 0};
    }
    unsigned char c;
    ssize_t n = ::read(STDIN_FILENO, &c, 1);
    if (n < 0 && errno == EINTR) return {Key::Resize, 0};
    if (n <= 0) {  // input closed
        end();
        std::exit(0);
    }
    if (c == 27) {
        unsigned char s;
        if (!waitKey(30) || ::read(STDIN_FILENO, &s, 1) != 1) return {Key::Esc, 0};
        if (s != '[' && s != 'O') return {Key::Esc, 0};
        unsigned char code;
        if (!waitKey(30) || ::read(STDIN_FILENO, &code, 1) != 1) return {Key::Esc, 0};
        switch (code) {
            case 'A': return {Key::Up, 0};
            case 'B': return {Key::Down, 0};
            case 'C': return {Key::Right, 0};
            case 'D': return {Key::Left, 0};
            default: break;
        }
        if (std::isdigit(code)) {  // e.g. ESC[3~ (delete): swallow the rest
            unsigned char t = 0;
            while (t != '~' && waitKey(30) && ::read(STDIN_FILENO, &t, 1) == 1) {
            }
        }
        return {Key::None, 0};
    }
    if (c == '\r' || c == '\n') return {Key::Enter, 0};
    if (c == 127 || c == 8) return {Key::Backspace, 0};
    if (c == '\t') return {Key::Tab, 0};
    return {Key::Char, static_cast<char>(c)};
}

void sleepMs(int ms) { std::this_thread::sleep_for(std::chrono::milliseconds(ms)); }

double now() {
    static const auto start = std::chrono::steady_clock::now();
    return std::chrono::duration<double>(std::chrono::steady_clock::now() - start).count();
}

int width() {
    winsize ws;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == 0 && ws.ws_col > 0) return ws.ws_col;
    return 80;
}

int height() {
    winsize ws;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == 0 && ws.ws_row > 0) return ws.ws_row;
    return 24;
}

bool operator==(Rgb a, Rgb b) { return a.r == b.r && a.g == b.g && a.b == b.b; }

Rgb mix(Rgb a, Rgb b, double t) {
    t = std::max(0.0, std::min(1.0, t));
    return {static_cast<int>(a.r + (b.r - a.r) * t), static_cast<int>(a.g + (b.g - a.g) * t),
            static_cast<int>(a.b + (b.b - a.b) * t)};
}

Rgb scale(Rgb c, double k) {
    k = std::max(0.0, k);
    return {std::min(255, static_cast<int>(c.r * k)), std::min(255, static_cast<int>(c.g * k)),
            std::min(255, static_cast<int>(c.b * k))};
}

std::string fg(Rgb c) {
    char buf[32];
    std::snprintf(buf, sizeof(buf), "\033[38;2;%d;%d;%dm", c.r, c.g, c.b);
    return buf;
}

std::string bg(Rgb c) {
    char buf[32];
    std::snprintf(buf, sizeof(buf), "\033[48;2;%d;%d;%dm", c.r, c.g, c.b);
    return buf;
}

std::string gradient(const std::string& utf8, Rgb from, Rgb to) {
    std::vector<std::string> cps = codePoints(utf8);
    std::string out;
    size_t n = cps.size();
    for (size_t i = 0; i < n; ++i) {
        if (cps[i] == " ") {
            out += ' ';
            continue;
        }
        out += fg(mix(from, to, n > 1 ? static_cast<double>(i) / (n - 1) : 0.0));
        out += cps[i];
    }
    return out + kReset;
}

int visibleWidth(const std::string& s) {
    int w = 0;
    for (size_t i = 0; i < s.size(); ++i) {
        unsigned char b = static_cast<unsigned char>(s[i]);
        if (b == 27) {  // skip CSI sequence up to its final byte
            ++i;
            if (i < s.size() && s[i] == '[') {
                ++i;
                while (i < s.size() && !(s[i] >= 0x40 && s[i] <= 0x7E)) ++i;
            }
            continue;
        }
        if ((b & 0xC0) != 0x80) ++w;
    }
    return w;
}

std::string padRight(const std::string& s, int width) {
    int pad = width - visibleWidth(s);
    return pad > 0 ? s + std::string(pad, ' ') : s;
}

std::string truncate(const std::string& s, int width) {
    if (visibleWidth(s) <= width) return s;
    std::string out;
    int w = 0;
    for (size_t i = 0; i < s.size();) {
        unsigned char b = static_cast<unsigned char>(s[i]);
        if (b == 27) {  // copy escape sequences untouched
            size_t j = i + 1;
            if (j < s.size() && s[j] == '[') {
                ++j;
                while (j < s.size() && !(s[j] >= 0x40 && s[j] <= 0x7E)) ++j;
            }
            out += s.substr(i, j + 1 - i);
            i = j + 1;
            continue;
        }
        size_t len = utf8Length(b);
        if (w == width - 1) break;
        out += s.substr(i, len);
        ++w;
        i += len;
    }
    return out + "…" + kReset;
}

std::string repeat(const std::string& s, int n) {
    std::string out;
    for (int i = 0; i < n; ++i) out += s;
    return out;
}

std::string bar(double fraction, int width, Rgb color, Rgb track) {
    static const char* const kEighths[] = {"", "▏", "▎", "▍", "▌", "▋", "▊", "▉"};
    fraction = std::max(0.0, std::min(1.0, fraction));
    int eighths = static_cast<int>(fraction * width * 8 + 0.5);
    int full = eighths / 8;
    int part = eighths % 8;
    std::string out = fg(color) + repeat("█", full) + kEighths[part];
    int used = full + (part ? 1 : 0);
    out += fg(track) + repeat("─", width - used) + kReset;
    return out;
}

void Canvas::put(int x, int y, const std::string& ch, Rgb color, bool bold) {
    if (!inside(x, y)) return;
    Cell& c = at(x, y);
    c.ch = ch;
    c.fg = color;
    c.bold = bold;
}

void Canvas::drawStyled(int x, int y, const std::string& s, Rgb defaultFg, Rgb defaultBg) {
    Rgb curFg = defaultFg;
    Rgb curBg = defaultBg;
    bool bold = false;
    for (size_t i = 0; i < s.size();) {
        unsigned char b = static_cast<unsigned char>(s[i]);
        if (b == 27) {
            size_t j = i + 1;
            if (j < s.size() && s[j] == '[') {
                ++j;
                size_t start = j;
                while (j < s.size() && !(s[j] >= 0x40 && s[j] <= 0x7E)) ++j;
                if (j < s.size() && s[j] == 'm') {
                    std::vector<int> p;
                    std::string params = s.substr(start, j - start);
                    size_t k = 0;
                    while (k <= params.size()) {
                        size_t e = params.find(';', k);
                        if (e == std::string::npos) e = params.size();
                        p.push_back(e > k ? std::atoi(params.substr(k, e - k).c_str()) : 0);
                        k = e + 1;
                    }
                    for (size_t q = 0; q < p.size(); ++q) {
                        int v = p[q];
                        if (v == 0) {
                            curFg = defaultFg;
                            curBg = defaultBg;
                            bold = false;
                        } else if (v == 1) {
                            bold = true;
                        } else if (v == 22) {
                            bold = false;
                        } else if (v == 39) {
                            curFg = defaultFg;
                        } else if (v == 49) {
                            curBg = defaultBg;
                        } else if ((v == 38 || v == 48) && q + 1 < p.size()) {
                            Rgb c = {0, 0, 0};
                            if (p[q + 1] == 2 && q + 4 < p.size()) {
                                c = {p[q + 2], p[q + 3], p[q + 4]};
                                q += 4;
                            } else if (p[q + 1] == 5 && q + 2 < p.size()) {
                                c = from256(p[q + 2]);
                                q += 2;
                            }
                            (v == 38 ? curFg : curBg) = c;
                        }
                    }
                }
            }
            i = j + 1;
            continue;
        }
        size_t len = utf8Length(b);
        if (inside(x, y)) {
            Cell& c = at(x, y);
            c.ch = s.substr(i, len);
            c.fg = curFg;
            c.bg = curBg;
            c.bold = bold;
            c.set = true;
        }
        ++x;
        i += len;
    }
}

void invalidate() { gPrevious = Canvas(); }

void flush(const Canvas& canvas) {
    bool full = gPrevious.w != canvas.w || gPrevious.h != canvas.h;
    std::string out;
    if (full) out += "\033[0m\033[2J";

    for (int y = 0; y < canvas.h; ++y) {
        int x = 0;
        while (x < canvas.w) {
            if (!full && sameCell(canvas.at(x, y), gPrevious.at(x, y))) {
                ++x;
                continue;
            }
            // Start of a changed run: position the cursor and send the full style.
            char pos[24];
            std::snprintf(pos, sizeof(pos), "\033[%d;%dH", y + 1, x + 1);
            out += pos;
            const Cell* last = nullptr;
            int unchanged = 0;
            while (x < canvas.w) {
                const Cell& c = canvas.at(x, y);
                bool same = !full && sameCell(c, gPrevious.at(x, y));
                if (same && ++unchanged > 4) break;  // long unchanged gap: end the run
                if (!same) unchanged = 0;
                if (!last || last->bold != c.bold) out += c.bold ? kBold : kNormal;
                if (!last || last->fg != c.fg) out += terminalColor(c.fg, false);
                if (!last || last->bg != c.bg) out += terminalColor(c.bg, true);
                out += c.ch;
                last = &c;
                ++x;
            }
        }
    }
    out += "\033[0m";
    writeAll(out);
    gPrevious = canvas;
}

}  // namespace tui
