#include "List.h"

void addNodeToList(ListNode** current_node, HashTable** table) {
	ListNode* node = (ListNode*)malloc(sizeof(ListNode));
	
	
	if(*current_node != NULL){
		node->table = *table;
		node->next = NULL;
		(*current_node)->next = node;
	}
	else {
		*current_node = (ListNode*)malloc(sizeof(ListNode));	
		(*current_node)->next = NULL;
		(*current_node)->table = *table;
	}
}

void freeList(ListNode* list_head) {
	ListNode* temp = list_head;

	while(list_head != NULL){
		temp = list_head;
		list_head = list_head->next;
		delete_all(temp->table);
		free(temp);
	}
}