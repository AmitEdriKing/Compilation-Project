#include "SymbolTable.h"

// Creates symbol table and links it to the table of the current scope
symbol_table *make_table(symbol_table *current_table)
{
	// new symbol table
	symbol_table *table = (symbol_table *)malloc(sizeof(symbol_table));

	assert(table);

	// links it to the the current scope
	table->father = current_table;

	// new hash table
	table->table = new_hash_table();


	return table;
}

// Returns pointer to the enclosing table (table of the "father")
symbol_table *pop_table(symbol_table *current_table)
{
	symbol_table *table = NULL;

	if(current_table)
	{
		// pointer to the enclosing table
		table = current_table->father;

		// free hash table
		free_hash_table(current_table->table);

		// free symbol table
		free(current_table);
	}		
	
	return table;
}

// Inserts variable name and type into symbol table of the current scope, and returns pointer to the new entry
table_entry *insert(symbol_table *current_table, char *lexeme)
{
	table_entry *entry = NULL;

	// name did not appear in the current table
	if(!lookup(current_table, lexeme))
	{
		// create new table entry
		entry = insert_hash_table(current_table->table, lexeme);
	}

	return entry;
}

// Returns the found entry pointer of the current scope or NULL
table_entry *lookup(symbol_table *current_table, char *lexeme)
{
	// search hash table the given key table entry
	return search_hash_table(current_table->table, lexeme);
}

// Returns the found entry pointer (hierarchical search) or NULL 
table_entry *find(symbol_table *current_table, char *lexeme)
{
	table_entry *entry = NULL;
	symbol_table *table = current_table;   // saving original table pointer

	while(table)
	{
		// search current scope table entry
		entry = lookup(table, lexeme);

		// entry found
		if(entry)
		{
			break;
		}
		// entry not found
		else
		{
			// hierarchical search - go to your father
			table = table->father;
		}
	}

	return entry;
}

// Store table entry type in the symbol type
void set_id_kind(table_entry *entry, elm_type *element)
{
	if(entry)
	{
		// sets table entry type
		entry->value = element;
	}
}

// Returns table entry type
elm_type *get_id_kind(table_entry *entry)
{
	elm_type *element = NULL;

	if(entry)
	{
		// sets table entry type
		element = entry->value;
	}

	return element;
}


// Checks if all declarations been used
int used_all_declarations(symbol_table *st)
{
	int used_all = 1;
	int i;

	if(st)
	{
		
		ListNode *node;

		for(i = 0; i < st->table->size; i++)
		{
			if(st->table->items[i])
			{
				node = st->table->items[i]->head;

				while(node)
				{
					if(node->entry->value->used == 0)
					{
						used_all = 0;
						break;
					}

					node = node->next;
				}
			}

			if(used_all == 0)
			{
				break;
			}
		}
	}
	return used_all;
}

void InsertlistToTable(symbol_table *st,id_list *lstOfIds)
{
	elm_type *element=new_elmType();
	table_entry *entry=new_table_entry();
	id_list* curr=lstOfIds;

	while(curr != NULL)
	{
		entry=find(st,curr->lexema);
		if(!entry)
		{
			element->kind=curr->type;
			element->size=curr->sizeArr;
			element->used=0;
			entry=insert(st,curr->lexema);
			set_id_kind(entry,element);
		}
		element=new_elmType();
		curr=curr->next;
	}
}