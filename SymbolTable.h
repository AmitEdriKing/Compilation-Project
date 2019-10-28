#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H
#include "HashTable.h"
#include "ElementType.h"

typedef struct symbol_table
{
	int returnflag;

	eKind returnKind;

	hash_table *table;

	struct symbol_table *father;

}symbol_table;

symbol_table* make_table(symbol_table* );


symbol_table* pop_table(symbol_table* );

table_entry* lookup (symbol_table* , char* );


table_entry * insert(symbol_table* , char* );

void set_id_kind(table_entry *, elm_type *);

elm_type* get_id_kind(table_entry* );

table_entry* find (symbol_table* , char* );

int used_all_declarations(symbol_table *st);

void InsertlistToTable(symbol_table *st,id_list *lstOfIds);



//to check 

//make,pop,set,get,lookup,find

#endif