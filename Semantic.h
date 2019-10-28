#ifndef SEMANTIC_H
#define SEMANTIC_H
#include <stdio.h>
#include "Token.h"
#include "Parser.h"
#include "SymbolTable.h"

/////////////////////////////////////////    
// TODO:
// 1. Type Checking - a.VAR_DEFINITION -> check if the variable type is defined
//					  b.TYPE_DEFINITION -> check if the type is already defined	
// 2. Scope Analysis - a.VAR_DEFINITION -> check if the variable type is defined in the same table 
//					  b.TYPE_DEFINITION -> check if the type is already defined	in the same table or above
/////////////////////////////////////////  
void printAllTables();
void freeTableList();

/////////////////////////////////////////    semantic_Match() functions		/////////////////////////////////////////
void semantic_match(eTOKENS eType);
void semantic_match_ar_op();

/////////////////////////////////////////    semantic_Parse_X() functions	/////////////////////////////////////////
void semantic_parse();
void parse_BB(eKind*kind);
void parse_FB();
void semantic_parse_PROGRAM();
void semantic_parse_VAR_DEFITION(id_list** lst,eCommand *c);
id_list* semantic_parse_VAR_DEFINITIONS(eCommand *c);
void semantic_parse_VAR_DEFINITIONS_TAG(id_list** lst,eCommand *c); 
eKind semantic_parse_TYPE();
int semantic_parse_VARIABLES_LIST(eKind *kind,id_list** lst,eCommand *c);
void semantic_parse_VARIABLES_LIST_TAG(int *size,eKind *kind,id_list** lst,eCommand *c,int* count);
void semantic_parse_VARIABLE(int *,eKind * ,id_list** lst,eCommand *c,int* count); 
void semantic_parse_VARIABLE_TAG(char* name, int *size,eKind *kind,id_list** lst,eCommand *c,int* count);
void semantic_parse_FUNC_DEFINITIONS();
void semantic_parse_FUNC_DEFINITIONS_TAG();
void semantic_parse_FUNC_DEFINITION();
eKind semantic_parse_RETURNED_TYPE();
id_list* semantic_parse_PARAM_DEFINITIONS(); 
void semantic_parse_STATEMENTS(eCommand *,eKind *returnedType);
void semantic_parse_STATEMENTS_TAG(eCommand *,eKind *returnedType); 
void semantic_parse_STATEMENT(eCommand *);
void semantic_parse_STATEMENT_TAG(eCommand *c,eKind *kind);
void semantic_parse_STATEMENT_TAG_2(char* id_name,eKind* idkind,eCommand *c);
void semantic_parse_BLOCK(eCommand *c,eKind *kind,id_list *parametersList);
void semantic_parse_FUNCTION_CALL();
int semantic_parse_PARAMETERS_LIST();
eKind semantic_parse_EXPRESSION();
eKind semantic_parse_EXPRESSION_TAG(eKind *lastElementKind);



/////////////////////////////////////////    Error handling		/////////////////////////////////////////
void semantic_errorHandling(eRule current_rule);


#endif