#include "HashTable.h"

// Hash table size
#define SIZE 64

// New table entry
table_entry *new_table_entry()
{
	table_entry *entry = (table_entry *)malloc(sizeof(table_entry));

	assert(entry);

	entry->value = NULL;
	entry->key = NULL;

	return entry;
}

// New list node
ListNode *new_list_node()
{
	ListNode *node = (ListNode *)malloc(sizeof(ListNode));

	assert(node);

	node->entry = new_table_entry();
	node->next = NULL;

	return node;
}

// New list
List *new_list()
{
	List *lst = (List *)malloc(sizeof(List));

	assert(lst);

	lst->head = NULL;
	lst->list_size = 0;

	return lst;
}

// New hash table
hash_table *new_hash_table()
{
	hash_table *ht = (hash_table *)malloc(sizeof(hash_table));

	assert(ht);

	ht->size = SIZE;

	ht->items = (List **)calloc(ht->size, sizeof(List *));

	assert(ht->items);	

	return ht;
}

// Free table entry
void free_table_entry(table_entry *entry)
{
	if(entry)
	{
		if(entry->value)
		{
			free_elm_type(entry->value);
		}

		free(entry);
	}
}

// Free list node
void free_list_node(ListNode *node)
{
	if(node)
	{
		if(node->entry)
		{
			free_table_entry(node->entry);
		}

		free(node);
	}
}

// Free list
void free_list(List *lst)
{
	if(lst)
	{
		ListNode *current_node;

		while(lst->head)
		{
			current_node = lst->head;

			lst->head = lst->head->next;

			free_list_node(current_node);			
		}

		free(lst);
	}
}

// Free hash table
void free_hash_table(hash_table *ht)
{
	if(ht)
	{
		if(ht->items)
		{
			int i;

			for(i = 0; i < ht->size; i++)
			{
				if(ht->items[i])
				{
					free_list(ht->items[i]);
				}
			}
		}

		free(ht);
	}
}

// Insert list
ListNode *insert_list(List *lst, char *key)
{
	ListNode *node = NULL;

	if(lst)
	{
		node = new_list_node();

		node->entry->key = key;

		node->next = lst->head;

		lst->head = node;

		lst->list_size++;
	}

	return node;
}

// Insert table key and value and returns the index of the stored entry
table_entry *insert_hash_table(hash_table *ht, char *key)
{
	table_entry *entry = NULL;

	if(ht)
	{
		// index for the list entry
		int index = probe_index(ht, key);

		// first item inserted into the list
		if(ht->items[index] == NULL )
		{
			ht->items[index] = new_list();
		}

		// if item not found insert list
		if((search_list(ht->items[index], key) == NULL))
		{
			entry = insert_list(ht->items[index], key)->entry;
		}		
	}

	return entry;
}

// Search list for a specific key
ListNode *search_list(List *lst, char *key)
{
	ListNode *node = NULL;

	if(lst && key)
	{
		node = lst->head;

		while(node)
		{
			if(strcmp(node->entry->key, key) == 0)
			{
				break;
			}

			node = node->next;
		}
	}

	return node;
}

// Search table entry in the hash table
table_entry *search_hash_table(hash_table *ht, char *key)
{
	// finds the index of the entry list with the possible key
	int index = probe_index(ht, key);

	// search element in a list
	ListNode *node = search_list(ht->items[index], key);

	return node ? node->entry : NULL;
}

// Find index for the entry
int probe_index(hash_table *ht, char *key)
{
	int index;

	// gets the number that generated from a string by the hash function
	unsigned long hashed_key = hash(key);

	// make sure the index is in the correct range
	index = hashed_key % ht->size;

	// return index found
	return index;
}

// Hash function - converts a string into a number 
unsigned long hash(char *key)
{
	int c;
	unsigned long hash = 0;  

	while (c = *key++)  
	{
		hash = c + (hash << 6) + (hash << 16) - hash;
	}

	 return hash;
}