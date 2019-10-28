#ifndef LIST_H
#define LIST_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "HashTable.h"

typedef struct ListNode{
	HashTable* table;
	struct ListNode* next;
}ListNode;

void addNodeToList(ListNode** current_node, HashTable** table);
void freeList(ListNode* current_node);

#endif