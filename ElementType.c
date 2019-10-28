#include "ElementType.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

void free_elm_type(elm_type* element)
{
	if(element) 
	{
		if(element->numOfFuncParameters > 0)
			free_listOfIds(element->parameters_id);
		free(element);
	}
}

elm_type* new_elmType()
{
	elm_type* elementProduction=(elm_type*)malloc(sizeof(elm_type));

	elementProduction->kind = NULLKind;

	return elementProduction;
}

void print_list(id_list * head) {
    id_list * current = head;
    while (current != NULL) {
        printf("%d\t%s\t%c\n", current->sizeArr,current->lexema,current->type);
        current = current->next;
    }
}

id_list *new_id_list()
{
	id_list *node = (id_list *)malloc(sizeof(id_list));

	assert(node);

	node->next = NULL;

	return node;
}


void add(id_list ** head, int arr,eKind type, char *lex)
{
	if (*head == NULL)
	{
		*head = (id_list*)malloc(sizeof(id_list));
		(*head)->sizeArr = arr;
		(*head)->lexema = lex;
		(*head)->type = type;
		(*head)->next = NULL;
		(*head)->used=0;
	}
	else
	{
		id_list * current = *head;
		while (current->next != NULL) {
			current = current->next;
		}

		/* now we can add a new variable */
		current->next = (id_list*)malloc(sizeof(id_list));
		current->next->sizeArr = arr;
		current->next->lexema = lex;
		current->next->type = type;
		current->next->used=0;
		current->next->next = NULL;
	}
}



id_list* findId(id_list *head, char* lex)
{
    if (head == NULL)
        return NULL;
    return (!strcmp(head->lexema ,lex)) ? head : findId(head->next, lex);
}

void free_listOfIds(id_list* head)
{
    if (head == NULL)
        return;
    
    free_listOfIds(head->next);
    free(head);
}

int List_Size(id_list* head)  
{  
    int count = 0;  
    id_list* current = head; 
    while (current != NULL)  
    {  
        count++;  
        current = current->next;  
    } 

    return count; 
}

id_list * concatenate(id_list *head1, id_list *head2)
{
	id_list *p;
	if (head1 == NULL)                           
		return (head2);
	if (head2 == NULL)                       
		return (head1);
	p = head1;                            
	while (p->next != NULL)
		p = p->next;
	p->next = head2;                           
	return (head1);
}





int checktwolistskind(id_list *p, id_list  *q)
{
	while (p && q)
    {
		if ((p->type != q-> type) || (p->sizeArr != q->sizeArr))
        {
            return 0;
        }
        else
        {
            p = p->next;
            q = q->next;
        }
    }
    if (p != NULL || q != NULL)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}


