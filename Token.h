#ifndef TOKEN_H
#define TOKEN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern FILE *yyin, *yyout, *yyout2, *yyout3;
extern int yylex();

typedef enum eTOKENS
{
	TOKEN_FIRSTTOKEN=-1,
	TOKEN_PROGRAM ,
	TOKEN_END ,
	TOKEN_REAL ,
	TOKEN_INTEGER ,
	TOKEN_VOID ,
	TOKEN_RETURN ,
	TOKEN_REAL_NUM ,
	TOKEN_INT_NUM ,
	TOKEN_ID ,
	TOKEN_MULTIPLICATION ,
	TOKEN_DIVISION ,
	TOKEN_ASSIGMENT ,
	TOKEN_SIGN ,
	TOKEN_COMMA ,
	TOKEN_SEMICOLON ,
	TOKEN_LEFT_PARENTHESIS ,
	TOKEN_RIGHT_PARENTHESIS ,
	TOKEN_LEFT_BRACKET ,
	TOKEN_RIGHT_BRACKET ,
	TOKEN_LEFT_CURLY_BRACE ,
	TOKEN_RIGHT_CURLY_BRACE ,
	EOFF

}eTOKENS;

typedef struct Token
{
	eTOKENS kind;
	char *lexeme;
	int lineNumber;
}Token;

typedef struct Node
{
	Token *tokensArray;
	struct Node *prev;
	struct Node *next;
} Node;

void create_and_store_token(eTOKENS kind, char* lexeme, int numOfLine);
Token *next_token();
Token *back_token();
void printTokenStruct(Token token);
char* eTokenToString(eTOKENS etoken);
void freeTokenSTorage();
void goToFirstTokenInStorage();

#endif