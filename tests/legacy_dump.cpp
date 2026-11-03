// One-off harness: runs the ORIGINAL scoring code (FunctionMain.h) over every
// tier/boundary combination and writes the results to a CSV. That CSV is the
// frozen oracle the refactored scoring is checked against.
//
//   g++ -std=c++11 -I. tests/legacy_dump.cpp -o /tmp/legacy_dump
//   /tmp/legacy_dump tests/golden_scores.csv > /dev/null
#include <cstdio>
#include <sstream>
#include "../FunctionMain.h"

int main(int argc, char** argv) {
    if (argc != 2) { fprintf(stderr, "usage: %s out.csv\n", argv[0]); return 2; }

    const int times[]  = {0, 500, 501, 1000, 1001, 3000, 3001, 10000};
    const int wrongs[] = {0, 1, 3, 4, 10};
    const int dups[]   = {0, 20, 21, 35, 36, 50, 51, 100};
    const int passes[] = {0, 1};
    const char probs[] = "ABCDE";

    InitialWeights();
    studentList L;
    InitList(L);
    studentNode* tail = L;
    long long id = 100000000000LL;
    std::ostringstream script;  // what a user would type into calculateResults

    for (char p : std::string(probs))
    for (int t : times) for (int pa : passes) for (int w : wrongs) for (int d : dups) {
        studentNode* s = (studentNode*)malloc(sizeof(studentNode));
        s->studentNumber = ++id;
        s->problemNumber = p;
        s->time = t;
        s->pass = pa;
        s->wrongTimes = w;
        s->duplicateRate = d;
        s->visited = 0;
        s->results = 0.00;  // same initial value CreateFromTail uses
        s->next = NULL;
        tail->next = s;
        tail = s;
        script << s->studentNumber << " n ";
    }
    script << "0\n";

    std::istringstream in(script.str());
    std::streambuf* oldIn = cin.rdbuf(in.rdbuf());
    std::ostringstream sink;
    std::streambuf* oldOut = cout.rdbuf(sink.rdbuf());
    calculateResults(L);
    cin.rdbuf(oldIn);
    cout.rdbuf(oldOut);

    FILE* f = fopen(argv[1], "w");
    if (!f) { perror(argv[1]); return 1; }
    fprintf(f, "problem,time,passed,wrongTimes,duplicateRate,expected\n");
    for (studentNode* s = L->next; s; s = s->next)
        fprintf(f, "%c,%d,%d,%d,%d,%.6f\n", s->problemNumber, s->time, s->pass,
                s->wrongTimes, s->duplicateRate, s->results);
    fclose(f);
    return 0;
}
