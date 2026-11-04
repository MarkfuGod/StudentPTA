#include "fx.h"

#include <algorithm>
#include <cmath>
#include <random>

using tui::Canvas;
using tui::Rgb;

namespace fx {

namespace {

const double kPi = 3.14159265358979323846;

std::mt19937& rng() {
    static std::mt19937 g(std::random_device{}());
    return g;
}

double rnd(double lo, double hi) { return std::uniform_real_distribution<double>(lo, hi)(rng()); }
int rndInt(int lo, int hi) { return std::uniform_int_distribution<int>(lo, hi)(rng()); }

// ------------------------------------------------------------ digital rain --

struct Drop {
    bool active = false;
    double y = 0, speed = 0, wait = 0;
    int length = 0;
};

const char kRainGlyphs[] = "01<>/\\|=+*#%$&{}[]~^";

// ---------------------------------------------------------------- particles --

struct Particle {
    double x, y, vx, vy, life, maxLife;
    Rgb color;
};

// -------------------------------------------------------------------- robot --

enum class Mood { Normal, Happy, Sad };

struct Robot {
    double x = 4;
    int dir = 1;
    bool moving = true;
    double stateUntil = 3;
    double nextBlink = 2;
    Mood mood = Mood::Normal;
    double moodUntil = 0;
    double jumpStart = -10;
    std::string bubble;
    double bubbleUntil = 0;
    double nextIdle = 9;
};

const char* const kIdleLines[] = {"beep boop", "neon never sleeps", "0 bugs today?", "hydrate, coder",
                                  "scoring at 30 fps", "press 3 for a report", "ping... pong"};

// ------------------------------------------------------------------- state --

struct World {
    double last = -1;
    std::vector<Drop> rain;
    std::vector<Particle> particles;
    Robot robot;
    double glitchUntil = 0, glitchStrength = 0, nextGlitch = 5;
    Transition pending = Transition::None;
    Canvas prevUi;
    int uiX = 0, uiY = 0;
};

World& world() {
    static World w;
    return w;
}

void spawnBurst(double x, double y, int count, double power) {
    static const Rgb colors[] = {kPink, kCyan, kYellow, kPurple, kGreen};
    for (int i = 0; i < count; ++i) {
        double a = rnd(0, 2 * kPi);
        double s = rnd(6, 22) * power;
        double life = rnd(0.5, 1.2);
        world().particles.push_back({x, y, std::cos(a) * s, std::sin(a) * s * 0.5 - rnd(2, 6) * power, life, life,
                                     colors[rndInt(0, 4)]});
    }
}

void update(int W, int H) {
    World& w = world();
    double now = t();
    double dt = w.last < 0 ? 0 : std::min(0.1, now - w.last);
    w.last = now;

    // rain
    if (static_cast<int>(w.rain.size()) != W) w.rain.assign(static_cast<size_t>(W), Drop());
    for (Drop& d : w.rain) {
        if (!d.active) {
            d.wait -= dt;
            if (d.wait <= 0 && rnd(0, 1) < 0.02) {
                d.active = true;
                d.y = -rnd(0, H / 2.0);
                d.speed = rnd(5, 14);
                d.length = rndInt(4, 11);
            }
            continue;
        }
        d.y += d.speed * dt;
        if (d.y - d.length > H) {
            d.active = false;
            d.wait = rnd(0.5, 4);
        }
    }

    // particles
    for (Particle& p : w.particles) {
        p.vy += 14 * dt;
        p.vx *= 1 - 1.2 * dt;
        p.x += p.vx * dt;
        p.y += p.vy * dt;
        p.life -= dt;
    }
    w.particles.erase(std::remove_if(w.particles.begin(), w.particles.end(),
                                     [](const Particle& p) { return p.life <= 0; }),
                      w.particles.end());

    // robot
    Robot& r = w.robot;
    if (r.mood != Mood::Normal && now > r.moodUntil) r.mood = Mood::Normal;
    if (now > r.stateUntil) {
        r.moving = !r.moving;
        r.stateUntil = now + (r.moving ? rnd(2.5, 6) : rnd(0.8, 2.2));
        if (!r.moving && rnd(0, 1) < 0.4) r.dir = -r.dir;
    }
    if (r.moving) r.x += r.dir * 7.0 * dt;
    if (r.x < 1) {
        r.x = 1;
        r.dir = 1;
    }
    if (r.x > W - 7) {
        r.x = W - 7;
        r.dir = -1;
    }
    if (now > r.nextIdle) {
        if (now > r.bubbleUntil) {
            r.bubble = kIdleLines[rndInt(0, sizeof(kIdleLines) / sizeof(kIdleLines[0]) - 1)];
            r.bubbleUntil = now + 2.4;
        }
        r.nextIdle = now + rnd(14, 26);
    }

    // spontaneous small glitches
    if (now > w.nextGlitch) {
        w.glitchUntil = now + rnd(0.08, 0.18);
        w.glitchStrength = 0.35;
        w.nextGlitch = now + rnd(4, 9);
    }
}

void drawBackground(Canvas& c) {
    World& w = world();
    double now = t();
    for (int y = 0; y < c.h; ++y)
        for (int x = 0; x < c.w; ++x) {
            tui::Cell& cell = c.at(x, y);
            cell.ch = " ";
            cell.bg = y % 2 || !tui::trueColor() ? kBase : Rgb{12, 10, 26};  // faint scanlines (24-bit only)
            cell.fg = kInk;
            cell.bold = false;
            cell.set = false;
        }

    for (int x = 0; x < c.w && x < static_cast<int>(w.rain.size()); ++x) {
        const Drop& d = w.rain[static_cast<size_t>(x)];
        if (!d.active) continue;
        int head = static_cast<int>(d.y);
        for (int k = 0; k < d.length; ++k) {
            int y = head - k;
            if (y < 0 || y >= c.h) continue;
            unsigned seed = static_cast<unsigned>(x * 7919 + y * 104729 + static_cast<int>(now * 6) * 31);
            char g = kRainGlyphs[seed % (sizeof(kRainGlyphs) - 1)];
            double fade = 1.0 - static_cast<double>(k) / d.length;
            Rgb col = k == 0 ? tui::mix(c.at(x, y).bg, Rgb{120, 255, 230}, 0.55)
                             : tui::mix(c.at(x, y).bg, Rgb{0, 160, 170}, 0.38 * fade);
            c.put(x, y, std::string(1, g), col);
        }
    }

}

// Sparks are drawn last, above the UI panels.
void drawParticles(Canvas& c) {
    for (const Particle& p : world().particles) {
        int x = static_cast<int>(std::lround(p.x));
        int y = static_cast<int>(std::lround(p.y));
        if (!c.inside(x, y)) continue;
        double f = p.life / p.maxLife;
        const char* glyph = f > 0.75 ? "✦" : f > 0.5 ? "*" : f > 0.25 ? "+" : "·";
        c.put(x, y, glyph, tui::mix(c.at(x, y).bg, p.color, 0.35 + 0.65 * f), f > 0.5);
    }
}

void drawRobot(Canvas& c, int laneTop) {
    Robot& r = world().robot;
    double now = t();
    int x = static_cast<int>(r.x);
    double sinceJump = now - r.jumpStart;
    int lift = sinceJump < 0.55 ? (sinceJump < 0.12 || sinceJump > 0.45 ? 1 : 2) : 0;
    int y = laneTop - lift;

    bool blink = now > r.nextBlink && now < r.nextBlink + 0.14;
    if (now > r.nextBlink + 0.14) r.nextBlink = now + rnd(2, 5);

    std::string eyes;
    Rgb eyeColor = kYellow;
    if (r.mood == Mood::Happy) {
        eyes = "^ ^";
        eyeColor = kPink;
    } else if (r.mood == Mood::Sad) {
        eyes = "x x";
        eyeColor = kRed;
    } else if (blink) {
        eyes = "- -";
    } else {
        eyes = r.dir > 0 ? " oo" : "oo ";
    }

    bool step = r.moving && static_cast<int>(now * 7) % 2 == 0;
    std::string legs = lift ? " \\ / " : step ? " / \\ " : " | | ";
    double glow = 0.75 + 0.25 * pulse(1.5);

    c.put(x + 2, y - 1, "•", tui::scale(kPink, glow), true);  // antenna light
    static const char* const head[] = {"╭", "─", "┴", "─", "╮"};
    for (int i = 0; i < 5; ++i) c.put(x + i, y, head[i], kCyan);
    c.put(x, y + 1, "│", kCyan);
    for (int i = 0; i < 3; ++i) c.put(x + 1 + i, y + 1, std::string(1, eyes[static_cast<size_t>(i)]), eyeColor, true);
    c.put(x + 4, y + 1, "│", kCyan);
    for (int i = 0; i < 5; ++i) c.put(x + i, y + 2, std::string(1, legs[static_cast<size_t>(i)]), kPurple);

    if (now < r.bubbleUntil && !r.bubble.empty()) {
        std::string text = " " + r.bubble + " ";
        int len = tui::visibleWidth(text);
        int bx = r.dir > 0 ? x + 4 : x + 1 - len;
        bx = std::max(0, std::min(c.w - len, bx));
        int by = y - 2;
        if (by >= 0) {
            c.drawStyled(bx, by, tui::kBold + tui::fg(kBase) + tui::bg(kYellow) + text, kBase, kYellow);
        }
    }
}

// Project a UI layer as a card rotated by `angle` (0 = facing us) around the
// vertical axis through the centre of its bounding box.
void projectLayer(Canvas& out, const Canvas& layer, int x0, int y0, int bw, int bh, double angle, int side) {
    double c = std::cos(angle);
    double s = std::sin(angle);
    double cx = x0 + bw / 2.0;
    double cy = y0 + bh / 2.0;
    double halfW = bw / 2.0;
    double shade = 0.35 + 0.65 * std::fabs(c);
    for (int y = y0 - 2; y < y0 + bh + 2; ++y) {
        for (int x = x0; x < x0 + bw; ++x) {
            if (!out.inside(x, y)) continue;
            double u = (x + 0.5 - cx) / (halfW * std::max(0.02, std::fabs(c)));
            if (u < -1 || u > 1) continue;
            double k = 1 + 0.28 * s * u * side;  // near edge taller, far edge shorter
            double sy = cy + (y + 0.5 - cy) / k;
            int lx = static_cast<int>(cx + u * halfW);
            int ly = static_cast<int>(std::floor(sy));
            if (!layer.inside(lx, ly) || ly < y0 || ly >= y0 + bh) continue;
            const tui::Cell& src = layer.at(lx, ly);
            if (!src.set) continue;
            tui::Cell& dst = out.at(x, y);
            dst.ch = std::fabs(c) < 0.2 && src.ch != " " ? "▌" : src.ch;
            dst.fg = tui::scale(src.fg, shade);
            dst.bg = tui::scale(src.bg, shade);
            dst.bold = src.bold;
        }
    }
}

void overlay(Canvas& out, const Canvas& layer) {
    for (size_t i = 0; i < out.cells.size(); ++i)
        if (layer.cells[i].set) out.cells[i] = layer.cells[i];
}

void applyGlitch(Canvas& c, int x0, int y0, int bw, int bh) {
    World& w = world();
    if (t() > w.glitchUntil) return;
    int rows = 1 + static_cast<int>(w.glitchStrength * 4);
    for (int i = 0; i < rows; ++i) {
        int y = y0 + rndInt(0, std::max(0, bh - 1));
        if (y < 0 || y >= c.h) continue;
        int shift = rndInt(1, 2 + static_cast<int>(w.glitchStrength * 4)) * (rndInt(0, 1) ? 1 : -1);
        std::vector<tui::Cell> row(c.cells.begin() + y * c.w, c.cells.begin() + (y + 1) * c.w);
        Rgb tint = rndInt(0, 1) ? kPink : kCyan;
        for (int x = std::max(0, x0 - 4); x < std::min(c.w, x0 + bw + 4); ++x) {
            int sx = x - shift;
            if (sx < 0 || sx >= c.w) continue;
            tui::Cell cell = row[static_cast<size_t>(sx)];
            cell.fg = tui::mix(cell.fg, tint, 0.6);
            if (rnd(0, 1) < 0.06 * w.glitchStrength) {
                static const char* const blocks[] = {"▓", "▒", "░", "█"};
                cell.ch = blocks[rndInt(0, 3)];
            }
            c.at(x, y) = cell;
        }
    }
}

}  // namespace

double t() { return tui::now(); }

double pulse(double hz) {
    // Quantised to 12 levels: colours only change when the level does, which
    // keeps the terminal output small without visible stepping.
    double v = 0.5 + 0.5 * std::sin(t() * hz * 2 * kPi);
    return std::round(v * 12) / 12;
}

void transition(Transition tr) { world().pending = tr; }

void glitch(double seconds, double strength) {
    world().glitchUntil = t() + seconds;
    world().glitchStrength = strength;
}

void burstAt(int uiRow, int uiCol, int count, double power) {
    spawnBurst(world().uiX + uiCol, world().uiY + uiRow, count, power);
}

void say(const std::string& bubble, double seconds) {
    Robot& r = world().robot;
    r.bubble = bubble;
    r.bubbleUntil = t() + seconds;
}

void celebrate(const std::string& bubble) {
    Robot& r = world().robot;
    r.mood = Mood::Happy;
    r.moodUntil = t() + 1.8;
    r.jumpStart = t();
    say(bubble);
    spawnBurst(r.x + 2, tui::height() - 4, 14, 0.7);
}

void oops(const std::string& bubble) {
    Robot& r = world().robot;
    r.mood = Mood::Sad;
    r.moodUntil = t() + 1.6;
    say(bubble, 2.0);
    glitch(0.25, 1.0);
}

void show(const std::vector<std::string>& ui) {
    World& w = world();
    int W = tui::width();
    int H = tui::height();
    update(W, H);

    int widest = 0;
    for (const std::string& l : ui) widest = std::max(widest, tui::visibleWidth(l));
    int rows = static_cast<int>(ui.size());

    Canvas frame(W, H);
    if (widest > W || rows > H) {
        drawBackground(frame);
        std::string msg = "Window too small: need " + std::to_string(widest) + "x" + std::to_string(rows + 4) +
                          ", have " + std::to_string(W) + "x" + std::to_string(H);
        frame.drawStyled(0, 0, tui::kBold + tui::fg(kYellow) + tui::truncate(msg, W), kInk, kBase);
        tui::flush(frame);
        return;
    }

    const int lane = 4;  // robot (3 rows) + antenna
    bool robotFits = rows + lane <= H;
    int usable = robotFits ? H - lane : H;
    w.uiX = (W - widest) / 2;
    w.uiY = std::max(0, (usable - rows) / 3);

    Canvas layer(W, H);
    for (int i = 0; i < rows; ++i) {
        const std::string& line = ui[static_cast<size_t>(i)];
        bool boxed = false;
        for (const char* b : {"╭", "│", "├", "╰"})
            if (line.find(b) != std::string::npos && tui::visibleWidth(line) >= widest - 1) boxed = true;
        layer.drawStyled(w.uiX, w.uiY + i, line, kInk, boxed ? kPanel : ((w.uiY + i) % 2 || !tui::trueColor() ? kBase : Rgb{12, 10, 26}));
    }

    if (w.pending != Transition::None && w.prevUi.w == W && w.prevUi.h == H) {
        int side = w.pending == Transition::FlipForward ? 1 : -1;
        const int frames = 14;
        for (int f = 1; f < frames; ++f) {
            double a = kPi * f / frames;
            update(W, H);
            drawBackground(frame);
            if (robotFits) drawRobot(frame, H - 3);
            if (a < kPi / 2)
                projectLayer(frame, w.prevUi, 0, 0, W, H, a * side, side);
            else
                projectLayer(frame, layer, 0, 0, W, H, (a - kPi) * side, side);
            drawParticles(frame);
            tui::flush(frame);
            tui::sleepMs(18);
        }
    }
    w.pending = Transition::None;

    drawBackground(frame);
    if (robotFits) drawRobot(frame, H - 3);
    overlay(frame, layer);
    drawParticles(frame);
    applyGlitch(frame, w.uiX, w.uiY, widest, rows);
    tui::flush(frame);
    w.prevUi = layer;
}

}  // namespace fx
