#ifndef STUDENTPTA_FX_H
#define STUDENTPTA_FX_H

#include <string>
#include <vector>

#include "tui.h"

// Visual effects layer for the full-screen mode: neon palette, digital rain,
// particles, glitches, 3D card-flip transitions and a little walking robot.
// The UI hands over its lines with show(); everything else animates around it.
namespace fx {

// Neon palette
const tui::Rgb kPink = {255, 46, 151};
const tui::Rgb kCyan = {0, 236, 255};
const tui::Rgb kYellow = {255, 230, 30};
const tui::Rgb kPurple = {160, 90, 255};
const tui::Rgb kGreen = {40, 255, 160};
const tui::Rgb kRed = {255, 60, 80};
const tui::Rgb kInk = {232, 230, 250};
const tui::Rgb kMuted = {140, 128, 185};
const tui::Rgb kFaint = {58, 50, 92};
const tui::Rgb kBase = {9, 8, 20};      // screen background
const tui::Rgb kPanel = {17, 14, 36};   // inside boxes
const tui::Rgb kSelect = {74, 22, 98};  // highlighted row

double t();                 // seconds since start
double pulse(double hz);    // 0..1 sine wave, in 12 steps

// Draw one frame: background effects, robot, then the UI lines centred on top.
void show(const std::vector<std::string>& ui);

enum class Transition { None, FlipForward, FlipBack };
void transition(Transition t);  // played by the next show()

// Particle burst at a position inside the last UI block (row, column).
void burstAt(int uiRow, int uiCol, int count, double power = 1.0);
// Robot reactions; the text appears in a speech bubble.
void celebrate(const std::string& bubble);
void oops(const std::string& bubble);
void say(const std::string& bubble, double seconds = 2.6);
void glitch(double seconds, double strength);

}  // namespace fx

#endif
