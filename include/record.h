#ifndef STUDENTPTA_RECORD_H
#define STUDENTPTA_RECORD_H

// One submission: one student's attempt at one problem.
struct Record {
    long long studentNumber;
    char problem;       // 'A'..'E', difficulty increases with the letter
    int time;           // seconds used
    bool passed;
    int wrongTimes;     // wrong submissions before passing
    int duplicateRate;  // plagiarism check, percent 0..100
    double result;      // weighted score, filled in by scoring
};

#endif
