//
//  FunctionsAnnounce.h
//  NewOne
//
//  Created by MarkfuGod on 2022/11/22.
//

#ifndef FunctionsAnnounce_h
#define FunctionsAnnounce_h
#include "Definition.h"
using namespace::std;
void PrintStar();
void PrintCross();
void PrintStart();
void PrintWelcome();
void PrintZZU();
void PrintMainMenu();
void PrintGoodBye();
void PrintChoice_1();
void PrintChoice_2();
void PrintPTA();

void modifyModifyWeights();
void InitialWeights();
void InitList(studentList &L);
void CreateFromTail(studentList &L);
void showListElement(studentList L);
void showSpecifiedListElement(studentNode* L, long long studentNumber);
void calculateResults(studentNode *L);
void judgeWeights();
void SetWeights();
void EditValue(string choice);

studentNode* Locate(studentList L, long long studentNumber);

int timeJudge(int time);
int passJudge(int pass);
int duplicateJudge(int duplicate);
int wrongtimeJudge(int wrongtime);
int Strains(int value);
int mixtureJudge(int x, int y);

#endif /* FunctionsAnnounce_h */
