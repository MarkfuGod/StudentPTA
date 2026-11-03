#include "tui.h"

#include <poll.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

#include <algorithm>
#include <cctype>
#include <cstdio>
#include <cerrno>
#include <chrono>
#include <csignal>
#include <cstdlib>
#include <cstring>
#include <thread>

namespace tui {

const char* const kReset = "\033[0m";
const char* const kBold = "\033[1m";

namespace {

termios gSaved;
bool gActive = false;
bool gTrueColor = false;

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

int cubeLevel(int v) { return v < 48 ? 0 : v < 115 ? 1 : std::min(5, (v - 35) / 40); }

std::string colorCode(Rgb c, bool background) {
    char buf[32];
    if (gTrueColor) {
        std::snprintf(buf, sizeof(buf), "\033[%d;2;%d;%d;%dm", background ? 48 : 38, c.r, c.g, c.b);
    } else {
        int n;
        if (std::abs(c.r - c.g) < 12 && std::abs(c.g - c.b) < 12) {  // grey ramp looks better
            int v = (c.r + c.g + c.b) / 3;
            n = v < 8 ? 16 : v > 238 ? 231 : 232 + (v - 8) / 10;
        } else {
            n = 16 + 36 * cubeLevel(c.r) + 6 * cubeLevel(c.g) + cubeLevel(c.b);
        }
        std::snprintf(buf, sizeof(buf), "\033[%d;5;%dm", background ? 48 : 38, n);
    }
    return buf;
}

// Split a UTF-8 string into code points (as byte strings).
std::vector<std::string> codePoints(const std::string& s) {
    std::vector<std::string> out;
    for (size_t i = 0; i < s.size();) {
        unsigned char b = static_cast<unsigned char>(s[i]);
        size_t len = b < 0x80 ? 1 : (b >> 5) == 0x6 ? 2 : (b >> 4) == 0xE ? 3 : 4;
        out.push_back(s.substr(i, len));
        i += len;
    }
    return out;
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
}

void end() { restoreTerminal(); }

bool waitKey(int timeoutMs) {
    pollfd p = {STDIN_FILENO, POLLIN, 0};
    return ::poll(&p, 1, timeoutMs) > 0;
}

KeyEvent readKey() {
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

Rgb mix(Rgb a, Rgb b, double t) {
    t = std::max(0.0, std::min(1.0, t));
    return {static_cast<int>(a.r + (b.r - a.r) * t), static_cast<int>(a.g + (b.g - a.g) * t),
            static_cast<int>(a.b + (b.b - a.b) * t)};
}

std::string fg(Rgb c) { return colorCode(c, false); }
std::string bg(Rgb c) { return colorCode(c, true); }

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
        size_t len = b < 0x80 ? 1 : (b >> 5) == 0x6 ? 2 : (b >> 4) == 0xE ? 3 : 4;
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

void present(const std::vector<std::string>& lines) {
    int w = width();
    int h = height();
    int widest = 0;
    for (const std::string& l : lines) widest = std::max(widest, visibleWidth(l));
    if (widest > w) {  // would wrap and garble the layout
        std::string msg = "Window too small: please widen it to " + std::to_string(widest) + " columns (now " +
                          std::to_string(w) + ")";
        writeAll("\033[H\033[2J\033[1m" + truncate(msg, w) + kReset);
        return;
    }
    std::string margin(std::max(0, (w - widest) / 2), ' ');

    std::string frame = "\033[H";
    int rows = std::min(static_cast<int>(lines.size()), h);
    for (int i = 0; i < rows; ++i) {
        frame += margin + lines[i] + kReset + "\033[K";
        if (i + 1 < rows) frame += "\r\n";
    }
    frame += "\033[J";
    writeAll(frame);
}

}  // namespace tui
