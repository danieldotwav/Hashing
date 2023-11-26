#ifndef NODE_H
#define NODE_H

#include "StudentRecord.h"

struct Node {
    StudentRecord data;
    Node* next;
    Node* prev;

    Node(StudentRecord data, Node* next = nullptr, Node* prev = nullptr)
        : data(data), next(next), prev(prev) {}
};

#endif // NODE_H