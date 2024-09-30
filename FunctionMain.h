#ifndef FunctionMain_h
#define FunctionMain_h
#include "FunctionDefinitions.h"
using namespace std;

void calculateResults(studentNode *L) {
    while(1)
    {
    long long studentNumber;
    cout << "\033[01;32mPlease Input the student Number you want to calculate: \e[01m(Press \e[01;33m0\e[0m to quit)" << endl;
    cin >> studentNumber;
    if (studentNumber == 0) {
        return;
    }
    studentNode *p =Locate(L, studentNumber);
    studentNode *s = p;
    showSpecifiedListElement(p, studentNumber);
    int i = 0;
    if( p == NULL ) {
        cout << "\e[01;33;41m No such Student Found, Please Type in the Right Number:\e[0m" << endl;
        continue;
    }
    while(p != NULL){
        if(p -> visited == 1) i ++;
            p = p -> next;
    }
   // if (i < 5) {
   //     cout << " Please Input the full Sets of the Problems to get a total Evaluation!" << endl;
   //     cout << " Press Q to Go Back to Main Menu" << endl;
   //     char c;
   //     cin >> c;
   //     if(c == 'Q') return;
   // }
        
        while(s != NULL) {
            if(s -> visited == 1 && s -> studentNumber == studentNumber) {
            switch (s -> problemNumber) {
                case 'A':
                    
                    //cout << "Yes" << endl;
                    if(passJudge(s -> pass) == 0 || duplicateJudge(s ->duplicateRate) == 9)
                        Result.A = 0.00;
                    else if(passJudge(s -> pass) == 1 && duplicateJudge(s -> duplicateRate != 9)) {
                        switch (timeJudge(s -> time)) {
                            case 2:
                                if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 13)
                                    Result.A = 1.00*(standardWeights.buttomTime + standardWeights.buttomWrongTime + standardWeights.buttomDuplicateRate + Base) * AWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 14)
                                    Result.A = 1.00*(standardWeights.buttomTime + standardWeights.buttomWrongTime + standardWeights.mediumDuplicateRate + Base) * AWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 15)
                                    Result.A = 1.00*(standardWeights.buttomTime + standardWeights.buttomWrongTime + standardWeights.topDuplicateRate + Base) * AWeight;
                                /* <-------------------------------------------------------->
                                 */
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 17)
                                    Result.A = 1.00*(standardWeights.buttomTime + standardWeights.mediumWrongTime + standardWeights.buttomDuplicateRate + Base) * AWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 18)
                                    Result.A = 1.00*(standardWeights.buttomTime + standardWeights.mediumWrongTime + standardWeights.mediumDuplicateRate + Base) * AWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 19)
                                    Result.A = 1.00*(standardWeights.buttomTime + standardWeights.mediumWrongTime + standardWeights.topDuplicateRate + Base) * AWeight;
                                /* <-------------------------------------------------------->
                                 */
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 21)
                                    Result.A = 1.00*(standardWeights.buttomTime + standardWeights.topWrongTime + standardWeights.buttomDuplicateRate + Base) * AWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 22)
                                    Result.A = 1.00*(standardWeights.buttomTime + standardWeights.topWrongTime + standardWeights.mediumDuplicateRate + Base) * AWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 23)
                                    Result.A = 1.00*(standardWeights.buttomTime + standardWeights.topWrongTime + standardWeights.topDuplicateRate + Base) * AWeight;
                                /* <-------------------------------------------------------->
                                 */
                                break;
                            case 4:
                                if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 13)
                                    Result.A = 1.00*(standardWeights.higherTime + standardWeights.buttomWrongTime + standardWeights.buttomDuplicateRate + Base) * AWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 14)
                                    Result.A = 1.00*(standardWeights.higherTime + standardWeights.buttomWrongTime + standardWeights.mediumDuplicateRate + Base) * AWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 15)
                                    Result.A = 1.00*(standardWeights.higherTime + standardWeights.buttomWrongTime + standardWeights.topDuplicateRate + Base) * AWeight;
                                /* <-------------------------------------------------------->
                                 */
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 17)
                                    Result.A = 1.00*(standardWeights.higherTime + standardWeights.mediumWrongTime + standardWeights.buttomDuplicateRate + Base) * AWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 18)
                                    Result.A = 1.00*(standardWeights.higherTime + standardWeights.mediumWrongTime + standardWeights.mediumDuplicateRate + Base) * AWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 19)
                                    Result.A = 1.00*(standardWeights.higherTime + standardWeights.mediumWrongTime + standardWeights.topDuplicateRate + Base) * AWeight;
                                /* <-------------------------------------------------------->
                                 */
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 21)
                                    Result.A = 1.00*(standardWeights.higherTime + standardWeights.topWrongTime + standardWeights.buttomDuplicateRate + Base) * AWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 22)
                                    Result.A = 1.00*(standardWeights.higherTime + standardWeights.topWrongTime + standardWeights.mediumDuplicateRate + Base) * AWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 23)
                                    Result.A = 1.00*(standardWeights.higherTime + standardWeights.topWrongTime + standardWeights.topDuplicateRate + Base) * AWeight;
                                /* <-------------------------------------------------------->
                                 */
                                break;
                            case 3:
                                if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 13)
                                    Result.A = 1.00*(standardWeights.mediumTime + standardWeights.buttomWrongTime + standardWeights.buttomDuplicateRate + Base) * AWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 14)
                                    Result.A = 1.00*(standardWeights.mediumTime + standardWeights.buttomWrongTime + standardWeights.mediumDuplicateRate + Base) * AWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 15)
                                    Result.A = 1.00*(standardWeights.mediumTime + standardWeights.buttomWrongTime + standardWeights.topDuplicateRate + Base) * AWeight;
                                /* <-------------------------------------------------------->
                                 */
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 17)
                                    Result.A = 1.00*(standardWeights.mediumTime + standardWeights.mediumWrongTime + standardWeights.buttomDuplicateRate + Base) * AWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 18)
                                    Result.A = 1.00*(standardWeights.mediumTime + standardWeights.mediumWrongTime + standardWeights.mediumDuplicateRate + Base) * AWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 19)
                                    Result.A = 1.00*(standardWeights.mediumTime + standardWeights.mediumWrongTime + standardWeights.topDuplicateRate + Base) * AWeight;
                                /* <-------------------------------------------------------->
                                 */
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 21)
                                    Result.A = 1.00*(standardWeights.mediumTime + standardWeights.topWrongTime + standardWeights.buttomDuplicateRate + Base) * AWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 22)
                                    Result.A = 1.00*(standardWeights.mediumTime + standardWeights.topWrongTime + standardWeights.mediumDuplicateRate + Base) * AWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 23)
                                    Result.A = 1.00*(standardWeights.mediumTime + standardWeights.topWrongTime + standardWeights.topDuplicateRate + Base) * AWeight;
                                /* <-------------------------------------------------------->
                                 */
                                break;
                            case 5:
                                if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 13)
                                    Result.A = 1.00*(standardWeights.topTime + standardWeights.buttomWrongTime + standardWeights.buttomDuplicateRate + Base) * AWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 14)
                                    Result.A = 1.00*(standardWeights.topTime + standardWeights.buttomWrongTime + standardWeights.mediumDuplicateRate + Base) * AWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 15)
                                    Result.A = 1.00*(standardWeights.topTime + standardWeights.buttomWrongTime + standardWeights.topDuplicateRate + Base) * AWeight;
                                /* <-------------------------------------------------------->
                                 */
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 17)
                                    Result.A = 1.00*(standardWeights.topTime + standardWeights.mediumWrongTime + standardWeights.buttomDuplicateRate + Base) * AWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 18)
                                    Result.A = 1.00*(standardWeights.topTime + standardWeights.mediumWrongTime + standardWeights.mediumDuplicateRate + Base) * AWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 19)
                                    Result.A = 1.00*(standardWeights.topTime + standardWeights.mediumWrongTime + standardWeights.topDuplicateRate + Base) * AWeight;
                                /* <-------------------------------------------------------->
                                 */
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 21)
                                    Result.A = 1.00*(standardWeights.topTime + standardWeights.topWrongTime + standardWeights.buttomDuplicateRate + Base) * AWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 22)
                                    Result.A = 1.00*(standardWeights.topTime + standardWeights.topWrongTime + standardWeights.mediumDuplicateRate + Base) * AWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 23)
                                    Result.A = 1.00*(standardWeights.topTime + standardWeights.topWrongTime + standardWeights.topDuplicateRate + Base) * AWeight;
                                /* <-------------------------------------------------------->
                                 */
                                break;
                            default:
                                break;
                        }
                        s -> results = Result.A;
                    }
                    break;
                case 'B':
                    
                    if(passJudge(s -> pass) == 0 || duplicateJudge(s ->duplicateRate) == 9)
                        Result.B = 0.00;
                    else if(passJudge(s -> pass) == 1 && duplicateJudge(s -> duplicateRate != 9)) {
                        switch (timeJudge(s -> time)) {
                            case 2:
                                if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 13)
                                    Result.B = 1.00*(standardWeights.buttomTime + standardWeights.buttomWrongTime + standardWeights.buttomDuplicateRate + Base) * BWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 14)
                                    Result.B = 1.00*(standardWeights.buttomTime + standardWeights.buttomWrongTime + standardWeights.mediumDuplicateRate + Base) * BWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 15)
                                    Result.B = 1.00*(standardWeights.buttomTime + standardWeights.buttomWrongTime + standardWeights.topDuplicateRate + Base) * BWeight;
                                /* <-------------------------------------------------------->
                                 */
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 17)
                                    Result.B = 1.00*(standardWeights.buttomTime + standardWeights.mediumWrongTime + standardWeights.buttomDuplicateRate + Base) * BWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 18)
                                    Result.B = 1.00*(standardWeights.buttomTime + standardWeights.mediumWrongTime + standardWeights.mediumDuplicateRate + Base) * BWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 19)
                                    Result.B = 1.00*(standardWeights.buttomTime + standardWeights.mediumWrongTime + standardWeights.topDuplicateRate + Base) * BWeight;
                                /* <-------------------------------------------------------->
                                 */
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 21)
                                    Result.B = 1.00*(standardWeights.buttomTime + standardWeights.topWrongTime + standardWeights.buttomDuplicateRate + Base) * BWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 22)
                                    Result.B = 1.00*(standardWeights.buttomTime + standardWeights.topWrongTime + standardWeights.mediumDuplicateRate + Base) * BWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 23)
                                    Result.B = 1.00*(standardWeights.buttomTime + standardWeights.topWrongTime + standardWeights.topDuplicateRate + Base) * BWeight;
                                /* <-------------------------------------------------------->
                                 */
                                break;
                            case 4:
                                if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 13)
                                    Result.B = 1.00*(standardWeights.higherTime + standardWeights.buttomWrongTime + standardWeights.buttomDuplicateRate + Base) * BWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 14)
                                    Result.B = 1.00*(standardWeights.higherTime + standardWeights.buttomWrongTime + standardWeights.mediumDuplicateRate + Base) * BWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 15)
                                    Result.B = 1.00*(standardWeights.higherTime + standardWeights.buttomWrongTime + standardWeights.topDuplicateRate + Base) * BWeight;
                                /* <-------------------------------------------------------->
                                 */
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 17)
                                    Result.B = 1.00*(standardWeights.higherTime + standardWeights.mediumWrongTime + standardWeights.buttomDuplicateRate + Base) * BWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 18)
                                    Result.B = 1.00*(standardWeights.higherTime + standardWeights.mediumWrongTime + standardWeights.mediumDuplicateRate + Base) * BWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 19)
                                    Result.B = 1.00*(standardWeights.higherTime + standardWeights.mediumWrongTime + standardWeights.topDuplicateRate + Base) * BWeight;
                                /* <-------------------------------------------------------->
                                 */
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 21)
                                    Result.B = 1.00*(standardWeights.higherTime + standardWeights.topWrongTime + standardWeights.buttomDuplicateRate + Base) * BWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 22)
                                    Result.B = 1.00*(standardWeights.higherTime + standardWeights.topWrongTime + standardWeights.mediumDuplicateRate + Base) * BWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 23)
                                    Result.B = 1.00*(standardWeights.higherTime + standardWeights.topWrongTime + standardWeights.topDuplicateRate + Base) * BWeight;
                                /* <-------------------------------------------------------->
                                 */
                                break;
                            case 3:
                                if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 13)
                                    Result.B = 1.00*(standardWeights.mediumTime + standardWeights.buttomWrongTime + standardWeights.buttomDuplicateRate + Base) * BWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 14)
                                    Result.B = 1.00*(standardWeights.mediumTime + standardWeights.buttomWrongTime + standardWeights.mediumDuplicateRate + Base) * BWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 15)
                                    Result.B = 1.00*(standardWeights.mediumTime + standardWeights.buttomWrongTime + standardWeights.topDuplicateRate + Base) * BWeight;
                                /* <-------------------------------------------------------->
                                 */
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 17)
                                    Result.B = 1.00*(standardWeights.mediumTime + standardWeights.mediumWrongTime + standardWeights.buttomDuplicateRate + Base) * BWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 18)
                                    Result.B = 1.00*(standardWeights.mediumTime + standardWeights.mediumWrongTime + standardWeights.mediumDuplicateRate + Base) * BWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 19)
                                    Result.B = 1.00*(standardWeights.mediumTime + standardWeights.mediumWrongTime + standardWeights.topDuplicateRate + Base) * BWeight;
                                /* <-------------------------------------------------------->
                                 */
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 21)
                                    Result.B = 1.00*(standardWeights.mediumTime + standardWeights.topWrongTime + standardWeights.buttomDuplicateRate + Base) * BWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 22)
                                    Result.B = 1.00*(standardWeights.mediumTime + standardWeights.topWrongTime + standardWeights.mediumDuplicateRate + Base) * BWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 23)
                                    Result.B = 1.00*(standardWeights.mediumTime + standardWeights.topWrongTime + standardWeights.topDuplicateRate + Base) * BWeight;
                                /* <-------------------------------------------------------->
                                 */
                                break;
                            case 5:
                                if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 13)
                                    Result.B = 1.00*(standardWeights.topTime + standardWeights.buttomWrongTime + standardWeights.buttomDuplicateRate + Base) * BWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 14)
                                    Result.B = 1.00*(standardWeights.topTime + standardWeights.buttomWrongTime + standardWeights.mediumDuplicateRate + Base) * BWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 15)
                                    Result.B = 1.00*(standardWeights.topTime + standardWeights.buttomWrongTime + standardWeights.topDuplicateRate + Base) * BWeight;
                                /* <-------------------------------------------------------->
                                 */
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 17)
                                    Result.B = 1.00*(standardWeights.topTime + standardWeights.mediumWrongTime + standardWeights.buttomDuplicateRate + Base) * BWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 18)
                                    Result.B = 1.00*(standardWeights.topTime + standardWeights.mediumWrongTime + standardWeights.mediumDuplicateRate + Base) * BWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 19)
                                    Result.B = 1.00*(standardWeights.topTime + standardWeights.mediumWrongTime + standardWeights.topDuplicateRate + Base) * BWeight;
                                /* <-------------------------------------------------------->
                                 */
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 21)
                                    Result.B = 1.00*(standardWeights.topTime + standardWeights.topWrongTime + standardWeights.buttomDuplicateRate + Base) * BWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 22)
                                    Result.B = 1.00*(standardWeights.topTime + standardWeights.topWrongTime + standardWeights.mediumDuplicateRate + Base) * BWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 23)
                                    Result.B = 1.00*(standardWeights.topTime + standardWeights.topWrongTime + standardWeights.topDuplicateRate + Base) * BWeight;
                                /* <-------------------------------------------------------->
                                 */
                                break;
                            default:
                                break;
                        }
                        s -> results = Result.B;
                    }
                    break;
                case 'C':
                    if(passJudge(s -> pass) == 0 || duplicateJudge(s ->duplicateRate) == 9)
                        Result.C = 0.00;
                    else if(passJudge(s -> pass) == 1 && duplicateJudge(s -> duplicateRate != 9)) {
                        switch (timeJudge(s -> time)) {
                            case 2:
                                if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 13)
                                    Result.C = 1.00*(standardWeights.buttomTime + standardWeights.buttomWrongTime + standardWeights.buttomDuplicateRate + Base) * CWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 14)
                                    Result.C = 1.00*(standardWeights.buttomTime + standardWeights.buttomWrongTime + standardWeights.mediumDuplicateRate + Base) * CWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 15)
                                    Result.C = 1.00*(standardWeights.buttomTime + standardWeights.buttomWrongTime + standardWeights.topDuplicateRate + Base) * CWeight;
                                /* <-------------------------------------------------------->
                                 */
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 17)
                                    Result.C = 1.00*(standardWeights.buttomTime + standardWeights.mediumWrongTime + standardWeights.buttomDuplicateRate + Base) * CWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 18)
                                    Result.C = 1.00*(standardWeights.buttomTime + standardWeights.mediumWrongTime + standardWeights.mediumDuplicateRate + Base) * CWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 19)
                                    Result.C = 1.00*(standardWeights.buttomTime + standardWeights.mediumWrongTime + standardWeights.topDuplicateRate + Base) * CWeight;
                                /* <-------------------------------------------------------->
                                 */
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 21)
                                    Result.C = 1.00*(standardWeights.buttomTime + standardWeights.topWrongTime + standardWeights.buttomDuplicateRate + Base) * CWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 22)
                                    Result.C = 1.00*(standardWeights.buttomTime + standardWeights.topWrongTime + standardWeights.mediumDuplicateRate + Base) * CWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 23)
                                    Result.C = 1.00*(standardWeights.buttomTime + standardWeights.topWrongTime + standardWeights.topDuplicateRate + Base) * CWeight;
                                /* <-------------------------------------------------------->
                                 */
                                break;
                            case 4:
                                if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 13)
                                    Result.C = 1.00*(standardWeights.higherTime + standardWeights.buttomWrongTime + standardWeights.buttomDuplicateRate + Base) * CWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 14)
                                    Result.C = 1.00*(standardWeights.higherTime + standardWeights.buttomWrongTime + standardWeights.mediumDuplicateRate + Base) * CWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 15)
                                    Result.C = 1.00*(standardWeights.higherTime + standardWeights.buttomWrongTime + standardWeights.topDuplicateRate + Base) * CWeight;
                                /* <-------------------------------------------------------->
                                 */
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 17)
                                    Result.C = 1.00*(standardWeights.higherTime + standardWeights.mediumWrongTime + standardWeights.buttomDuplicateRate + Base) * CWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 18)
                                    Result.C = 1.00*(standardWeights.higherTime + standardWeights.mediumWrongTime + standardWeights.mediumDuplicateRate + Base) * CWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 19)
                                    Result.C = 1.00*(standardWeights.higherTime + standardWeights.mediumWrongTime + standardWeights.topDuplicateRate + Base) * CWeight;
                                /* <-------------------------------------------------------->
                                 */
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 21)
                                    Result.C = 1.00*(standardWeights.higherTime + standardWeights.topWrongTime + standardWeights.buttomDuplicateRate + Base) * CWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 22)
                                    Result.C = 1.00*(standardWeights.higherTime + standardWeights.topWrongTime + standardWeights.mediumDuplicateRate + Base) * CWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 23)
                                    Result.C = 1.00*(standardWeights.higherTime + standardWeights.topWrongTime + standardWeights.topDuplicateRate + Base) * CWeight;
                                /* <-------------------------------------------------------->
                                 */
                                break;
                            case 3:
                                if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 13)
                                    Result.C = 1.00*(standardWeights.mediumTime + standardWeights.buttomWrongTime + standardWeights.buttomDuplicateRate + Base) * CWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 14)
                                    Result.C = 1.00*(standardWeights.mediumTime + standardWeights.buttomWrongTime + standardWeights.mediumDuplicateRate + Base) * CWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 15)
                                    Result.C = 1.00*(standardWeights.mediumTime + standardWeights.buttomWrongTime + standardWeights.topDuplicateRate + Base) * CWeight;
                                /* <-------------------------------------------------------->
                                 */
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 17)
                                    Result.C = 1.00*(standardWeights.mediumTime + standardWeights.mediumWrongTime + standardWeights.buttomDuplicateRate + Base) * CWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 18)
                                    Result.C = 1.00*(standardWeights.mediumTime + standardWeights.mediumWrongTime + standardWeights.mediumDuplicateRate + Base) * CWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 19)
                                    Result.C = 1.00*(standardWeights.mediumTime + standardWeights.mediumWrongTime + standardWeights.topDuplicateRate + Base) * CWeight;
                                /* <-------------------------------------------------------->
                                 */
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 21)
                                    Result.C = 1.00*(standardWeights.mediumTime + standardWeights.topWrongTime + standardWeights.buttomDuplicateRate + Base) * CWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 22)
                                    Result.C = 1.00*(standardWeights.mediumTime + standardWeights.topWrongTime + standardWeights.mediumDuplicateRate + Base) * CWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 23)
                                    Result.C = 1.00*(standardWeights.mediumTime + standardWeights.topWrongTime + standardWeights.topDuplicateRate + Base) * CWeight;
                                /* <-------------------------------------------------------->
                                 */
                                break;
                            case 5:
                                if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 13)
                                    Result.C = 1.00*(standardWeights.topTime + standardWeights.buttomWrongTime + standardWeights.buttomDuplicateRate + Base) * CWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 14)
                                    Result.C = 1.00*(standardWeights.topTime + standardWeights.buttomWrongTime + standardWeights.mediumDuplicateRate + Base) * CWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 15)
                                    Result.C = 1.00*(standardWeights.topTime + standardWeights.buttomWrongTime + standardWeights.topDuplicateRate + Base) * CWeight;
                                /* <-------------------------------------------------------->
                                 */
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 17)
                                    Result.C = 1.00*(standardWeights.topTime + standardWeights.mediumWrongTime + standardWeights.buttomDuplicateRate + Base) * CWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 18)
                                    Result.C = 1.00*(standardWeights.topTime + standardWeights.mediumWrongTime + standardWeights.mediumDuplicateRate + Base) * CWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 19)
                                    Result.C = 1.00*(standardWeights.topTime + standardWeights.mediumWrongTime + standardWeights.topDuplicateRate + Base) * CWeight;
                                /* <-------------------------------------------------------->
                                 */
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 21)
                                    Result.C = 1.00*(standardWeights.topTime + standardWeights.topWrongTime + standardWeights.buttomDuplicateRate + Base) * CWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 22)
                                    Result.C = 1.00*(standardWeights.topTime + standardWeights.topWrongTime + standardWeights.mediumDuplicateRate + Base) * CWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 23)
                                    Result.C = 1.00*(standardWeights.topTime + standardWeights.topWrongTime + standardWeights.topDuplicateRate + Base) * CWeight;
                                /* <-------------------------------------------------------->
                                 */
                                break;
                            default:
                                break;
                        }
                        s -> results = Result.C;
                    }
                    //cout << "No" << endl;
                    break;
                case 'D':
                    if(passJudge(s -> pass) == 0 || duplicateJudge(s ->duplicateRate) == 9)
                        Result.D = 0.00;
                    else if(passJudge(s -> pass) == 1 && duplicateJudge(s -> duplicateRate != 9)) {
                        switch (timeJudge(s -> time)) {
                            case 2:
                                if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 13)
                                    Result.D = 1.00*(standardWeights.buttomTime + standardWeights.buttomWrongTime + standardWeights.buttomDuplicateRate + Base) * DWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 14)
                                    Result.D = 1.00*(standardWeights.buttomTime + standardWeights.buttomWrongTime + standardWeights.mediumDuplicateRate + Base) * DWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 15)
                                    Result.D = 1.00*(standardWeights.buttomTime + standardWeights.buttomWrongTime + standardWeights.topDuplicateRate + Base) * DWeight;
                                /* <-------------------------------------------------------->
                                 */
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 17)
                                    Result.D = 1.00*(standardWeights.buttomTime + standardWeights.mediumWrongTime + standardWeights.buttomDuplicateRate + Base) * DWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 18)
                                    Result.D = 1.00*(standardWeights.buttomTime + standardWeights.mediumWrongTime + standardWeights.mediumDuplicateRate + Base) * DWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 19)
                                    Result.D = 1.00*(standardWeights.buttomTime + standardWeights.mediumWrongTime + standardWeights.topDuplicateRate + Base) * DWeight;
                                /* <-------------------------------------------------------->
                                 */
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 21)
                                    Result.D = 1.00*(standardWeights.buttomTime + standardWeights.topWrongTime + standardWeights.buttomDuplicateRate + Base) * DWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 22)
                                    Result.D = 1.00*(standardWeights.buttomTime + standardWeights.topWrongTime + standardWeights.mediumDuplicateRate + Base) * DWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 23)
                                    Result.D = 1.00*(standardWeights.buttomTime + standardWeights.topWrongTime + standardWeights.topDuplicateRate + Base) * DWeight;
                                /* <-------------------------------------------------------->
                                 */
                                break;
                            case 4:
                                if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 13)
                                    Result.D = 1.00*(standardWeights.higherTime + standardWeights.buttomWrongTime + standardWeights.buttomDuplicateRate + Base) * DWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 14)
                                    Result.D = 1.00*(standardWeights.higherTime + standardWeights.buttomWrongTime + standardWeights.mediumDuplicateRate + Base) * DWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 15)
                                    Result.D = 1.00*(standardWeights.higherTime + standardWeights.buttomWrongTime + standardWeights.topDuplicateRate + Base) * DWeight;
                                /* <-------------------------------------------------------->
                                 */
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 17)
                                    Result.D = 1.00*(standardWeights.higherTime + standardWeights.mediumWrongTime + standardWeights.buttomDuplicateRate + Base) * DWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 18)
                                    Result.D = 1.00*(standardWeights.higherTime + standardWeights.mediumWrongTime + standardWeights.mediumDuplicateRate + Base) * DWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 19)
                                    Result.D = 1.00*(standardWeights.higherTime + standardWeights.mediumWrongTime + standardWeights.topDuplicateRate + Base) * DWeight;
                                /* <-------------------------------------------------------->
                                 */
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 21)
                                    Result.D = 1.00*(standardWeights.higherTime + standardWeights.topWrongTime + standardWeights.buttomDuplicateRate + Base) * DWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 22)
                                    Result.D = 1.00*(standardWeights.higherTime + standardWeights.topWrongTime + standardWeights.mediumDuplicateRate + Base) * DWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 23)
                                    Result.D = 1.00*(standardWeights.higherTime + standardWeights.topWrongTime + standardWeights.topDuplicateRate + Base) * DWeight;
                                /* <-------------------------------------------------------->
                                 */
                                break;
                            case 3:
                                if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 13)
                                    Result.D = 1.00*(standardWeights.mediumTime + standardWeights.buttomWrongTime + standardWeights.buttomDuplicateRate + Base) * DWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 14)
                                    Result.D = 1.00*(standardWeights.mediumTime + standardWeights.buttomWrongTime + standardWeights.mediumDuplicateRate + Base) * DWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 15)
                                    Result.D = 1.00*(standardWeights.mediumTime + standardWeights.buttomWrongTime + standardWeights.topDuplicateRate + Base) * DWeight;
                                /* <-------------------------------------------------------->
                                 */
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 17)
                                    Result.D = 1.00*(standardWeights.mediumTime + standardWeights.mediumWrongTime + standardWeights.buttomDuplicateRate + Base) * DWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 18)
                                    Result.D = 1.00*(standardWeights.mediumTime + standardWeights.mediumWrongTime + standardWeights.mediumDuplicateRate + Base) * DWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 19)
                                    Result.D = 1.00*(standardWeights.mediumTime + standardWeights.mediumWrongTime + standardWeights.topDuplicateRate + Base) * DWeight;
                                /* <-------------------------------------------------------->
                                 */
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 21)
                                    Result.D = 1.00*(standardWeights.mediumTime + standardWeights.topWrongTime + standardWeights.buttomDuplicateRate + Base) * DWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 22)
                                    Result.D = 1.00*(standardWeights.mediumTime + standardWeights.topWrongTime + standardWeights.mediumDuplicateRate + Base) * DWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 23)
                                    Result.D = 1.00*(standardWeights.mediumTime + standardWeights.topWrongTime + standardWeights.topDuplicateRate + Base) * DWeight;
                                /* <-------------------------------------------------------->
                                 */
                                break;
                            case 5:
                                if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 13)
                                    Result.D = 1.00*(standardWeights.topTime + standardWeights.buttomWrongTime + standardWeights.buttomDuplicateRate + Base) * DWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 14)
                                    Result.D = 1.00*(standardWeights.topTime + standardWeights.buttomWrongTime + standardWeights.mediumDuplicateRate + Base) * DWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 15)
                                    Result.D = 1.00*(standardWeights.topTime + standardWeights.buttomWrongTime + standardWeights.topDuplicateRate + Base) * DWeight;
                                /* <-------------------------------------------------------->
                                 */
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 17)
                                    Result.D = 1.00*(standardWeights.topTime + standardWeights.mediumWrongTime + standardWeights.buttomDuplicateRate + Base) * DWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 18)
                                    Result.D = 1.00*(standardWeights.topTime + standardWeights.mediumWrongTime + standardWeights.mediumDuplicateRate + Base) * DWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 19)
                                    Result.D = 1.00*(standardWeights.topTime + standardWeights.mediumWrongTime + standardWeights.topDuplicateRate + Base) * DWeight;
                                /* <-------------------------------------------------------->
                                 */
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 21)
                                    Result.D = 1.00*(standardWeights.topTime + standardWeights.topWrongTime + standardWeights.buttomDuplicateRate + Base) * DWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 22)
                                    Result.D = 1.00*(standardWeights.topTime + standardWeights.topWrongTime + standardWeights.mediumDuplicateRate + Base) * DWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 23)
                                    Result.D = 1.00*(standardWeights.topTime + standardWeights.topWrongTime + standardWeights.topDuplicateRate + Base) * DWeight;
                                /* <-------------------------------------------------------->
                                 */
                                break;
                            default:
                                break;
                        }
                        s -> results = Result.D;
                    }
                    break;
                case 'E':
                    if(passJudge(s -> pass) == 0 || duplicateJudge(s ->duplicateRate) == 9)
                        Result.E = 0.00;
                    else if(passJudge(s -> pass) == 1 && duplicateJudge(s -> duplicateRate != 9)) {
                        switch (timeJudge(s -> time)) {
                            case 2:
                                if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 13)
                                    Result.E = 1.00*(standardWeights.buttomTime + standardWeights.buttomWrongTime + standardWeights.buttomDuplicateRate + Base) * EWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 14)
                                    Result.E = 1.00*(standardWeights.buttomTime + standardWeights.buttomWrongTime + standardWeights.mediumDuplicateRate + Base) * EWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 15)
                                    Result.E = 1.00*(standardWeights.buttomTime + standardWeights.buttomWrongTime + standardWeights.topDuplicateRate + Base) * EWeight;
                                /* <-------------------------------------------------------->
                                 */
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 17)
                                    Result.E = 1.00*(standardWeights.buttomTime + standardWeights.mediumWrongTime + standardWeights.buttomDuplicateRate + Base) * EWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 18)
                                    Result.E = 1.00*(standardWeights.buttomTime + standardWeights.mediumWrongTime + standardWeights.mediumDuplicateRate + Base) * EWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 19)
                                    Result.E = 1.00*(standardWeights.buttomTime + standardWeights.mediumWrongTime + standardWeights.topDuplicateRate + Base) * EWeight;
                                /* <-------------------------------------------------------->
                                 */
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 21)
                                    Result.E = 1.00*(standardWeights.buttomTime + standardWeights.topWrongTime + standardWeights.buttomDuplicateRate + Base) * EWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 22)
                                    Result.E = 1.00*(standardWeights.buttomTime + standardWeights.topWrongTime + standardWeights.mediumDuplicateRate + Base) * EWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 23)
                                    Result.E = 1.00*(standardWeights.buttomTime + standardWeights.topWrongTime + standardWeights.topDuplicateRate + Base) * EWeight;
                                /* <-------------------------------------------------------->
                                 */
                                break;
                            case 4:
                                if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 13)
                                    Result.E = 1.00*(standardWeights.higherTime + standardWeights.buttomWrongTime + standardWeights.buttomDuplicateRate + Base) * EWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 14)
                                    Result.E = 1.00*(standardWeights.higherTime + standardWeights.buttomWrongTime + standardWeights.mediumDuplicateRate + Base) * EWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 15)
                                    Result.E = 1.00*(standardWeights.higherTime + standardWeights.buttomWrongTime + standardWeights.topDuplicateRate + Base) * EWeight;
                                /* <-------------------------------------------------------->
                                 */
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 17)
                                    Result.E = 1.00*(standardWeights.higherTime + standardWeights.mediumWrongTime + standardWeights.buttomDuplicateRate + Base) * EWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 18)
                                    Result.E = 1.00*(standardWeights.higherTime + standardWeights.mediumWrongTime + standardWeights.mediumDuplicateRate + Base) * EWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 19)
                                    Result.E = 1.00*(standardWeights.higherTime + standardWeights.mediumWrongTime + standardWeights.topDuplicateRate + Base) * EWeight;
                                /* <-------------------------------------------------------->
                                 */
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 21)
                                    Result.E = 1.00*(standardWeights.higherTime + standardWeights.topWrongTime + standardWeights.buttomDuplicateRate + Base) * EWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 22)
                                    Result.E = 1.00*(standardWeights.higherTime + standardWeights.topWrongTime + standardWeights.mediumDuplicateRate + Base) * EWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 23)
                                    Result.E = 1.00*(standardWeights.higherTime + standardWeights.topWrongTime + standardWeights.topDuplicateRate + Base) * EWeight;
                                /* <-------------------------------------------------------->
                                 */
                                break;
                            case 3:
                                if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 13)
                                    Result.E = 1.00*(standardWeights.mediumTime + standardWeights.buttomWrongTime + standardWeights.buttomDuplicateRate + Base) * EWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 14)
                                    Result.E = 1.00*(standardWeights.mediumTime + standardWeights.buttomWrongTime + standardWeights.mediumDuplicateRate + Base) * EWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 15)
                                    Result.E = 1.00*(standardWeights.mediumTime + standardWeights.buttomWrongTime + standardWeights.topDuplicateRate + Base) * EWeight;
                                /* <-------------------------------------------------------->
                                 */
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 17)
                                    Result.E = 1.00*(standardWeights.mediumTime + standardWeights.mediumWrongTime + standardWeights.buttomDuplicateRate + Base) * EWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 18)
                                    Result.E = 1.00*(standardWeights.mediumTime + standardWeights.mediumWrongTime + standardWeights.mediumDuplicateRate + Base) * EWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 19)
                                    Result.E = 1.00*(standardWeights.mediumTime + standardWeights.mediumWrongTime + standardWeights.topDuplicateRate + Base) * EWeight;
                                /* <-------------------------------------------------------->
                                 */
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 21)
                                    Result.E = 1.00*(standardWeights.mediumTime + standardWeights.topWrongTime + standardWeights.buttomDuplicateRate + Base) * EWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 22)
                                    Result.E = 1.00*(standardWeights.mediumTime + standardWeights.topWrongTime + standardWeights.mediumDuplicateRate + Base) * EWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 23)
                                    Result.E = 1.00*(standardWeights.mediumTime + standardWeights.topWrongTime + standardWeights.topDuplicateRate + Base) * EWeight;
                                /* <-------------------------------------------------------->
                                 */
                                break;
                            case 5:
                                if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 13)
                                    Result.E = 1.00*(standardWeights.topTime + standardWeights.buttomWrongTime + standardWeights.buttomDuplicateRate + Base) * EWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 14)
                                    Result.E = 1.00*(standardWeights.topTime + standardWeights.buttomWrongTime + standardWeights.mediumDuplicateRate + Base) * EWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 15)
                                    Result.E = 1.00*(standardWeights.topTime + standardWeights.buttomWrongTime + standardWeights.topDuplicateRate + Base) * EWeight;
                                /* <-------------------------------------------------------->
                                 */
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 17)
                                    Result.E = 1.00*(standardWeights.topTime + standardWeights.mediumWrongTime + standardWeights.buttomDuplicateRate + Base) * EWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 18)
                                    Result.E = 1.00*(standardWeights.topTime + standardWeights.mediumWrongTime + standardWeights.mediumDuplicateRate + Base) * EWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 19)
                                    Result.E = 1.00*(standardWeights.topTime + standardWeights.mediumWrongTime + standardWeights.topDuplicateRate + Base) * EWeight;
                                /* <-------------------------------------------------------->
                                 */
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 21)
                                    Result.E = 1.00*(standardWeights.topTime + standardWeights.topWrongTime + standardWeights.buttomDuplicateRate + Base) * EWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 22)
                                    Result.E = 1.00*(standardWeights.topTime + standardWeights.topWrongTime + standardWeights.mediumDuplicateRate + Base) * EWeight;
                                else if(mixtureJudge(duplicateJudge(s -> duplicateRate), wrongtimeJudge(s -> wrongTimes)) == 23)
                                    Result.E = 1.00*(standardWeights.topTime + standardWeights.topWrongTime + standardWeights.topDuplicateRate + Base) * EWeight;
                                /* <-------------------------------------------------------->
                                 */
                                break;
                            default:
                                break;
                        }
                        s -> results = Result.E;
                    }
                    break;
                default:
                    break;
                }
            }
            s = s -> next;
        }
        
        string quit;
        cout << "Do you want to continue or quit? [q/n]" << endl;
        cin >> quit;
        if(quit == "q") break;
        else continue;
    }
}
#endif /* FunctionMain_h */
