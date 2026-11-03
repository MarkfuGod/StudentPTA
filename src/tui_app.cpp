#include "tui_app.h"

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "banner.h"
#include "menu.h"
#include "tui.h"

using tui::Key;
using tui::KeyEvent;
using tui::Rgb;

namespace {

// ---------------------------------------------------------------- palette --

const Rgb kCyan = {90, 200, 250};
const Rgb kViolet = {165, 125, 255};
const Rgb kPink = {255, 110, 180};
const Rgb kGreen = {90, 220, 140};
const Rgb kYellow = {250, 200, 80};
const Rgb kRed = {255, 95, 95};
const Rgb kText = {228, 228, 238};
const Rgb kMuted = {140, 140, 160};
const Rgb kFaint = {70, 70, 90};
const Rgb kSelect = {48, 44, 92};

const int kW = 66;  // width of every box

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

// ------------------------------------------------------------------ boxes --

std::string boxTop(const std::string& title, Rgb c) {
    std::string t = title.empty() ? "" : " " + title + " ";
    int dashes = kW - 3 - tui::visibleWidth(t);
    return paint(c, "╭─") + strong(kText, t) + paint(c, tui::repeat("─", dashes) + "╮");
}

std::string boxRow(const std::string& content, Rgb c) {
    return paint(c, "│") + " " + tui::padRight(tui::truncate(content, kW - 4), kW - 4) + " " + paint(c, "│");
}

std::string boxRule(Rgb c) { return paint(c, "├" + tui::repeat("─", kW - 2) + "┤"); }
std::string boxBottom(Rgb c) { return paint(c, "╰" + tui::repeat("─", kW - 2) + "╯"); }

// Row with a solid highlight bar; content must not contain kReset.
std::string selectedRow(const std::string& content, Rgb c) {
    return paint(c, "│") + tui::bg(kSelect) + " " + tui::padRight(tui::truncate(content, kW - 4), kW - 4) + " " + tui::kReset +
           paint(c, "│");
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
    return {tui::gradient(top, kCyan, kPink), tui::gradient(bottom, kCyan, kPink)};
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

std::string plural(size_t n, const char* word) {
    return std::to_string(n) + " " + word + (n == 1 ? "" : "s");
}

std::string statusLine(const RecordList& list) {
    Stats s = stats(list);
    return paint(kGreen, "●") + paint(kMuted, " " + plural(s.records, "record") + " · " +
                                                 plural(s.students.size(), "student"));
}

std::string titleBar(const std::string& screen, const RecordList& list) {
    std::string left = tui::gradient("StudentPTA", kCyan, kPink) + paint(kMuted, "  ›  ") + strong(kText, screen);
    std::string right = statusLine(list);
    int gap = kW - tui::visibleWidth(left) - tui::visibleWidth(right);
    return left + std::string(std::max(1, gap), ' ') + right;
}

const char* const kTimeRange[] = {"<= 500s", "<= 1000s", "<= 3000s", "> 3000s"};
const char* const kWrongRange[] = {"0 times", "1-3 times", "> 3 times"};
const char* const kDupRange[] = {"<= 20%", "<= 35%", "<= 50%"};

double problemMax(char problem) { return 100.0 * kProblemWeight[problem - 'A']; }

// ------------------------------------------------------------------ intro --

void intro() {
    std::vector<std::string> art = splitLines(zzuArt());
    int widest = 0;
    for (const std::string& l : art) widest = std::max(widest, tui::visibleWidth(l));
    if (tui::width() < widest + 2 || tui::height() < static_cast<int>(art.size()) + 1) return;

    std::vector<std::string> shown;
    for (size_t i = 0; i < art.size(); ++i) {
        double t = static_cast<double>(i) / art.size();
        shown.push_back(tui::fg(tui::mix(Rgb{255, 70, 90}, Rgb{255, 170, 90}, t)) + art[i]);
        tui::present(shown);
        if (tui::waitKey(14)) {
            tui::readKey();
            return;
        }
    }
    if (tui::waitKey(700)) tui::readKey();
}

// ------------------------------------------------------------------ login --

bool loginScreen() {
    std::string name;
    std::string error;
    int shake = 0;
    static const int kShake[] = {0, 4, 0, 4, 1, 3, 2};
    while (true) {
        std::vector<std::string> lines = {"", ""};
        for (const std::string& l : logo()) lines.push_back("  " + l);
        lines.push_back(paint(kMuted, "  PTA lab score analyzer · Zhengzhou University"));
        lines.push_back("");
        std::string pad(kShake[shake], ' ');
        std::string field = strong(kText, name) + paint(kCyan, "▌");
        lines.push_back(pad + boxTop("Sign in", kViolet));
        lines.push_back(pad + boxRow(paint(kMuted, "Account   ") + field, kViolet));
        lines.push_back(pad + boxRow(error.empty() ? "" : paint(kRed, "✗ " + error), kViolet));
        lines.push_back(pad + boxBottom(kViolet));
        lines.push_back(footer("type your account · enter sign in · esc quit"));
        tui::present(lines);

        if (shake > 0) {
            shake = (shake + 1) % 7;
            tui::sleepMs(35);
            continue;
        }
        KeyEvent k = tui::readKey();
        if (k.key == Key::Esc) return false;
        if (k.key == Key::Enter) {
            if (name == kAccountName) return true;
            error = name.empty() ? "Please type an account name" : "Unknown account \"" + name + "\"";
            shake = 1;
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
        {"Input records", "add submissions with a live score preview"},
        {"Records", "browse everything entered so far"},
        {"Report", "report card, strengths and weaknesses"},
        {"Scoring rules", "tune the points of every tier"},
        {"Quit", ""},
    };
    const int count = 5;
    while (true) {
        std::vector<std::string> lines = {""};
        for (const std::string& l : logo()) lines.push_back("  " + l);
        lines.push_back(paint(kMuted, "  PTA lab score analyzer   ") + statusLine(list));
        lines.push_back("");
        lines.push_back(boxTop("Menu", kCyan));
        for (int i = 0; i < count; ++i) {
            std::string num = std::to_string(i + 1);
            if (i == selected) {
                std::string row = tui::fg(kPink) + "▶ " + tui::fg(kText) + tui::kBold + num + "  " +
                                  tui::padRight(items[i].label, 15) + "\033[22m" + tui::fg(kMuted) + items[i].hint;
                lines.push_back(selectedRow(row, kCyan));
            } else {
                lines.push_back(boxRow("  " + paint(kFaint, num) + "  " + paint(kText, tui::padRight(items[i].label, 15)) +
                                           paint(kFaint, items[i].hint),
                                       kCyan));
            }
        }
        lines.push_back(boxBottom(kCyan));
        lines.push_back(footer("↑↓ move · enter open · 1-5 jump · q quit"));
        tui::present(lines);

        KeyEvent k = tui::readKey();
        if (k.key == Key::Up || (k.key == Key::Char && k.ch == 'k')) selected = (selected + count - 1) % count;
        else if (k.key == Key::Down || k.key == Key::Tab || (k.key == Key::Char && k.ch == 'j'))
            selected = (selected + 1) % count;
        else if (k.key == Key::Enter) return selected;
        else if (k.key == Key::Esc || (k.key == Key::Char && k.ch == 'q')) return count - 1;
        else if (k.key == Key::Char && k.ch >= '1' && k.ch <= '0' + count) {
            selected = k.ch - '1';
            return selected;
        }
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
};

enum Field { kStudent, kProblem, kTime, kPassed, kWrong, kDup, kFieldCount };

int toInt(const std::string& s) { return s.empty() ? 0 : std::atoi(s.c_str()); }

std::string textField(const std::string& value, bool focused, const std::string& unit) {
    if (focused) return paint(kCyan, "› ") + strong(kText, value) + paint(kCyan, "▌") + paint(kMuted, unit);
    if (value.empty()) return "  " + paint(kFaint, "—");
    return "  " + paint(kText, value) + paint(kMuted, unit);
}

std::string choiceField(const std::vector<std::string>& options, int chosen, bool focused) {
    std::string out = focused ? paint(kCyan, "‹ ") : "  ";
    for (size_t i = 0; i < options.size(); ++i) {
        if (static_cast<int>(i) == chosen)
            out += tui::bg(focused ? kCyan : kFaint) + tui::fg(focused ? Rgb{20, 20, 30} : kText) + tui::kBold + " " +
                   options[i] + " " + tui::kReset;
        else
            out += paint(kMuted, " " + options[i] + " ");
    }
    return out + (focused ? paint(kCyan, " ›") : "");
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

    std::vector<std::string> out;
    out.push_back(boxTop("Live score", kViolet));
    out.push_back(boxRow(partRow("Time", f.time.empty() ? "—" : f.time + "s", kTimeRange[timeTier(r.time)], tp,
                                 kMaxTierScore, !f.time.empty() && qualifies),
                         kViolet));
    out.push_back(boxRow(partRow("Wrong", f.wrong.empty() ? "—" : f.wrong, kWrongRange[wrongTier(r.wrongTimes)], wp,
                                 kMaxTierScore, !f.wrong.empty() && qualifies),
                         kViolet));
    const char* dupRange = r.duplicateRate > kMaxDuplicateRate ? "> 50%" : kDupRange[duplicateTier(r.duplicateRate)];
    out.push_back(boxRow(partRow("Duplicate", f.dup.empty() ? "—" : f.dup + "%", dupRange, dp, kMaxTierScore,
                                 !f.dup.empty() && qualifies),
                         kViolet));
    out.push_back(boxRow(partRow("Passed", f.passed ? "yes" : "no", "", f.passed ? kPassScore : 0, kPassScore,
                                 qualifies),
                         kViolet));
    out.push_back(boxRule(kViolet));
    double max = 100.0 * weight;
    std::string verdict;
    bool complete = !f.time.empty() && !f.wrong.empty() && !f.dup.empty();
    if (!complete && r.passed && r.duplicateRate <= kMaxDuplicateRate) {
        verdict = paint(kMuted, "fill in all fields to see the score");
        out.push_back(boxRow(paint(kFaint, tui::repeat("─", 20)) + "  " + verdict, kViolet));
        out.push_back(boxBottom(kViolet));
        return out;
    }
    if (!r.passed) verdict = paint(kRed, "✗ not passed: this problem scores 0");
    else if (r.duplicateRate > kMaxDuplicateRate) verdict = paint(kRed, "✗ duplicate over 50%: this problem scores 0");
    else
        verdict = paint(kMuted, "problem ") + strong(kText, std::string(1, r.problem)) + paint(kMuted, " × " + fmt("%.2f", weight) + "  =  ") +
                  strong(scoreColor(score / max), fmt("%.2f", score)) + paint(kMuted, " / " + fmt("%.0f", max) + " pts");
    out.push_back(boxRow(tui::bar(score / max, 20, scoreColor(score / max), kFaint) + "  " + verdict, kViolet));
    out.push_back(boxBottom(kViolet));
    return out;
}

void inputScreen(RecordList& list, const ScoreConfig& config) {
    Form f;
    static const char* const kLabels[] = {"Student No", "Problem", "Time used", "Passed", "Wrong times", "Duplicate"};
    while (true) {
        std::vector<std::string> lines = {titleBar("Input records", list), ""};
        lines.push_back(boxTop("New submission", kCyan));
        for (int i = 0; i < kFieldCount; ++i) {
            bool on = f.field == i;
            std::string widget;
            switch (i) {
                case kStudent: {
                    widget = textField(f.student, on, "");
                    if (!f.student.empty() && f.student.size() != 12)
                        widget += paint(kYellow, "  " + std::to_string(f.student.size()) + "/12 digits");
                    break;
                }
                case kProblem: widget = choiceField({"A", "B", "C", "D", "E"}, f.problem, on); break;
                case kTime: widget = textField(f.time, on, f.time.empty() ? "" : " s"); break;
                case kPassed: widget = choiceField({"Yes", "No"}, f.passed ? 0 : 1, on); break;
                case kWrong: widget = textField(f.wrong, on, ""); break;
                case kDup: widget = textField(f.dup, on, f.dup.empty() ? "" : " %"); break;
            }
            std::string label = on ? strong(kText, tui::padRight(kLabels[i], 13)) : paint(kMuted, tui::padRight(kLabels[i], 13));
            lines.push_back(boxRow(label + widget, kCyan));
        }
        lines.push_back(boxBottom(kCyan));
        for (const std::string& l : preview(f, config)) lines.push_back(l);
        lines.push_back(f.flash.empty() ? "" : "  " + paint(f.flashGood ? kGreen : kRed, f.flash));
        lines.push_back(footer("↑↓ field · ←→ choose · enter next / save on last · esc back"));
        tui::present(lines);

        KeyEvent k = tui::readKey();
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
            // save
            int missing = -1;
            if (f.student.size() != 12 || std::atoll(f.student.c_str()) < kMinStudentNumber) missing = kStudent;
            else if (f.time.empty()) missing = kTime;
            else if (f.wrong.empty()) missing = kWrong;
            else if (f.dup.empty()) missing = kDup;
            if (missing >= 0) {
                f.field = missing;
                f.flashGood = false;
                f.flash = missing == kStudent ? "✗ Student number must be 12 digits" : std::string("✗ Please fill in ") + kLabels[missing];
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
            f.flash = "✓ Saved " + std::string(1, r.problem) + " for " + f.student + "  +" +
                      fmt("%.2f", r.result) + " pts · next problem ready";
            f.problem = (f.problem + 1) % kProblemCount;
            f.time.clear();
            f.wrong.clear();
            f.dup.clear();
            f.passed = true;
            f.field = kTime;
        }
        if (k.key != Key::Enter && k.key != Key::None && k.key != Key::Resize) f.flash.clear();
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
    while (true) {
        std::vector<const RecordNode*> rows;
        for (const RecordNode* p = list.head; p; p = p->next) rows.push_back(p);
        int visible = std::max(3, tui::height() - 9);
        int maxOffset = std::max(0, static_cast<int>(rows.size()) - visible);
        offset = std::min(offset, maxOffset);

        std::vector<std::string> lines = {titleBar("Records", list), ""};
        lines.push_back(boxTop("All submissions", kCyan));
        if (rows.empty()) {
            lines.push_back(boxRow("", kCyan));
            lines.push_back(boxRow(paint(kMuted, "No records yet. Choose ") + strong(kText, "Input records") +
                                       paint(kMuted, " from the menu."),
                                   kCyan));
            lines.push_back(boxRow("", kCyan));
        } else {
            lines.push_back(boxRow(paint(kMuted, "Student No     Prob Time    Pass  Wrong  Dup   Score"), kCyan));
            lines.push_back(boxRule(kCyan));
            for (int i = offset; i < std::min(static_cast<int>(rows.size()), offset + visible); ++i)
                lines.push_back(boxRow(recordRow(rows[i]->data, scoreRecord(rows[i]->data, config)), kCyan));
        }
        lines.push_back(boxBottom(kCyan));
        std::string range = rows.empty() ? "" : "rows " + std::to_string(offset + 1) + "–" +
                                                    std::to_string(std::min(static_cast<int>(rows.size()), offset + visible)) +
                                                    " of " + std::to_string(rows.size()) + " · ";
        lines.push_back(footer(range + (maxOffset > 0 ? "↑↓ scroll · " : "") + "esc back"));
        tui::present(lines);

        KeyEvent k = tui::readKey();
        if (k.key == Key::Up) offset = std::max(0, offset - 1);
        else if (k.key == Key::Down) offset = std::min(maxOffset, offset + 1);
        else if (k.key == Key::Esc || k.key == Key::Enter || (k.key == Key::Char && k.ch == 'q')) return;
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

    const int frames = 18;
    bool animate = true;
    for (int frame = 0; frame <= frames || !animate;) {
        double t = animate ? easeOut(static_cast<double>(frame) / frames) : 1.0;
        std::vector<std::string> lines = {titleBar("Report", list), ""};
        lines.push_back(boxTop("Report card · " + std::to_string(id), kViolet));
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
                                   kViolet));
        }
        lines.push_back(boxRule(kViolet));
        double tf = std::min(1.0, total / 100.0) * t;
        lines.push_back(boxRow(strong(kText, "Total  ") + tui::bar(tf, 36, scoreColor(tf), kFaint) + "  " +
                                   strong(scoreColor(tf), fmt("%6.2f", total * t)) + paint(kMuted, " / 100"),
                               kViolet));
        if (total > 100.0 + 1e-9)
            lines.push_back(boxRow(paint(kYellow, "! some problems were entered more than once"), kViolet));
        lines.push_back(boxRule(kViolet));
        if (eligible == 0) {
            lines.push_back(boxRow(paint(kMuted, "No passed problems yet, so there is no profile to show."), kViolet));
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
                                       kViolet));
            const Trait* best = &traits[0];
            const Trait* worst = &traits[0];
            for (const Trait& tr : traits) {
                if (tr.value > best->value) best = &tr;
                if (tr.value < worst->value) worst = &tr;
            }
            std::string summary = best->value - worst->value < 1e-9
                                      ? paint(kGreen, "Balanced across speed, accuracy and originality")
                                      : paint(kGreen, std::string("▲ Strength: ") + best->name) + "     " +
                                            paint(kYellow, std::string("▼ Work on: ") + worst->name);
            lines.push_back(boxRow("", kViolet));
            lines.push_back(boxRow(summary, kViolet));
        }
        lines.push_back(boxBottom(kViolet));
        lines.push_back(footer("esc back to the student list"));
        tui::present(lines);

        if (animate) {
            if (frame == frames) {
                animate = false;
                continue;
            }
            ++frame;
            if (tui::waitKey(28)) {
                tui::readKey();
                frame = frames;
            }
            continue;
        }
        KeyEvent k = tui::readKey();
        if (k.key == Key::Esc || k.key == Key::Enter || (k.key == Key::Char && k.ch == 'q')) return;
    }
}

void reportScreen(RecordList& list, const ScoreConfig& config) {
    int selected = 0;
    while (true) {
        Stats s = stats(list);
        std::vector<std::string> lines = {titleBar("Report", list), ""};
        lines.push_back(boxTop("Choose a student", kViolet));
        if (s.students.empty()) {
            lines.push_back(boxRow("", kViolet));
            lines.push_back(boxRow(paint(kMuted, "No records yet. Choose ") + strong(kText, "Input records") +
                                       paint(kMuted, " from the menu."),
                                   kViolet));
            lines.push_back(boxRow("", kViolet));
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
                lines.push_back(selectedRow(tui::fg(kPink) + "▶ " + tui::fg(kText) + text + tui::bar(f, 20, scoreColor(f), kFaint) +
                                                tui::bg(kSelect) + "  " + tui::fg(scoreColor(f)) + fmt("%6.2f", total),
                                            kViolet));
            else
                lines.push_back(boxRow("  " + paint(kText, text) + tui::bar(f, 20, scoreColor(f), kFaint) + "  " +
                                           paint(scoreColor(f), fmt("%6.2f", total)),
                                       kViolet));
        }
        lines.push_back(boxBottom(kViolet));
        lines.push_back(footer(s.students.empty() ? "esc back" : "↑↓ move · enter open report · esc back"));
        tui::present(lines);

        KeyEvent k = tui::readKey();
        int n = static_cast<int>(s.students.size());
        if (k.key == Key::Esc || (k.key == Key::Char && k.ch == 'q')) return;
        if (n == 0) {
            if (k.key == Key::Enter) return;
            continue;
        }
        if (k.key == Key::Up) selected = (selected + n - 1) % n;
        else if (k.key == Key::Down) selected = (selected + 1) % n;
        else if (k.key == Key::Enter) reportCard(list, config, s.students[selected]);
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
        std::vector<std::string> lines = {titleBar("Scoring rules", list), ""};
        lines.push_back(boxTop("Points per tier", kCyan));
        for (int i = 0; i < count; ++i) {
            const Field& f = fields[i];
            double frac = static_cast<double>(*f.points) / kMaxTierScore;
            bool newGroup = i > 0 && std::string(fields[i - 1].item) != f.item;
            if (newGroup) lines.push_back(boxRow("", kCyan));
            std::string name = (i == 0 || newGroup) ? f.item : "";
            if (i == selected) {
                std::string row = tui::fg(kPink) + "▶ " + tui::fg(kText) + tui::kBold + tui::padRight(name, 11) +
                                  "\033[22m" + tui::padRight(f.range, 11) + tui::fg(kCyan) + "◂ " + tui::fg(kText) +
                                  tui::kBold + fmt("%2.0f", *f.points) + "\033[22m" + tui::fg(kCyan) + " ▸  " +
                                  tui::bar(frac, 22, scoreColor(frac), kFaint) + tui::bg(kSelect);
                lines.push_back(selectedRow(row, kCyan));
            } else {
                lines.push_back(boxRow("  " + paint(kText, tui::padRight(name, 11)) + paint(kMuted, tui::padRight(f.range, 11)) +
                                           "  " + paint(kText, fmt("%2.0f", *f.points)) + "    " +
                                           tui::bar(frac, 22, scoreColor(frac), kFaint),
                                       kCyan));
            }
        }
        lines.push_back(boxRule(kCyan));
        int best = config.time[0] + config.wrong[0] + config.duplicate[0] + kPassScore;
        lines.push_back(boxRow(paint(kMuted, "Passed is fixed at 25 · not passed or dup > 50% scores 0"), kCyan));
        lines.push_back(boxRow(paint(kMuted, "Weights A–E 0.10 0.15 0.20 0.25 0.30 · best problem now ") +
                                   strong(best == 100 ? kGreen : kYellow, std::to_string(best) + "/100"),
                               kCyan));
        lines.push_back(boxBottom(kCyan));
        lines.push_back(footer("↑↓ choose · ←→ adjust · r reset defaults · esc back"));
        tui::present(lines);

        KeyEvent k = tui::readKey();
        int* v = fields[selected].points;
        if (k.key == Key::Up) selected = (selected + count - 1) % count;
        else if (k.key == Key::Down || k.key == Key::Tab) selected = (selected + 1) % count;
        else if (k.key == Key::Left || (k.key == Key::Char && (k.ch == '-' || k.ch == 'h'))) *v = std::max(0, *v - 1);
        else if (k.key == Key::Right || (k.key == Key::Char && (k.ch == '+' || k.ch == '=' || k.ch == 'l')))
            *v = std::min(kMaxTierScore, *v + 1);
        else if (k.key == Key::Char && k.ch == 'r') config = defaultScoreConfig();
        else if (k.key == Key::Esc || k.key == Key::Enter || (k.key == Key::Char && k.ch == 'q')) return;
    }
}

// ---------------------------------------------------------------- goodbye --

void goodbye() {
    std::vector<std::string> lines = {"", ""};
    for (const std::string& l : logo()) lines.push_back("  " + l);
    lines.push_back("");
    lines.push_back("  " + tui::gradient("See you next time ✦", kViolet, kPink));
    tui::present(lines);
    tui::sleepMs(450);
}

}  // namespace

void runTui(RecordList& list, ScoreConfig& config) {
    tui::begin();
    intro();
    if (!loginScreen()) {
        tui::end();
        return;
    }
    int selected = 0;
    while (true) {
        switch (mainMenu(list, selected)) {
            case 0: inputScreen(list, config); break;
            case 1: recordsScreen(list, config); break;
            case 2: reportScreen(list, config); break;
            case 3: rulesScreen(list, config); break;
            default:
                goodbye();
                tui::end();
                std::cout << tui::gradient("Thanks for using StudentPTA ✦", kCyan, kPink) << "\n";
                return;
        }
    }
}
