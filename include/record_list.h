#ifndef STUDENTPTA_RECORD_LIST_H
#define STUDENTPTA_RECORD_LIST_H

#include "record.h"

// Singly linked list of records, appended at the tail.
// Storage only: no input, output or scoring in here.
struct RecordNode {
    Record data;
    RecordNode* next;
};

struct RecordList {
    RecordNode* head;
    RecordNode* tail;
};

void initList(RecordList& list);
void append(RecordList& list, const Record& record);
void freeList(RecordList& list);

#endif
