
#ifndef ELEMENT_TYPE_H
#define ELEMENT_TYPE_H
#include <stdio.h>
#include <stdlib.h>


typedef enum eKind
{
	Integer,
	IntegerArray,
	IntegerFunction,
	Real,
	RealArray,
	RealFunction,
	Void,
	Error_type,
	Return,
	NULLKind

}eKind;

typedef enum eCommand
{
	FunctionType,
	RegularType,
	FunctionCallType
}eCommand;


typedef struct id_list
{
    struct id_list *next;
    char* lexema;
    int sizeArr;
    eKind type;
	int used;
    
}id_list;



typedef struct elm_type
{
	eKind kind;

	int size;

	int numOfFuncParameters;

	int used;

	id_list *parameters_id;

}elm_type;


id_list *new_id_list();
void print_list(id_list * head);                            // print
void add(id_list ** head, int arr,eKind type, char *lex);   // push new node to the end
id_list* findId(id_list *head, char* lex);                   //serch by lexema
void free_listOfIds(id_list* head);                          //free list
int List_Size(id_list* head);
id_list * concatenate(id_list *head1, id_list *head2);       //concat two list
int checktwolistskind(id_list *p, id_list  *q);

void free_elm_type(elm_type* element);

elm_type* new_elmType();


#endif 