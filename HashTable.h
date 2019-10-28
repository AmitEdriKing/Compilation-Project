#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "ElementType.h"

// Table entry
typedef struct table_entry
{
	// table key
	char *key;

	// table value
	elm_type* value;

}table_entry;

// List node
typedef struct ListNode
{
	// the data stored
	table_entry *entry;

	// next node
	struct ListNode *next;

}ListNode;

// List
typedef struct List
{
	// list size
	int list_size;

	// head of the list
	ListNode *head;

}List;

// Hash Table
typedef struct hash_table
{
	// array size
	int size;

	// array of lists
	List **items;

}hash_table;

// New table entry
table_entry *new_table_entry();

// New list node
ListNode *new_list_node();

// New list
List *new_list();

// New hash table
hash_table *new_hash_table();

// Free table entry
void free_table_entry(table_entry *);

// Free list noide
void free_list_node(ListNode *);

// Free list
void free_list(List *);

// Free hash table
void free_hash_table(hash_table *);

// Insert list
ListNode *insert_list(List *, char *);

// Insert table key and value and returns the index of the stored entry
table_entry *insert_hash_table(hash_table *, char *);

// Search list for a specific key
ListNode *search_list(List *, char *);

// Search table entry in the hash table
table_entry *search_hash_table(hash_table *, char *);

// Find index for the entry
int probe_index(hash_table *, char *);

// Hash function - converts a string into a number 
unsigned long hash(char *);

#endif