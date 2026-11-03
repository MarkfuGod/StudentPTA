#include "banner.h"
#include "console.h"
#include "menu.h"
#include "record_list.h"
#include "scoring.h"

int main() {
    printZZU();
    login();
    printWelcome();

    RecordList list;
    initList(list);
    ScoreConfig config = defaultScoreConfig();

    bool running = true;
    while (running) {
        printMainMenu();
        switch (readInt("\033[01;34m Please Input Your Choice:\033[0m ", 1, 5)) {
            case 1:
                editScores(config);
                break;
            case 2:
                clearScreen();
                showRecords(list);
                break;
            case 3:
                calculateResults(list, config);
                clearScreen();
                break;
            case 4:
                inputRecords(list);
                break;
            case 5:
                running = false;
                break;
        }
    }

    clearScreen();
    printGoodBye();
    printPTA();
    freeList(list);
    return 0;
}
