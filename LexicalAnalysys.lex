
%{ 
#include <stdio.h> 
#include <stdlib.h>
#include "Token.h" 
#include "Parser.h"
#include "Semantic.h"

	int line = 1;
%}

NEW_LINE            \n
WHITE_SPACE         " "
TAB                 \t
PROGRAM "program"
END "end"
REAL "real"
INTEGER "integer"
VOID "void"
RETURN "return" 
LETTER [a-zA-Z]
DIGIT [0-9]
SIGN {"+"|"-"}
ID [A-Za-z](("_"[0-9A-Za-z]+)?([0-9A-Za-z])*)*
REAL_NUM [1-9]{DIGIT}*"."{DIGIT}+|0"."{DIGIT}+
INT_NUM (0)|([1-9]{DIGIT}*)
MULTIPLICATION "*"   
DIVISION "/"     
ASSIGMENT "="
COMMA ","
SEMICOLON ";"
LEFT_PARENTHESIS "["
RIGHT_PARENTHESIS "]"
LEFT_BRACKET "("
RIGHT_BRACKET ")"
LEFT_CURLY_BRACE "{"
RIGHT_CURLY_BRACE "}"
COMMENT "--".*
ERROR .
END_OF_FILE   <<EOF>>


%%

{WHITE_SPACE}       {}
{TAB}               {}
{PROGRAM}		{fprintf(yyout, "Token of kind '{PROGRAM}' was found at line: {%d}, lexeme: '{%s}'.\n",line,yytext);create_and_store_token(TOKEN_PROGRAM, yytext ,line);return 1;}
{END}			{fprintf(yyout, "Token of kind '{END}' was found at line: {%d}, lexeme: '{%s}'.\n",line,yytext);create_and_store_token(TOKEN_END, yytext ,line);return 1;}
{REAL}			{fprintf(yyout, "Token of kind '{REAL}' was found at line: {%d}, lexeme: '{%s}'.\n",line,yytext);create_and_store_token(TOKEN_REAL, yytext ,line);return 1;}
{INTEGER}		{fprintf(yyout, "Token of kind '{INTEGER}' was found at line: {%d}, lexeme: '{%s}'.\n",line,yytext);create_and_store_token(TOKEN_INTEGER, yytext ,line);return 1;}
{VOID}			{fprintf(yyout, "Token of kind '{VOID}' was found at line: {%d}, lexeme: '{%s}'.\n",line,yytext);create_and_store_token(TOKEN_VOID, yytext ,line);return 1;}
{RETURN}		{fprintf(yyout, "Token of kind '{RETURN}' was found at line: {%d}, lexeme: '{%s}'.\n",line,yytext);create_and_store_token(TOKEN_RETURN, yytext ,line);return 1;}
{REAL_NUM}		{fprintf(yyout, "Token of kind '{REAL_NUM}' was found at line: {%d}, lexeme: '{%s}'.\n",line,yytext);create_and_store_token(TOKEN_REAL_NUM, yytext ,line);return 1;}
{INT_NUM}		{fprintf(yyout, "Token of kind '{INT_NUM}' was found at line: {%d}, lexeme: '{%s}'.\n",line,yytext);create_and_store_token(TOKEN_INT_NUM, yytext ,line);return 1;}
{ID}			{fprintf(yyout, "Token of kind '{ID}' was found at line: {%d}, lexeme: '{%s}'.\n",line,yytext);create_and_store_token(TOKEN_ID, yytext ,line);return 1;}
{MULTIPLICATION}	{fprintf(yyout, "Token of kind '{MULTIPLICATION}' was found at line: {%d}, lexeme: '{%s}'.\n",line,yytext);create_and_store_token(TOKEN_MULTIPLICATION, yytext ,line);return 1;}
{DIVISION}		{fprintf(yyout, "Token of kind '{DIVISION}' was found at line: {%d}, lexeme: '{%s}'.\n",line,yytext);create_and_store_token(TOKEN_DIVISION, yytext ,line);return 1;}
{ASSIGMENT}		{fprintf(yyout, "Token of kind '{ASSIGNMENT}' was found at line: {%d}, lexeme: '{%s}'.\n",line,yytext);create_and_store_token(TOKEN_ASSIGMENT, yytext ,line);return 1;}
{COMMA}			{fprintf(yyout, "Token of kind '{COMMA}' was found at line: {%d}, lexeme: '{%s}'.\n",line,yytext);create_and_store_token(TOKEN_COMMA, yytext ,line);return 1;}
{SEMICOLON}		{fprintf(yyout, "Token of kind '{SEMICOLON}' was found at line: {%d}, lexeme: '{%s}'.\n",line,yytext);create_and_store_token(TOKEN_SEMICOLON, yytext ,line);return 1;}
{LEFT_PARENTHESIS}	{fprintf(yyout, "Token of kind '{LEFT_PARENTHESIS}' was found at line: {%d}, lexeme: '{%s}'.\n",line,yytext);create_and_store_token(TOKEN_LEFT_PARENTHESIS, yytext ,line);return 1;}
{RIGHT_PARENTHESIS}	{fprintf(yyout, "Token of kind '{RIGHT_PARENTHESIS}' was found at line: {%d}, lexeme: '{%s}'.\n",line,yytext);create_and_store_token(TOKEN_RIGHT_PARENTHESIS, yytext ,line);return 1;}
{LEFT_BRACKET}		{fprintf(yyout, "Token of kind '{LEFT_BRACKET}' was found at line: {%d}, lexeme: '{%s}'.\n",line,yytext);create_and_store_token(TOKEN_LEFT_BRACKET, yytext ,line);return 1;}
{RIGHT_BRACKET}		{fprintf(yyout, "Token of kind '{RIGHT_BRACKET}' was found at line: {%d}, lexeme: '{%s}'.\n",line,yytext);create_and_store_token(TOKEN_RIGHT_BRACKET, yytext ,line);return 1;}
{LEFT_CURLY_BRACE}	{fprintf(yyout, "Token of kind '{LEFT_CURLY_BRACE}' was found at line: {%d}, lexeme: '{%s}'.\n",line,yytext);create_and_store_token(TOKEN_LEFT_CURLY_BRACE, yytext ,line);return 1;}
{RIGHT_CURLY_BRACE}	{fprintf(yyout, "Token of kind '{RIGHT_CURLY_BRACE}' was found at line: {%d}, lexeme: '{%s}'.\n",line,yytext);create_and_store_token(TOKEN_RIGHT_CURLY_BRACE, yytext ,line);return 1;}

{COMMENT}           	{}

{NEW_LINE}          	{line++;}

<<EOF>>			{fprintf(yyout, "Token of kind '{END_OF_FILE}' was found ,we at the end of file!"); create_and_store_token(EOFF, yytext ,line);return 1;}

{ERROR}			{fprintf(yyout, "The character '{%s}' at line: %d does not begin any legal token in the language.\n", yytext, line);}

%%                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            
void main()
{
   	yyin = fopen("C:\\temp\\test1.txt","r");
    yyout = fopen("C:\\temp\\test1_315793794_310471362_lex.txt", "w");
	yyout2 = fopen("C:\\temp\\test1_315793794_310471362_syntactic.txt", "w");
    	yyout3 = fopen("C:\\temp\\test1_315793794_310471362_semantic.txt", "w");
    if(yyin && yyout && yyout2 && yyout3){
		parse();
goToFirstTokenInStorage();
		semantic_parse();
  fclose(yyin);
    fclose(yyout);
    fclose(yyout2);
    fclose(yyout3);
    freeTokenSTorage();
	}
	else{
		yyout = stdout;
	}
  
    line = 1;
	yyrestart(yyin);    
    

    // yyin = fopen("C:\\temp\\test2.txt","r");
    yyin = fopen("C:\\temp\\test2.txt","r");    
    yyout = fopen("C:\\temp\\test2_315793794_310471362_lex.txt", "w");
	yyout2 = fopen("C:\\temp\\test2_315793794_310471362_syntactic.txt", "w");
	yyout3 = fopen("C:\\temp\\test2_315793794_310471362_semantic.txt", "w");
    if(yyin && yyout && yyout2 && yyout3) 
	{
		 parse();
goToFirstTokenInStorage();
	         semantic_parse();
    fclose(yyin);
    fclose(yyout);
    fclose(yyout2);
    fclose(yyout3);
    freeTokenSTorage();
	}
	else 
	{
		yyout = stdout;
	}


}
