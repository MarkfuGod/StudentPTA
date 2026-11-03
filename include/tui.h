#ifndef STUDENTPTA_TUI_H
#define STUDENTPTA_TUI_H

#include <string>
#include <vector>

// Minimal full-screen terminal toolkit: raw keyboard input, colours and
// flicker-free redraws. POSIX only (macOS / Linux), no external libraries.
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

KeyEvent readKey();        // blocks; returns Key::Resize when the window changes size
bool waitKey(int timeoutMs);  // true if a key is ready to read
void sleepMs(int ms);
int width();
int height();

struct Rgb {
    int r, g, b;
};
Rgb mix(Rgb a, Rgb b, double t);
std::string fg(Rgb c);
std::string bg(Rgb c);
extern const char* const kReset;
extern const char* const kBold;

std::string gradient(const std::string& utf8, Rgb from, Rgb to);
int visibleWidth(const std::string& s);  // ignores escape codes, counts UTF-8 code points
std::string padRight(const std::string& s, int width);
// Cut to at most `width` visible cells (ending in "…" when cut), keeping escape codes.
std::string truncate(const std::string& s, int width);
std::string repeat(const std::string& s, int n);
// Horizontal bar with 1/8-cell precision, `width` cells wide.
std::string bar(double fraction, int width, Rgb color, Rgb track);

// Draw a whole screen in one write, centred horizontally.
void present(const std::vector<std::string>& lines);

}  // namespace tui

#endif
