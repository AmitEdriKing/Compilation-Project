#pragma once
#ifndef PARSER_H
#define PARSER_H
#include <stdio.h>
#include "Token.h"

typedef enum eRule{
    eRule_PROGRAM = 0,
    eRule_VAR_DEFINITIONS,
    eRule_VAR_DEFINITIONS_TAG,
    eRule_VAR_DEFITION,
    eRule_TYPE,
    eRule_VARIABLES_LIST,
    eRule_VARIABLES_LIST_TAG,
    eRule_VARIABLE,
    eRule_VARIABLE_TAG,
    eRule_FUNC_DEFINTIONS,
    eRule_FUNC_DEFINTIONS_TAG,
    eRule_FUNC_DEFINTION,
    eRule_RETURNED_TYPE,
    eRule_PARAM_DEFINITIONS,
    eRule_STATEMENTS,
    eRule_STATEMENTS_TAG,
	eRule_STATEMENT_TAG_2,
	eRule_STATEMENT,
    eRule_STATEMENT_TAG,
    eRule_BLOCK,
    eRule_FUNCTION_CALL,
    eRule_PARAMETERS_LIST,
    eRule_EXPRESSION,
    eRule_EXPRESSION_TAG
}eRule;


Token* current_token;


/////////////////////////////////////////    Match() functions

void match(eTOKENS eType);
void match_ar_op();

/////////////////////////////////////////    Parse_X() functions

void parse();

void parse_VAR_DEFITION();
void parse_VAR_DEFINITIONS();
void parse_VAR_DEFINITIONS_TAG(); 
void parse_PROGRAM();
void parse_TYPE();
void parse_VARIABLES_LIST();
void parse_VARIABLES_LIST_TAG();
void parse_VARIABLE(); 
void parse_VARIABLE_TAG();
void parse_FUNC_DEFINITIONS();
void parse_FUNC_DEFINITIONS_TAG();
void parse_FUNC_DEFINITION();
void parse_RETURNED_TYPE();
void parse_PARAM_DEFINITIONS(); 
void parse_STATEMENTS();
void parse_STATEMENTS_TAG(); 
void parse_STATEMENT();
void parse_STATEMENT_TAG();
void parse_STATEMENT_TAG_2();
void parse_BLOCK(); 
void parse_FUNCTION_CALL();
void parse_PARAMETERS_LIST();
void parse_EXPRESSION();
void parse_EXPRESSION_TAG();


/////////////////////////////////////////    Error handling

void errorHandling(eRule current_rule);
void findFollowingToken(eTOKENS* followArray, int size);
void printError(Token* token, eTOKENS* followArray, int size);
void printFollowArray(eTOKENS* followArray, int size);

/////////////////////////////////////////    follow(X) functions

int isTokenFollowRule(Token* token, eTOKENS* followArray, int size);

#endif