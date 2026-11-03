#include "record_list.h"

void initList(RecordList& list) {
    list.head = nullptr;
    list.tail = nullptr;
}

void append(RecordList& list, const Record& record) {
    RecordNode* node = new RecordNode{record, nullptr};
    if (list.tail) {
        list.tail->next = node;
    } else {
        list.head = node;
    }
    list.tail = node;
}

void freeList(RecordList& list) {
    RecordNode* p = list.head;
    while (p) {
        RecordNode* next = p->next;
        delete p;
        p = next;
    }
    initList(list);
}

