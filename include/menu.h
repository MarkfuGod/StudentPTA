#ifndef STUDENTPTA_MENU_H
#define STUDENTPTA_MENU_H

#include "record_list.h"
#include "scoring.h"

const char* const kAccountName = "MarkfuGod";
const long long kMinStudentNumber = 100000000000LL;  // 12-digit student numbers
const long long kMaxStudentNumber = 999999999999LL;

// Line-by-line mode, used when not running in a terminal (pipes, scripts).
void runPlain(RecordList& list, ScoreConfig& config);

// The line-mode screens behind each main-menu entry.
void login();
void inputRecords(RecordList& list);
void showRecords(const RecordList& list);
void calculateResults(RecordList& list, const ScoreConfig& config);
void editScores(ScoreConfig& config);

#endif
