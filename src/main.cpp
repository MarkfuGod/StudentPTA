#include "menu.h"
#include "record_list.h"
#include "scoring.h"
#include "tui.h"
#include "tui_app.h"

int main() {
    RecordList list;
    initList(list);
    ScoreConfig config = defaultScoreConfig();

    // Full-screen mode in a real terminal; line mode for pipes and scripts
    // (or when STUDENTPTA_PLAIN=1 is set).
    if (tui::available()) {
        runTui(list, config);
    } else {
        runPlain(list, config);
    }

    freeList(list);
    return 0;
}
