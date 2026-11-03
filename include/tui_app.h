#ifndef STUDENTPTA_TUI_APP_H
#define STUDENTPTA_TUI_APP_H

#include "record_list.h"
#include "scoring.h"

// Full-screen interactive mode: arrow-key menus, live score preview while
// typing, animated report cards. Used when running in a real terminal.
void runTui(RecordList& list, ScoreConfig& config);

#endif
