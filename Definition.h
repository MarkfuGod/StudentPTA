#ifndef Definition_h
#define Definition_h
#define AWeight 0.10
#define BWeight 0.15
#define CWeight 0.20
#define DWeight 0.25
#define EWeight 0.30
#define Base 25.00
#include <iostream>
#include <cstring>
#include <vector>
#include <cstdlib>

typedef struct student{
    long long studentNumber;
    char problemNumber;
    int time;
    int pass;
    int duplicateRate;
    int wrongTimes;
    int visited;
    double results;
    struct student* next;
}studentNode, *studentList, student;

typedef struct standard{
    int buttomTime, mediumTime, higherTime, topTime;
    int pass1, pass0;
    int buttomDuplicateRate, mediumDuplicateRate, topDuplicateRate;
    int buttomWrongTime, mediumWrongTime, topWrongTime;
}standard;

typedef struct result{
    double A;
    double B;
    double C;
    double D;
    double E;
}result;


#endif /* Definition_h */
