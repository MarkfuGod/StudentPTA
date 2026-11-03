#ifndef STUDENTPTA_MENU_H
#define STUDENTPTA_MENU_H

#include "record_list.h"
#include "scoring.h"

// The interactive screens behind each main-menu entry.
void login();
void inputRecords(RecordList& list);
void showRecords(const RecordList& list);
void calculateResults(RecordList& list, const ScoreConfig& config);
void editScores(ScoreConfig& config);

#endif
