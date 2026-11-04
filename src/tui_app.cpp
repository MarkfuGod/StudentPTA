#include "tui_app.h"

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "banner.h"
#include "fx.h"
#include "menu.h"
#include "tui.h"

using tui::Key;
using tui::KeyEvent;
using tui::Rgb;

namespace {

// ---------------------------------------------------------------- palette --

const Rgb kCyan = fx::kCyan;
const Rgb kViolet = fx::kPurple;
const Rgb kPink = fx::kPink;
const Rgb kGreen = fx::kGreen;
const Rgb kYellow = fx::kYellow;
const Rgb kRed = fx::kRed;
const Rgb kText = fx::kInk;
const Rgb kMuted = fx::kMuted;
const Rgb kFaint = fx::kFaint;

const int kW = 66;         // width of every box
const int kFrameMs = 33;   // ~30 fps

std::string paint(Rgb c, const std::string& s) { return tui::fg(c) + s + tui::kReset; }
std::string strong(Rgb c, const std::string& s) { return tui::kBold + tui::fg(c) + s + tui::kReset; }

std::string fmt(const char* format, double v) {
    char buf[32];
    std::snprintf(buf, sizeof(buf), format, v);
    return buf;
}

// red -> yellow -> green as the fraction goes 0 -> 1
Rgb scoreColor(double f) {
    return f < 0.5 ? tui::mix(kRed, kYellow, f * 2) : tui::mix(kYellow, kGreen, (f - 0.5) * 2);
}

Rgb neonBorder() { return tui::mix(kCyan, kPink, fx::pulse(0.35)); }
bool caretOn() { return std::fmod(fx::t(), 1.0) < 0.6; }
std::string caret() { return caretOn() ? paint(kCyan, "▌") : " "; }

// ------------------------------------------------------------------ boxes --

std::string boxTop(const std::string& title, Rgb c) {
    std::string t = title.empty() ? "" : " " + title + " ";
    int dashes = kW - 5 - tui::visibleWidth(t);
    return paint(c, "╭─") + paint(kPink, "▸") + strong(kText, t) + paint(c, tui::repeat("─", dashes) + "◆╮");
}

std::string boxRow(const std::string& content, Rgb c) {
    return paint(c, "│") + " " + tui::padRight(tui::truncate(content, kW - 4), kW - 4) + " " + paint(c, "│");
}

std::string boxRule(Rgb c) { return paint(c, "├" + tui::repeat("─", kW - 2) + "┤"); }
std::string boxBottom(Rgb c) {
    return paint(c, "╰" + tui::repeat("─", kW - 8)) + paint(kFaint, "/// ") + paint(c, "──╯");
}

// Row with a pulsing highlight bar; content must not contain kReset.
std::string selectedRow(const std::string& content, Rgb c) {
    Rgb glow = tui::mix(fx::kSelect, Rgb{120, 30, 150}, fx::pulse(1.1));
    return paint(c, "│") + tui::bg(glow) + " " + tui::padRight(tui::truncate(content, kW - 4), kW - 4) + " " +
           tui::kReset + paint(c, "│");
}

std::string footer(const std::string& hints) { return paint(kMuted, "  " + hints); }

// ------------------------------------------------------------------- logo --

std::vector<std::string> logo() {
    struct Glyph {
        char ch;
        const char* top;
        const char* bottom;
    };
    static const Glyph glyphs[] = {
        {'S', "█▀▀", "▄▄█"}, {'T', "▀█▀", " █ "}, {'U', "█ █", "█▄█"}, {'D', "█▀▄", "█▄▀"},
        {'E', "█▀▀", "██▄"}, {'N', "█▄ █", "█ ▀█"}, {'P', "█▀█", "█▀▀"}, {'A', "▄▀█", "█▀█"},
        {' ', " ", " "},
    };
    std::string top, bottom;
    for (char ch : std::string("STUDENT PTA")) {
        for (const Glyph& g : glyphs) {
            if (g.ch != ch) continue;
            if (!top.empty()) {
                top += " ";
                bottom += " ";
            }
            top += g.top;
            bottom += g.bottom;
        }
    }
    // The gradient drifts slowly so the logo shimmers.
    double s = fx::pulse(0.25);
    Rgb a = tui::mix(kPink, kCyan, s), b = tui::mix(kCyan, kPink, s);
    return {tui::gradient(top, a, b), tui::gradient(bottom, a, b)};
}

std::vector<std::string> splitLines(const std::string& s) {
    std::vector<std::string> out;
    std::istringstream in(s);
    std::string line;
    while (std::getline(in, line)) out.push_back(line);
    return out;
}

// ------------------------------------------------------------------- data --

struct Stats {
    int records;
    std::vector<long long> students;  // in first-seen order
};

Stats stats(const RecordList& list) {
    Stats s = {0, {}};
    for (const RecordNode* p = list.head; p; p = p->next) {
        ++s.records;
        if (std::find(s.students.begin(), s.students.end(), p->data.studentNumber) == s.students.end())
            s.students.push_back(p->data.studentNumber);
    }
    return s;
}

std::string plural(size_t n, const char* word) { return std::to_string(n) + " " + word + (n == 1 ? "" : "s"); }

std::string statusLine(const RecordList& list) {
    Stats s = stats(list);
    Rgb dot = caretOn() ? kGreen : tui::scale(kGreen, 0.5);
    return paint(dot, "●") + paint(kMuted, " " + plural(s.records, "record") + " · " + plural(s.students.size(), "student"));
}

std::string titleBar(const std::string& screen, const RecordList& list) {
    std::string left = tui::gradient("STUDENT//PTA", kCyan, kPink) + paint(kMuted, " ⟩ ") + strong(kText, screen);
    std::string right = statusLine(list);
    int gap = kW - tui::visibleWidth(left) - tui::visibleWidth(right);
    return left + std::string(std::max(1, gap), ' ') + right;
}

const char* const kTimeRange[] = {"<= 500s", "<= 1000s", "<= 3000s", "> 3000s"};
const char* const kWrongRange[] = {"0 times", "1-3 times", "> 3 times"};
const char* const kDupRange[] = {"<= 20%", "<= 35%", "<= 50%"};

double problemMax(char problem) { return 100.0 * kProblemWeight[problem - 'A']; }

bool isBack(const KeyEvent& k) { return k.key == Key::Esc || (k.key == Key::Char && k.ch == 'q'); }

// ------------------------------------------------------------------ intro --

void zzuIntro() {
    std::vector<std::string> art = splitLines(zzuArt());
    int widest = 0;
    for (const std::string& l : art) widest = std::max(widest, tui::visibleWidth(l));
    if (tui::width() < widest + 2 || tui::height() < static_cast<int>(art.size()) + 1) return;

    std::vector<std::string> shown;
    for (size_t i = 0; i < art.size(); ++i) {
        double t = static_cast<double>(i) / art.size();
        shown.push_back(tui::fg(tui::mix(kPink, kCyan, t)) + tui::padRight(art[i], widest));
        fx::show(shown);
        if (tui::readKey(14).key != Key::None) return;
    }
    double until = fx::t() + 0.8;
    while (fx::t() < until) {
        fx::show(shown);
        if (tui::readKey(kFrameMs).key != Key::None) return;
    }
}

void boot() {
    struct Step {
        const char* text;
        const char* status;
    };
    static const Step steps[] = {
        {"PTA//OS 2.0  ·  ZZU lab node", ""},
        {"loading scoring kernel", "OK"},
        {"tier tables: 4 x 3 x 3 -> 10 values", "OK"},
        {"mounting record store", "OK"},
        {"calibrating neon", "OK"},
        {"waking companion unit", "OK"},
    };
    const int count = sizeof(steps) / sizeof(steps[0]);
    const double cps = 110;  // typing speed
    double start = fx::t();
    bool skip = false;
    while (true) {
        double elapsed = skip ? 1e9 : fx::t() - start;
        int budget = static_cast<int>(elapsed * cps);
        std::vector<std::string> lines = {"", ""};
        bool typing = false;
        for (int i = 0; i < count; ++i) {
            std::string text = steps[i].text;
            int len = static_cast<int>(text.size());
            if (budget <= 0) break;
            if (budget < len) {
                lines.push_back(paint(kPink, "› ") + paint(kText, text.substr(0, static_cast<size_t>(budget))) + caret());
                typing = true;
                break;
            }
            budget -= len + 6;  // short pause after each line
            std::string status = *steps[i].status ? paint(kGreen, std::string("[ ") + steps[i].status + " ]") : "";
            lines.push_back(paint(kPink, "› ") + paint(i == 0 ? kCyan : kText, tui::padRight(text, 40)) +
                            (i == 0 ? "" : paint(kFaint, " ··· ") + status));
        }
        double typedAt = 0;
        for (int i = 0; i < count; ++i) typedAt += std::string(steps[i].text).size() + 6;
        double barStart = typedAt / cps;
        double progress = skip ? 1.0 : std::min(1.0, std::max(0.0, (elapsed - barStart) / 0.8));
        if (!typing && elapsed >= barStart) {
            lines.push_back("");
            lines.push_back(paint(kMuted, "  boot  ") + tui::bar(progress, 40, tui::mix(kCyan, kPink, progress), kFaint) +
                            paint(kText, fmt("  %3.0f%%", progress * 100)));
            if (progress >= 1.0) {
                lines.push_back("");
                lines.push_back("  " + strong(kGreen, "SYSTEM READY") + paint(kMuted, "  ·  press any key"));
            }
        }
        for (std::string& l : lines) l = tui::padRight(l, kW);
        fx::show(lines);
        KeyEvent k = tui::readKey(kFrameMs);
        if (progress >= 1.0 && elapsed > barStart + 1.6) return;
        if (k.key != Key::None && k.key != Key::Resize) {
            if (progress >= 1.0) return;
            skip = true;
        }
    }
}

// ------------------------------------------------------------------ login --

bool loginScreen() {
    std::string name;
    std::string error;
    double shakeStart = -10;
    double grantedAt = -1;
    static const int kShake[] = {0, 4, 0, 4, 1, 3, 2, 2};
    fx::say("who goes there?", 3);
    while (true) {
        std::vector<std::string> lines = {""};
        for (const std::string& l : logo()) lines.push_back("  " + l);
        lines.push_back(paint(kMuted, "  PTA lab score analyzer · Zhengzhou University"));
        lines.push_back("");
        int frame = static_cast<int>((fx::t() - shakeStart) / 0.035);
        std::string pad(frame >= 0 && frame < 8 ? kShake[frame] : 2, ' ');
        Rgb border = grantedAt > 0 ? kGreen : error.empty() ? neonBorder() : kRed;
        lines.push_back(pad + boxTop("ACCESS TERMINAL", border));
        lines.push_back(pad + boxRow(paint(kMuted, "account  ") + paint(kPink, "› ") + strong(kText, name) +
                                         (grantedAt > 0 ? "" : caret()),
                                     border));
        std::string status = grantedAt > 0 ? strong(kGreen, "✓ ACCESS GRANTED")
                             : error.empty() ? paint(kFaint, "awaiting credentials…")
                                             : strong(kRed, "✗ ACCESS DENIED") + paint(kMuted, "  " + error);
        lines.push_back(pad + boxRow(status, border));
        lines.push_back(pad + boxBottom(border));
        lines.push_back(footer("type your account · enter sign in · esc quit"));
        fx::show(lines);

        if (grantedAt > 0) {
            if (fx::t() - grantedAt > 0.9) return true;
            tui::readKey(kFrameMs);
            continue;
        }
        KeyEvent k = tui::readKey(kFrameMs);
        if (k.key == Key::Esc) return false;
        if (k.key == Key::Enter) {
            if (name == kAccountName) {
                grantedAt = fx::t();
                fx::celebrate("welcome back!");
                fx::burstAt(7, kW / 2, 36, 1.2);
            } else {
                error = name.empty() ? "empty account" : "unknown \"" + name + "\"";
                shakeStart = fx::t();
                fx::oops("intruder?!");
            }
        } else if (k.key == Key::Backspace) {
            if (!name.empty()) name.pop_back();
        } else if (k.key == Key::Char && k.ch >= 33 && k.ch <= 126 && name.size() < 24) {
            name += k.ch;
            error.clear();
        }
    }
}

// -------------------------------------------------------------- main menu --

int mainMenu(const RecordList& list, int& selected) {
    struct Item {
        const char* label;
        const char* hint;
    };
    static const Item items[] = {
        {"Input records", "live score preview while you type"},
        {"Records", "browse everything entered so far"},
        {"Report", "report card, strengths and weaknesses"},
        {"Scoring rules", "tune the points of every tier"},
        {"Quit", "power down"},
    };
    const int count = 5;
    double moved = fx::t();
    while (true) {
        std::vector<std::string> lines = {""};
        for (const std::string& l : logo()) lines.push_back("  " + l);
        lines.push_back(paint(kMuted, "  PTA lab score analyzer   ") + statusLine(list));
        lines.push_back("");
        Rgb border = neonBorder();
        lines.push_back(boxTop("MAIN MENU", border));
        for (int i = 0; i < count; ++i) {
            std::string num = "0" + std::to_string(i + 1);
            if (i == selected) {
                // the label "types in" briefly after moving
                std::string label = items[i].label;
                size_t shown = std::min(label.size(), static_cast<size_t>((fx::t() - moved) * 90) + 1);
                std::string row = tui::fg(kPink) + "▶ " + tui::fg(kYellow) + tui::kBold + num + "  " + tui::fg(kText) +
                                  tui::padRight(label.substr(0, shown), 15) + tui::kNormal + tui::fg(kCyan) + items[i].hint;
                lines.push_back(selectedRow(row, border));
            } else {
                lines.push_back(boxRow("  " + paint(kFaint, num) + "  " + paint(kText, tui::padRight(items[i].label, 15)) +
                                           paint(kFaint, items[i].hint),
                                       border));
            }
        }
        lines.push_back(boxBottom(border));
        lines.push_back(footer("↑↓ move · enter open · 1-5 jump · q quit"));
        fx::show(lines);

        KeyEvent k = tui::readKey(kFrameMs);
        int before = selected;
        if (k.key == Key::Up || (k.key == Key::Char && k.ch == 'k')) selected = (selected + count - 1) % count;
        else if (k.key == Key::Down || k.key == Key::Tab || (k.key == Key::Char && k.ch == 'j'))
            selected = (selected + 1) % count;
        else if (k.key == Key::Enter) {
            fx::burstAt(5 + selected, 6, 18, 0.8);
            return selected;
        } else if (isBack(k)) {
            return count - 1;
        } else if (k.key == Key::Char && k.ch >= '1' && k.ch <= '0' + count) {
            selected = k.ch - '1';
            fx::burstAt(5 + selected, 6, 18, 0.8);
            return selected;
        }
        if (selected != before) moved = fx::t();
    }
}

// ------------------------------------------------------------ input form --

struct Form {
    std::string student, time, wrong, dup;
    int problem = 0;
    bool passed = true;
    int field = 0;
    std::string flash;
    bool flashGood = true;
    double flashAt = -10;
};

enum Field { kStudent, kProblem, kTime, kPassed, kWrong, kDup, kFieldCount };

int toInt(const std::string& s) { return s.empty() ? 0 : std::atoi(s.c_str()); }

std::string textField(const std::string& value, bool focused, const std::string& unit) {
    if (focused) return paint(kPink, "› ") + strong(kText, value) + caret() + paint(kMuted, unit);
    if (value.empty()) return "  " + paint(kFaint, "—");
    return "  " + paint(kText, value) + paint(kMuted, unit);
}

std::string choiceField(const std::vector<std::string>& options, int chosen, bool focused) {
    std::string out = focused ? paint(kPink, "‹ ") : "  ";
    for (size_t i = 0; i < options.size(); ++i) {
        if (static_cast<int>(i) == chosen)
            out += tui::bg(focused ? kCyan : kFaint) + tui::fg(focused ? Rgb{10, 8, 20} : kText) + tui::kBold + " " +
                   options[i] + " " + tui::kReset;
        else
            out += paint(kMuted, " " + options[i] + " ");
    }
    return out + (focused ? paint(kPink, " ›") : "");
}

std::string partRow(const char* name, const std::string& value, const char* range, int points, int maxPoints,
                    bool active) {
    double f = maxPoints > 0 ? static_cast<double>(points) / maxPoints : 0;
    std::string b = active ? tui::bar(f, 18, scoreColor(f), kFaint) : paint(kFaint, tui::repeat("─", 18));
    std::string pts = active ? strong(scoreColor(f), fmt("%2.0f", points)) : paint(kFaint, " –");
    return paint(kMuted, tui::padRight(name, 11)) + paint(kText, tui::padRight(value, 8)) +
           paint(kMuted, tui::padRight(range, 11)) + b + "  " + pts;
}

std::vector<std::string> preview(const Form& f, const ScoreConfig& config) {
    Record r = {};
    r.problem = static_cast<char>('A' + f.problem);
    r.time = toInt(f.time);
    r.passed = f.passed;
    r.wrongTimes = toInt(f.wrong);
    r.duplicateRate = toInt(f.dup);
    double score = scoreRecord(r, config);
    double weight = kProblemWeight[f.problem];
    bool qualifies = r.passed && r.duplicateRate <= kMaxDuplicateRate;

    int tp = config.time[timeTier(r.time)];
    int wp = config.wrong[wrongTier(r.wrongTimes)];
    int dp = qualifies ? config.duplicate[duplicateTier(r.duplicateRate)] : 0;

    Rgb border = qualifies ? kViolet : kRed;
    std::vector<std::string> out;
    out.push_back(boxTop("LIVE SCORE", border));
    out.push_back(boxRow(partRow("Time", f.time.empty() ? "—" : f.time + "s", kTimeRange[timeTier(r.time)], tp,
                                 kMaxTierScore, !f.time.empty() && qualifies),
                         border));
    out.push_back(boxRow(partRow("Wrong", f.wrong.empty() ? "—" : f.wrong, kWrongRange[wrongTier(r.wrongTimes)], wp,
                                 kMaxTierScore, !f.wrong.empty() && qualifies),
                         border));
    const char* dupRange = r.duplicateRate > kMaxDuplicateRate ? "> 50%" : kDupRange[duplicateTier(r.duplicateRate)];
    out.push_back(boxRow(partRow("Duplicate", f.dup.empty() ? "—" : f.dup + "%", dupRange, dp, kMaxTierScore,
                                 !f.dup.empty() && qualifies),
                         border));
    out.push_back(boxRow(partRow("Passed", f.passed ? "yes" : "no", "", f.passed ? kPassScore : 0, kPassScore, qualifies),
                         border));
    out.push_back(boxRule(border));
    double max = 100.0 * weight;
    std::string verdict;
    bool complete = !f.time.empty() && !f.wrong.empty() && !f.dup.empty();
    if (!complete && qualifies) {
        out.push_back(boxRow(paint(kFaint, tui::repeat("─", 20)) + "  " + paint(kMuted, "fill in all fields to see the score"),
                             border));
        out.push_back(boxBottom(border));
        return out;
    }
    if (!r.passed) verdict = strong(kRed, "✗ NOT PASSED") + paint(kMuted, "  problem scores 0");
    else if (r.duplicateRate > kMaxDuplicateRate) verdict = strong(kRed, "✗ DUPLICATE > 50%") + paint(kMuted, "  scores 0");
    else
        verdict = paint(kMuted, "problem ") + strong(kText, std::string(1, r.problem)) +
                  paint(kMuted, " × " + fmt("%.2f", weight) + "  =  ") + strong(scoreColor(score / max), fmt("%.2f", score)) +
                  paint(kMuted, " / " + fmt("%.0f", max) + " pts");
    out.push_back(boxRow(tui::bar(score / max, 20, scoreColor(score / max), kFaint) + "  " + verdict, border));
    out.push_back(boxBottom(border));
    return out;
}

void inputScreen(RecordList& list, const ScoreConfig& config) {
    Form f;
    static const char* const kLabels[] = {"Student No", "Problem", "Time used", "Passed", "Wrong times", "Duplicate"};
    while (true) {
        Rgb border = neonBorder();
        std::vector<std::string> lines = {titleBar("Input records", list), ""};
        lines.push_back(boxTop("NEW SUBMISSION", border));
        for (int i = 0; i < kFieldCount; ++i) {
            bool on = f.field == i;
            std::string widget;
            switch (i) {
                case kStudent:
                    widget = textField(f.student, on, "");
                    if (!f.student.empty() && f.student.size() != 12)
                        widget += paint(kYellow, "  " + std::to_string(f.student.size()) + "/12 digits");
                    break;
                case kProblem: widget = choiceField({"A", "B", "C", "D", "E"}, f.problem, on); break;
                case kTime: widget = textField(f.time, on, f.time.empty() ? "" : " s"); break;
                case kPassed: widget = choiceField({"Yes", "No"}, f.passed ? 0 : 1, on); break;
                case kWrong: widget = textField(f.wrong, on, ""); break;
                case kDup: widget = textField(f.dup, on, f.dup.empty() ? "" : " %"); break;
            }
            std::string label = on ? strong(kCyan, tui::padRight(kLabels[i], 13)) : paint(kMuted, tui::padRight(kLabels[i], 13));
            lines.push_back(boxRow(label + widget, border));
        }
        lines.push_back(boxBottom(border));
        std::vector<std::string> pv = preview(f, config);
        int scoreRow = static_cast<int>(lines.size() + pv.size()) - 2;
        for (const std::string& l : pv) lines.push_back(l);
        bool fresh = fx::t() - f.flashAt < 4;
        lines.push_back(fresh && !f.flash.empty() ? "  " + strong(f.flashGood ? kGreen : kRed, f.flash) : "");
        lines.push_back(footer("↑↓ field · ←→ choose · enter next / save on last · esc back"));
        fx::show(lines);

        KeyEvent k = tui::readKey(kFrameMs);
        if (k.key == Key::None || k.key == Key::Resize) continue;
        bool isChoice = f.field == kProblem || f.field == kPassed;
        if (k.key == Key::Esc) return;
        if (k.key == Key::Up) f.field = (f.field + kFieldCount - 1) % kFieldCount;
        else if (k.key == Key::Down || k.key == Key::Tab) f.field = (f.field + 1) % kFieldCount;
        else if (isChoice && (k.key == Key::Left || k.key == Key::Right)) {
            int step = k.key == Key::Left ? -1 : 1;
            if (f.field == kProblem) f.problem = (f.problem + step + kProblemCount) % kProblemCount;
            else f.passed = !f.passed;
        } else if (isChoice && k.key == Key::Char) {
            char c = static_cast<char>(std::toupper(static_cast<unsigned char>(k.ch)));
            if (f.field == kProblem && c >= 'A' && c < 'A' + kProblemCount) f.problem = c - 'A';
            if (f.field == kPassed && (c == 'Y' || c == '1')) f.passed = true;
            if (f.field == kPassed && (c == 'N' || c == '0')) f.passed = false;
        } else if (!isChoice && k.key == Key::Char && k.ch >= '0' && k.ch <= '9') {
            std::string* s = f.field == kStudent ? &f.student : f.field == kTime ? &f.time : f.field == kWrong ? &f.wrong : &f.dup;
            size_t maxLen = f.field == kStudent ? 12 : f.field == kTime ? 7 : f.field == kWrong ? 5 : 3;
            std::string next = (*s == "0" ? "" : *s) + k.ch;
            if (next.size() <= maxLen && !(f.field == kDup && toInt(next) > 100)) *s = next;
        } else if (!isChoice && k.key == Key::Backspace) {
            std::string* s = f.field == kStudent ? &f.student : f.field == kTime ? &f.time : f.field == kWrong ? &f.wrong : &f.dup;
            if (!s->empty()) s->pop_back();
        } else if (k.key == Key::Enter) {
            if (f.field < kFieldCount - 1) {
                f.field++;
                continue;
            }
            int missing = -1;
            if (f.student.size() != 12 || std::atoll(f.student.c_str()) < kMinStudentNumber) missing = kStudent;
            else if (f.time.empty()) missing = kTime;
            else if (f.wrong.empty()) missing = kWrong;
            else if (f.dup.empty()) missing = kDup;
            if (missing >= 0) {
                f.field = missing;
                f.flashGood = false;
                f.flashAt = fx::t();
                f.flash = missing == kStudent ? "✗ student number must be 12 digits" : std::string("✗ please fill in ") + kLabels[missing];
                fx::oops("missing data!");
                continue;
            }
            Record r = {};
            r.studentNumber = std::atoll(f.student.c_str());
            r.problem = static_cast<char>('A' + f.problem);
            r.time = toInt(f.time);
            r.passed = f.passed;
            r.wrongTimes = toInt(f.wrong);
            r.duplicateRate = toInt(f.dup);
            r.result = scoreRecord(r, config);
            append(list, r);
            f.flashGood = true;
            f.flashAt = fx::t();
            f.flash = "✓ saved " + std::string(1, r.problem) + " for " + f.student + "  +" + fmt("%.2f", r.result) +
                      " pts · next problem ready";
            fx::burstAt(scoreRow, 12, r.result > 0 ? 40 : 10, r.result > 0 ? 1.2 : 0.5);
            if (r.result > 0) fx::celebrate("+" + fmt("%.2f", r.result) + "!");
            else fx::oops("0 pts...");
            f.problem = (f.problem + 1) % kProblemCount;
            f.time.clear();
            f.wrong.clear();
            f.dup.clear();
            f.passed = true;
            f.field = kTime;
        }
    }
}

// ---------------------------------------------------------------- records --

std::string recordRow(const Record& r, double result) {
    double f = result / problemMax(r.problem);
    return paint(kText, std::to_string(r.studentNumber)) + "   " + strong(kCyan, std::string(1, r.problem)) + "    " +
           paint(kText, tui::padRight(std::to_string(r.time) + "s", 8)) +
           (r.passed ? paint(kGreen, "✓") : paint(kRed, "✗")) + "     " +
           paint(kText, tui::padRight(std::to_string(r.wrongTimes), 7)) +
           paint(r.duplicateRate > kMaxDuplicateRate ? kRed : kText, tui::padRight(std::to_string(r.duplicateRate) + "%", 6)) +
           tui::bar(f, 8, scoreColor(f), kFaint) + " " + strong(scoreColor(f), fmt("%6.2f", result));
}

void recordsScreen(RecordList& list, const ScoreConfig& config) {
    int offset = 0;
    double opened = fx::t();
    while (true) {
        std::vector<const RecordNode*> rows;
        for (const RecordNode* p = list.head; p; p = p->next) rows.push_back(p);
        int visible = std::max(3, tui::height() - 13);
        int maxOffset = std::max(0, static_cast<int>(rows.size()) - visible);
        offset = std::min(offset, maxOffset);
        Rgb border = neonBorder();

        std::vector<std::string> lines = {titleBar("Records", list), ""};
        lines.push_back(boxTop("ALL SUBMISSIONS", border));
        if (rows.empty()) {
            lines.push_back(boxRow("", border));
            lines.push_back(boxRow(paint(kMuted, "No records yet. Choose ") + strong(kText, "Input records") +
                                       paint(kMuted, " from the menu."),
                                   border));
            lines.push_back(boxRow("", border));
        } else {
            lines.push_back(boxRow(paint(kMuted, "STUDENT NO     PROB TIME    PASS  WRONG  DUP   SCORE"), border));
            lines.push_back(boxRule(border));
            int end = std::min(static_cast<int>(rows.size()), offset + visible);
            for (int i = offset; i < end; ++i) {
                // rows slide in one after another when the screen opens
                double appear = (fx::t() - opened) * 25 - (i - offset);
                if (appear < 0) lines.push_back(boxRow("", border));
                else
                    lines.push_back(boxRow(tui::truncate(recordRow(rows[i]->data, scoreRecord(rows[i]->data, config)),
                                                         std::max(1, static_cast<int>(appear * 12))),
                                           border));
            }
        }
        lines.push_back(boxBottom(border));
        std::string range = rows.empty() ? "" : "rows " + std::to_string(offset + 1) + "–" +
                                                    std::to_string(std::min(static_cast<int>(rows.size()), offset + visible)) +
                                                    " of " + std::to_string(rows.size()) + " · ";
        lines.push_back(footer(range + (maxOffset > 0 ? "↑↓ scroll · " : "") + "esc back"));
        fx::show(lines);

        KeyEvent k = tui::readKey(kFrameMs);
        if (k.key == Key::Up) offset = std::max(0, offset - 1);
        else if (k.key == Key::Down) offset = std::min(maxOffset, offset + 1);
        else if (isBack(k) || k.key == Key::Enter) return;
    }
}

// ----------------------------------------------------------------- report --

double easeOut(double t) { return 1 - (1 - t) * (1 - t) * (1 - t); }

void reportCard(RecordList& list, const ScoreConfig& config, long long id) {
    std::vector<Record*> recs;
    for (RecordNode* p = list.head; p; p = p->next)
        if (p->data.studentNumber == id) {
            p->data.result = scoreRecord(p->data, config);
            recs.push_back(&p->data);
        }
    double total = 0;
    double speed = 0, accuracy = 0, originality = 0;
    int eligible = 0;
    for (Record* r : recs) {
        total += r->result;
        if (r->passed && r->duplicateRate <= kMaxDuplicateRate) {
            speed += config.time[timeTier(r->time)];
            accuracy += config.wrong[wrongTier(r->wrongTimes)];
            originality += config.duplicate[duplicateTier(r->duplicateRate)];
            ++eligible;
        }
    }
    if (eligible) {
        speed /= eligible * kMaxTierScore;
        accuracy /= eligible * kMaxTierScore;
        originality /= eligible * kMaxTierScore;
    }

    const double duration = 0.9;
    double start = fx::t();
    bool reacted = false;
    int totalRow = 0;
    while (true) {
        double t = easeOut(std::min(1.0, (fx::t() - start) / duration));
        Rgb border = neonBorder();
        std::vector<std::string> lines = {titleBar("Report", list), ""};
        lines.push_back(boxTop("REPORT CARD · " + std::to_string(id), border));
        for (Record* r : recs) {
            double max = problemMax(r->problem);
            double f = r->result / max * t;
            std::string detail = r->passed ? (r->duplicateRate > kMaxDuplicateRate ? paint(kRed, "dup > 50%") :
                                                  paint(kMuted, tui::padRight(std::to_string(r->time) + "s · " +
                                                                                 std::to_string(r->wrongTimes) + " wrong · " +
                                                                                 std::to_string(r->duplicateRate) + "%", 22)))
                                           : paint(kRed, "not passed");
            lines.push_back(boxRow(strong(kCyan, std::string(1, r->problem)) + "  " + tui::padRight(detail, 23) +
                                       tui::bar(f, 18, scoreColor(f), kFaint) + "  " +
                                       strong(scoreColor(f), fmt("%5.2f", r->result * t)) + paint(kMuted, fmt(" / %.0f", max)),
                                   border));
        }
        lines.push_back(boxRule(border));
        double tf = std::min(1.0, total / 100.0) * t;
        totalRow = static_cast<int>(lines.size());
        lines.push_back(boxRow(strong(kText, "TOTAL  ") + tui::bar(tf, 36, scoreColor(tf), kFaint) + "  " +
                                   strong(scoreColor(tf), fmt("%6.2f", total * t)) + paint(kMuted, " / 100"),
                               border));
        if (total > 100.0 + 1e-9)
            lines.push_back(boxRow(paint(kYellow, "! some problems were entered more than once"), border));
        lines.push_back(boxRule(border));
        if (eligible == 0) {
            lines.push_back(boxRow(paint(kMuted, "No passed problems yet, so there is no profile to show."), border));
        } else {
            struct Trait {
                const char* name;
                const char* meaning;
                double value;
            };
            Trait traits[] = {{"Speed", "time used", speed}, {"Accuracy", "few wrong tries", accuracy},
                              {"Originality", "low duplicates", originality}};
            for (const Trait& tr : traits)
                lines.push_back(boxRow(paint(kText, tui::padRight(tr.name, 13)) +
                                           tui::bar(tr.value * t, 24, scoreColor(tr.value), kFaint) + "  " +
                                           paint(kMuted, fmt("%3.0f%%", tr.value * 100 * t) + "  " + tr.meaning),
                                       border));
            const Trait* best = &traits[0];
            const Trait* worst = &traits[0];
            for (const Trait& tr : traits) {
                if (tr.value > best->value) best = &tr;
                if (tr.value < worst->value) worst = &tr;
            }
            std::string summary = best->value - worst->value < 1e-9
                                      ? strong(kGreen, "◆ Balanced across speed, accuracy and originality")
                                      : strong(kGreen, std::string("▲ STRENGTH  ") + best->name) + "     " +
                                            strong(kYellow, std::string("▼ WORK ON  ") + worst->name);
            lines.push_back(boxRow("", border));
            lines.push_back(boxRow(t >= 1.0 ? summary : "", border));
        }
        lines.push_back(boxBottom(border));
        lines.push_back(footer("esc back to the student list"));
        fx::show(lines);

        if (t >= 1.0 && !reacted) {
            reacted = true;
            if (total >= 80) {
                fx::celebrate("legend!");
                for (int i = 0; i < 4; ++i) fx::burstAt(totalRow, 10 + i * 12, 20, 1.3);
            } else if (total >= 60) {
                fx::celebrate("solid run");
                fx::burstAt(totalRow, 30, 18, 0.9);
            } else {
                fx::oops("keep grinding");
            }
        }
        KeyEvent k = tui::readKey(kFrameMs);
        if (k.key == Key::None || k.key == Key::Resize) continue;
        if (t < 1.0) {
            start = -10;  // any key finishes the animation
            continue;
        }
        if (isBack(k) || k.key == Key::Enter) return;
    }
}

void reportScreen(RecordList& list, const ScoreConfig& config) {
    int selected = 0;
    while (true) {
        Stats s = stats(list);
        Rgb border = neonBorder();
        std::vector<std::string> lines = {titleBar("Report", list), ""};
        lines.push_back(boxTop("CHOOSE A STUDENT", border));
        if (s.students.empty()) {
            lines.push_back(boxRow("", border));
            lines.push_back(boxRow(paint(kMuted, "No records yet. Choose ") + strong(kText, "Input records") +
                                       paint(kMuted, " from the menu."),
                                   border));
            lines.push_back(boxRow("", border));
        }
        selected = std::min(selected, std::max(0, static_cast<int>(s.students.size()) - 1));
        for (size_t i = 0; i < s.students.size(); ++i) {
            double total = 0;
            int n = 0;
            for (const RecordNode* p = list.head; p; p = p->next)
                if (p->data.studentNumber == s.students[i]) {
                    total += scoreRecord(p->data, config);
                    ++n;
                }
            double f = std::min(1.0, total / 100);
            std::string text = std::to_string(s.students[i]) + "   " + tui::padRight(plural(n, "problem"), 12);
            if (static_cast<int>(i) == selected)
                lines.push_back(selectedRow(tui::fg(kPink) + "▶ " + tui::fg(kText) + text +
                                                tui::fg(scoreColor(f)) + tui::repeat("█", static_cast<int>(f * 20)) +
                                                tui::fg(kFaint) + tui::repeat("─", 20 - static_cast<int>(f * 20)) + "  " +
                                                tui::fg(scoreColor(f)) + fmt("%6.2f", total),
                                            border));
            else
                lines.push_back(boxRow("  " + paint(kText, text) + tui::bar(f, 20, scoreColor(f), kFaint) + "  " +
                                           paint(scoreColor(f), fmt("%6.2f", total)),
                                       border));
        }
        lines.push_back(boxBottom(border));
        lines.push_back(footer(s.students.empty() ? "esc back" : "↑↓ move · enter open report · esc back"));
        fx::show(lines);

        KeyEvent k = tui::readKey(kFrameMs);
        int n = static_cast<int>(s.students.size());
        if (isBack(k)) return;
        if (n == 0) {
            if (k.key == Key::Enter) return;
            continue;
        }
        if (k.key == Key::Up) selected = (selected + n - 1) % n;
        else if (k.key == Key::Down) selected = (selected + 1) % n;
        else if (k.key == Key::Enter) {
            fx::transition(fx::Transition::FlipForward);
            reportCard(list, config, s.students[selected]);
            fx::transition(fx::Transition::FlipBack);
        }
    }
}

// ------------------------------------------------------------------ rules --

void rulesScreen(const RecordList& list, ScoreConfig& config) {
    struct Field {
        const char* item;
        const char* range;
        int* points;
    };
    const Field fields[] = {
        {"Time", kTimeRange[0], &config.time[0]},       {"Time", kTimeRange[1], &config.time[1]},
        {"Time", kTimeRange[2], &config.time[2]},       {"Time", kTimeRange[3], &config.time[3]},
        {"Wrong", kWrongRange[0], &config.wrong[0]},    {"Wrong", kWrongRange[1], &config.wrong[1]},
        {"Wrong", kWrongRange[2], &config.wrong[2]},    {"Duplicate", kDupRange[0], &config.duplicate[0]},
        {"Duplicate", kDupRange[1], &config.duplicate[1]}, {"Duplicate", kDupRange[2], &config.duplicate[2]},
    };
    const int count = sizeof(fields) / sizeof(fields[0]);
    int selected = 0;
    while (true) {
        Rgb border = neonBorder();
        std::vector<std::string> lines = {titleBar("Scoring rules", list), ""};
        lines.push_back(boxTop("POINTS PER TIER", border));
        for (int i = 0; i < count; ++i) {
            const Field& f = fields[i];
            double frac = static_cast<double>(*f.points) / kMaxTierScore;
            bool newGroup = i > 0 && std::string(fields[i - 1].item) != f.item;
            if (newGroup) lines.push_back(boxRow("", border));
            std::string name = (i == 0 || newGroup) ? f.item : "";
            if (i == selected) {
                int filled = static_cast<int>(frac * 22 + 0.5);
                std::string row = tui::fg(kPink) + "▶ " + tui::fg(kCyan) + tui::kBold + tui::padRight(name, 11) +
                                  tui::kNormal + tui::fg(kText) + tui::padRight(f.range, 11) + tui::fg(kPink) + "◂ " +
                                  tui::fg(kYellow) + tui::kBold + fmt("%2.0f", *f.points) + tui::kNormal + tui::fg(kPink) +
                                  " ▸  " + tui::fg(scoreColor(frac)) + tui::repeat("█", filled) + tui::fg(kFaint) +
                                  tui::repeat("─", 22 - filled);
                lines.push_back(selectedRow(row, border));
            } else {
                lines.push_back(boxRow("  " + paint(kCyan, tui::padRight(name, 11)) + paint(kMuted, tui::padRight(f.range, 11)) +
                                           "  " + paint(kText, fmt("%2.0f", *f.points)) + "    " +
                                           tui::bar(frac, 22, scoreColor(frac), kFaint),
                                       border));
            }
        }
        lines.push_back(boxRule(border));
        int best = config.time[0] + config.wrong[0] + config.duplicate[0] + kPassScore;
        lines.push_back(boxRow(paint(kMuted, "Passed is fixed at 25 · not passed or dup > 50% scores 0"), border));
        lines.push_back(boxRow(paint(kMuted, "Weights A–E 0.10 0.15 0.20 0.25 0.30 · best problem now ") +
                                   strong(best == 100 ? kGreen : kYellow, std::to_string(best) + "/100"),
                               border));
        lines.push_back(boxBottom(border));
        lines.push_back(footer("↑↓ choose · ←→ adjust · r reset defaults · esc back"));
        fx::show(lines);

        KeyEvent k = tui::readKey(kFrameMs);
        int* v = fields[selected].points;
        if (k.key == Key::Up) selected = (selected + count - 1) % count;
        else if (k.key == Key::Down || k.key == Key::Tab) selected = (selected + 1) % count;
        else if (k.key == Key::Left || (k.key == Key::Char && (k.ch == '-' || k.ch == 'h'))) *v = std::max(0, *v - 1);
        else if (k.key == Key::Right || (k.key == Key::Char && (k.ch == '+' || k.ch == '=' || k.ch == 'l')))
            *v = std::min(kMaxTierScore, *v + 1);
        else if (k.key == Key::Char && k.ch == 'r') {
            config = defaultScoreConfig();
            fx::celebrate("defaults restored");
        } else if (isBack(k) || k.key == Key::Enter) {
            return;
        }
    }
}

// ---------------------------------------------------------------- goodbye --

void goodbye() {
    fx::celebrate("bye! ✦");
    double start = fx::t();
    bool burst = false;
    while (fx::t() - start < 1.3) {
        std::vector<std::string> lines = {"", ""};
        for (const std::string& l : logo()) lines.push_back("  " + l);
        lines.push_back("");
        lines.push_back("  " + tui::gradient("POWERING DOWN · see you next time ✦", kPink, kCyan));
        fx::show(lines);
        if (!burst) {
            for (int i = 0; i < 5; ++i) fx::burstAt(1, 6 + i * 9, 22, 1.4);
            burst = true;
        }
        tui::readKey(kFrameMs);
    }
}

}  // namespace

void runTui(RecordList& list, ScoreConfig& config) {
    tui::begin();
    zzuIntro();
    boot();
    fx::transition(fx::Transition::FlipForward);
    if (!loginScreen()) {
        tui::end();
        return;
    }
    fx::transition(fx::Transition::FlipForward);
    int selected = 0;
    while (true) {
        int choice = mainMenu(list, selected);
        if (choice >= 4) break;
        fx::transition(fx::Transition::FlipForward);
        switch (choice) {
            case 0: inputScreen(list, config); break;
            case 1: recordsScreen(list, config); break;
            case 2: reportScreen(list, config); break;
            case 3: rulesScreen(list, config); break;
        }
        fx::transition(fx::Transition::FlipBack);
    }
    goodbye();
    tui::end();
    std::cout << tui::gradient("Thanks for using StudentPTA ✦", kCyan, kPink) << "\n";
}
