#ifndef STUDENTPTA_TUI_H
#define STUDENTPTA_TUI_H

#include <string>
#include <vector>

// Minimal full-screen terminal toolkit: raw keyboard input, colours, a cell
// canvas and diffed redraws. POSIX only (macOS / Linux), no external libraries.
namespace tui {

enum class Key { None, Resize, Up, Down, Left, Right, Enter, Esc, Backspace, Tab, Char };

struct KeyEvent {
    Key key;
    char ch;  // set when key == Key::Char
};

// True when stdin and stdout are both terminals (and STUDENTPTA_PLAIN is unset).
bool available();

// Enter / leave full-screen mode. end() also runs on exit and on Ctrl-C.
void begin();
void end();

// Waits up to timeoutMs (forever if negative). Key::None on timeout,
// Key::Resize when the window changed size.
KeyEvent readKey(int timeoutMs = -1);
bool waitKey(int timeoutMs);  // true if a key is ready to read
void sleepMs(int ms);
double now();  // seconds, monotonic
bool trueColor();  // 24-bit colour available (otherwise 256 colours)
int width();
int height();

struct Rgb {
    int r, g, b;
};
bool operator==(Rgb a, Rgb b);
inline bool operator!=(Rgb a, Rgb b) { return !(a == b); }
Rgb mix(Rgb a, Rgb b, double t);
Rgb scale(Rgb c, double k);

// Styling for strings that are later drawn onto a Canvas.
std::string fg(Rgb c);
std::string bg(Rgb c);
extern const char* const kReset;
extern const char* const kBold;
extern const char* const kNormal;  // turns bold off

std::string gradient(const std::string& utf8, Rgb from, Rgb to);
int visibleWidth(const std::string& s);  // ignores escape codes, counts UTF-8 code points
std::string padRight(const std::string& s, int width);
// Cut to at most `width` visible cells (ending in "…" when cut), keeping escape codes.
std::string truncate(const std::string& s, int width);
std::string repeat(const std::string& s, int n);
// Horizontal bar with 1/8-cell precision, `width` cells wide.
std::string bar(double fraction, int width, Rgb color, Rgb track);

struct Cell {
    std::string ch = " ";
    Rgb fg = {200, 200, 210};
    Rgb bg = {0, 0, 0};
    bool bold = false;
    bool set = false;  // written by drawStyled (used for layer transparency)
};

class Canvas {
public:
    Canvas() : w(0), h(0) {}
    Canvas(int width, int height) : w(width), h(height), cells(static_cast<size_t>(width * height)) {}

    bool inside(int x, int y) const { return x >= 0 && y >= 0 && x < w && y < h; }
    Cell& at(int x, int y) { return cells[static_cast<size_t>(y * w + x)]; }
    const Cell& at(int x, int y) const { return cells[static_cast<size_t>(y * w + x)]; }

    // Put one glyph, keeping the cell's background.
    void put(int x, int y, const std::string& ch, Rgb color, bool bold = false);
    // Draw a string that may contain fg()/bg()/kBold codes. Cells without an
    // explicit background get `defaultBg`; every covered cell becomes `set`.
    void drawStyled(int x, int y, const std::string& s, Rgb defaultFg, Rgb defaultBg);

    int w, h;
    std::vector<Cell> cells;
};

// Write the canvas to the terminal, sending only cells that changed since
// the previous flush.
void flush(const Canvas& canvas);
void invalidate();  // force the next flush to redraw everything

}  // namespace tui

#endif
