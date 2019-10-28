#pragma once
#include "Token.h"
#include "Parser.h"
#include <stdio.h>
#include <stdlib.h>


/* This package describes the storage of tokens identified in the input text.
* The storage is a bi-directional list of nodes.
* Each node is an array of tokens; the size of this array is defined as TOKEN_ARRAY_SIZE.
* Such data structure supports an efficient way to manipulate tokens.

There are three functions providing an external access to the storage:
- function create_and_store_tokens ; it is called by the lexical analyzer when it identifies a legal token in the input text.
- functions next_token and back_token; they are called by parser during the syntax analysis (the second stage of compilation)
*/

#define TOKEN_ARRAY_SIZE 100

int currentIndex = 0;
Node* currentNode = NULL;
Node* headNode = NULL;

/*
* This function creates a token and stores it in the storage.
*/
void create_and_store_token(eTOKENS kind, char* lexeme, int numOfLine)
{ 
	// case 1: there is still no tokens in the storage.

	if (currentNode == NULL)
	{
		currentNode = (Node*)malloc(sizeof(Node));

		if(currentNode == NULL)
		{
			fprintf(yyout,"\nUnable to allocate memory! \n"); 
			exit(0);
		}

		currentNode->tokensArray = (Token*)malloc(sizeof(Token)*TOKEN_ARRAY_SIZE);
		if(currentNode->tokensArray == NULL)
		{
			fprintf(yyout,"\nUnable to allocate memory! \n"); 
			exit(0);
		}

		currentNode->tokensArray[currentIndex].kind = TOKEN_FIRSTTOKEN;
		currentIndex++;
		currentNode->prev = NULL;
		currentNode->next = NULL;
		headNode = currentNode;
	}
	// case 2: at least one token exsits in the storage.

	else
	{
		// the array (the current node) is full, need to allocate a new node
		if (currentIndex == TOKEN_ARRAY_SIZE - 1)
		{
			currentIndex = 0;
			currentNode->next = (Node*)malloc(sizeof(Node));

			if(currentNode == NULL)
			{
				fprintf(yyout,"\nUnable to allocate memory! \n"); 
				exit(0);
			}

			currentNode->next->prev = currentNode;
			currentNode = currentNode->next;
			currentNode->tokensArray = (Token*)malloc(sizeof(Token)*TOKEN_ARRAY_SIZE);
			if(currentNode->tokensArray == NULL)
			{
				fprintf(yyout,"\nUnable to allocate memory! \n"); 
				exit(0);
			}

			currentNode->next = NULL;
		}
		// the array (the current node) is not full
		else
		{
			currentIndex++;
		}
	}

	currentNode->tokensArray[currentIndex].kind = kind;
	currentNode->tokensArray[currentIndex].lexeme = (char*)malloc(sizeof(char)*(strlen(lexeme)+1));
	strcpy(currentNode->tokensArray[currentIndex].lexeme, lexeme);
	currentNode->tokensArray[currentIndex].lineNumber = numOfLine;
}

/*
* This function returns the token in the storage that is stored immediately before the current token (if exsits).
*/
Token *back_token() {
	Token* retToken  = NULL;

	if (currentNode != NULL){
		if ((currentIndex > 0) && (currentIndex < TOKEN_ARRAY_SIZE)){ // if the back token is in  the current Node token's array
			retToken = &(currentNode->tokensArray[--currentIndex]);
		}
		else if ((currentIndex == 0) && (currentNode->prev != NULL)){ // if the back token is in the previous Node token's array
			retToken = &(currentNode->prev->tokensArray[TOKEN_ARRAY_SIZE-1]);
			currentIndex = TOKEN_ARRAY_SIZE-1;
			currentNode = currentNode->prev;
		}
	}

	return retToken;
}

/*
* If the next token already exists in the storage (this happens when back_token was called before this call to next_token): 
*  this function returns the next stored token.
* Else: continues to read the input file in order to identify, create and store a new token (using yylex function);
*  returns the token that was created.
*/
Token *next_token()
{

	Token* retToken  = NULL;

	if((currentNode != NULL))
	{
		if((currentNode->tokensArray[currentIndex+1]).lineNumber >= 0 && currentIndex < TOKEN_ARRAY_SIZE-1)//in the next move we find something
		{
			retToken = &(currentNode->tokensArray[++currentIndex]);
		}
		else
		{
			if( currentNode->next != NULL)
			{
				retToken = &(currentNode->next->tokensArray[0]);
				currentNode=currentNode->next;
				currentIndex=0;
			}
			else
			{
				yylex();
				retToken = &(currentNode->tokensArray[currentIndex]);
			}
		}
	}
	else
	{
		yylex();
		retToken = &(currentNode->tokensArray[currentIndex]);
	}

	return retToken;		
}

/*
* eTokenToString convert an enum type to char* by using switch case by the enum value
*/
char* eTokenToString(eTOKENS etoken){
	char* retString = (char*)malloc(sizeof(char) * 50);

	switch(etoken){
	case TOKEN_PROGRAM:
		strcpy(retString, "PROGRAM\0");
		break;
	case TOKEN_END:
		strcpy(retString, "END\0");
		break;
	case TOKEN_REAL:
		strcpy(retString, "REAL\0");
		break;
	case TOKEN_INTEGER:
		strcpy(retString, "INTEGER\0");
		break;
	case TOKEN_VOID:
		strcpy(retString, "VOID\0");
		break;
	case TOKEN_RETURN:
		strcpy(retString, "RETURN\0");
		break;
	case TOKEN_ID:
		strcpy(retString, "ID\0");
		break;
	case TOKEN_REAL_NUM:
		strcpy(retString, "REAL_NUM\0");
		break;
	case TOKEN_INT_NUM:
		strcpy(retString, "INT-NUM\0");
		break;
	case TOKEN_MULTIPLICATION:
		strcpy(retString, "MULTIPLICATION\0");
		break;
	case TOKEN_DIVISION:
		strcpy(retString, "DIVISION\0");
		break;
	case TOKEN_ASSIGMENT:
		strcpy(retString, "ASSIGNMENT\0");
		break;
	case TOKEN_COMMA:
		strcpy(retString, "COMMA\0");
		break;
	case TOKEN_SEMICOLON:
		strcpy(retString, "SEMICOLON\0");
		break;
	case TOKEN_LEFT_BRACKET:
		strcpy(retString, "LEFT_BRACKET\0");
		break;
	case TOKEN_RIGHT_BRACKET:
		strcpy(retString, "RIGHT_BRACKET\0");
		break;
	case TOKEN_LEFT_CURLY_BRACE:
		strcpy(retString, "LEFT_CURLY_BRACE\0");
		break;
	case TOKEN_RIGHT_CURLY_BRACE:
		strcpy(retString, "RIGHT_CURLY_BRACE\0");
		break;
	case TOKEN_LEFT_PARENTHESIS:
		strcpy(retString, "LEFT_PARENTHESIS\0");
		break;
	case TOKEN_RIGHT_PARENTHESIS:
		strcpy(retString, "RIGHT_PARENTHESIS\0");
		break;
	case EOFF:
		strcpy(retString, "EOF\0");
		break;
	
	}

	return retString;
}

/*
*	printTokenStruct() - print a given struct to the desired file
*/
void printTokenStruct(Token token)
{
	if((token.kind != TOKEN_FIRSTTOKEN) ){
		fprintf(yyout, "Token of type {%s} was found at line: {%d}, lexeme: {%s}.\n", eTokenToString(token.kind), token.lineNumber, token.lexeme);
	}
}




/*
*	freeTokenSTorage() - free the token's collection
*/
void freeTokenSTorage(){
	Node* temp = headNode;

	while(temp != NULL){

		temp = headNode;
		if(temp->next != NULL){
			headNode = headNode->next;
			free(temp->tokensArray);
			free(temp);
		}
		else{
			free(headNode->tokensArray);
			free(headNode);
			headNode = NULL;
			currentNode = NULL;
			temp = NULL;
			currentIndex = 0;
		}
	}
}

void goToFirstTokenInStorage(){
	currentNode = headNode;
	currentIndex = 0;
}
