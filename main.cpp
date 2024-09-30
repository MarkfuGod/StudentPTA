#include "Definition.h"
#include "FunctionsAnnounce.h"
#include "FunctionDefinitions.h"
#include "FunctionMain.h"

using namespace::std;

int main(void){
    //Standard.buttomTime = 1;
    //cout << Standard.buttomTime << endl;
    //modifyValue();
    //PrintStart();
    PrintZZU();
    InitialWeights();
    // cout << standardWeights.mediumTime << endl;
    // cout << standardWeights.pass0 << endl;
    // cout << standardWeights.mediumWrongTime << endl;
    string userName;
    while (userName != "MarkfuGod") {
        cout << "\033[01;04;32m Please Input the Account Name:\033[0m ";
        cin >> userName;
        continue;
    }
    cout << "SUCCESS !" << endl;
    PrintWelcome();
    studentList L;
    InitList(L);
    while (1) {
        PrintMainMenu();
        cout << "\e[01;34m Please Input Your Choice:\e[0m " ;
        string x;
        cin >> x;
        if (x == "1") {
            SetWeights();
            continue;
        }
        else if (x == "2") {
            system("clear");
            showListElement(L);
            continue;
        }
        else if (x == "3") {
            calculateResults(L);
            system("clear");
        }
        else if (x == "4") {
            CreateFromTail(L);
            // showListElement(L);
        }
        else if(x == "5") {
            break;
        }
        else {
            system("clear");
            printf("\e[01;33;41;5m Please Type in the Right Choice:\033[0m\n");
            cout << endl;
            cout << endl;
            cout << endl;
            cout << endl;
            cout << endl;
            continue;
        }
    }
    system("clear");
    PrintGoodBye();
    PrintPTA();
    system("Pause");
    // showListElement(L);
    // studentNode *p = Locate(L, 202104000707);
    // showSpecifiedListElement(p, 202104000707);
    // cout << Standard.buttomDuplicateRate << endl;
    return 0;
    
}

