#pragma once
#include "Token.h"
#include "Parser.h"


/////////////////////////////////////////    Match() functions		/////////////////////////////////////////
void match(eTOKENS eType){
	current_token = next_token();

	if (current_token->kind != eType){
		fprintf(yyout2, "Expected token '%s' at line: '%d', Actual token '%s', lexeme: '%s'.\n",
			eTokenToString(eType), current_token->lineNumber, eTokenToString(current_token->kind), current_token->lexeme);
	}
}

void match_ar_op() {
	current_token = next_token();

	if (current_token->kind != TOKEN_MULTIPLICATION && current_token->kind != TOKEN_DIVISION) {
		fprintf(yyout2, "\n****************MATCH_AR_OP - ERROR****************\n");
		fprintf(yyout2, "Expected token ar_op ( / , * ) at line: '%d', Actual token '%s', lexeme: '%s'.\n",
			current_token->lineNumber, eTokenToString(current_token->kind), current_token->lexeme);
		fprintf(yyout2, "****************MATCH_AR_OP - ERROR****************\n\n");
	}
}
/*
1.create semantic file
2.flag semantic

*/
/////////////////////////////////////////    Parse_X() functions		/////////////////////////////////////////
void parse()
{
	parse_PROGRAM();
	match(EOFF);
}

void parse_PROGRAM(){
	// first {program}, follow {EOF}
	current_token = next_token();

	switch (current_token->kind){
	case TOKEN_PROGRAM:
		fprintf(yyout2, "Rule (PROGRAM -> program VAR_DEFINITIONS; STATEMENTS end FUNC_DEFINITIONS)\n");
		parse_VAR_DEFINITIONS();
		match(TOKEN_SEMICOLON);
		parse_STATEMENTS();
		match(TOKEN_END);
		parse_FUNC_DEFINITIONS();
		break;
	default:
		errorHandling(eRule_PROGRAM);
	}
}

void parse_VAR_DEFINITION(){
	// first {real,integer}, follow { ';' , ')'  }
	current_token = next_token();

	switch (current_token->kind)
	{
	case TOKEN_REAL:
	case TOKEN_INTEGER:
		fprintf(yyout2, "Rule (VAR_DEFINITION -> TYPE  VARIABLES_LIST )\n");
		current_token = back_token();
		parse_TYPE();
		parse_VARIABLES_LIST();
		break;
	default:
		errorHandling(eRule_VAR_DEFITION);

	}
}

void parse_VAR_DEFINITIONS(){
	// first {REAL,INTEGER}, follow {';',')'}
	current_token = next_token();

	switch (current_token->kind){
	case TOKEN_REAL:
	case TOKEN_INTEGER:
		fprintf(yyout2, "Rule (VAR_DEFINITIONS -> VAR_DEFINITION VAR_DEFINITIONS_TAG) \n");
		current_token = back_token();
		parse_VAR_DEFINITION();
		parse_VAR_DEFINITIONS_TAG();
		break;
	default:
		errorHandling(eRule_VAR_DEFINITIONS);
	}
}

void parse_VAR_DEFINITIONS_TAG(){
	// first {;}, follow { ';' , ')' }
	current_token = next_token();

	switch (current_token->kind)
	{
	case TOKEN_SEMICOLON:
		current_token = next_token();
		switch (current_token->kind){
		case TOKEN_REAL:
		case TOKEN_INTEGER:
			fprintf(yyout2, "Rule (VAR_DEFINITIONS_TAG -> ; VAR_DEFINITIONS)\n");
			current_token = back_token();
			parse_VAR_DEFINITIONS();
			break;
		case TOKEN_RIGHT_BRACKET:
			fprintf(yyout2, "Rule (VAR_DEFINITIONS_TAG  -> epsilon)\n");
			current_token = back_token();
			break;
		case TOKEN_SEMICOLON:
			fprintf(yyout2, "Rule (VAR_DEFINITIONS_TAG  -> epsilon)\n");
			current_token = back_token();
			break;
		default:
			fprintf(yyout2, "Rule (VAR_DEFINITIONS_TAG  -> epsilon)\n");
			current_token = back_token();
			current_token = back_token();
		}
	break;
	case TOKEN_RIGHT_BRACKET:
		fprintf(yyout2, "Rule (VAR_DEFINITIONS_TAG  -> epsilon)\n");
		current_token = back_token();
		break;
	default:
		errorHandling(eRule_VAR_DEFINITIONS_TAG);
	}
}


void parse_TYPE(){
	// first {real,integer}, follow { id }
	current_token = next_token();

	switch (current_token->kind)
	{
	case TOKEN_REAL:
		fprintf(yyout2, "Rule (TYPE ->  real)  \n");
		break;
	case TOKEN_INTEGER:
		fprintf(yyout2, "Rule (TYPE ->  integer)  \n");
		break;
	default:
		errorHandling(eRule_TYPE);

	}
}

void parse_VARIABLES_LIST(){
	// first {id}, follow {')'}
	current_token = next_token();

	switch (current_token->kind){
	case TOKEN_ID:
		fprintf(yyout2, "Rule (VARIABLES_LIST -> VARIABLE VARIABLES_LIST_TAG ) \n");
		current_token=back_token();
		parse_VARIABLE();
		parse_VARIABLES_LIST_TAG();
		break;
	default:
		errorHandling(eRule_VARIABLES_LIST);
		break;
	}
}

void parse_VARIABLES_LIST_TAG(){
	// first {','}, follow {  ';' , ')'  }
	current_token = next_token();

	switch (current_token->kind)
	{
	case TOKEN_COMMA:
		fprintf(yyout2, "Rule (VARIABLES_LIST_TAG ->, VARIABLE VARIABLES_LIST_TAG)\n");
		parse_VARIABLE();
		parse_VARIABLES_LIST_TAG();
		break;
	case TOKEN_SEMICOLON:
		fprintf(yyout2 , "Rule (VARIABLES_LIST_TAG   -> epsilon)\n");
		current_token=back_token();
		break;
	case TOKEN_RIGHT_BRACKET:
		fprintf(yyout2, "(Rule VARIABLES_LIST_TAG   -> epsilon)\n");
		current_token=back_token();
		break;
	default:
		errorHandling(eRule_VARIABLES_LIST_TAG);

	}
}

void parse_VARIABLE(){
	// first {'id'}, follow {  ';' , '=' , ')') }
	current_token = next_token();

	switch (current_token->kind)
	{
	case TOKEN_ID:
		fprintf(yyout2, "Rule (VARIABLE  -> id  VARIABLE_TAG) \n");
		parse_VARIABLE_TAG();
		break;
	default:
		errorHandling(eRule_VARIABLE);
	}
}

void parse_VARIABLE_TAG(){
	// first {'['}, follow {  ';' , '=' , ')',',' }
	current_token = next_token();

	switch (current_token->kind)
	{
	case TOKEN_LEFT_PARENTHESIS:
		fprintf(yyout2, "Rule (VARIABLE_TAG   -> [int_number]) \n");
		match(TOKEN_INT_NUM);
		match(TOKEN_RIGHT_PARENTHESIS);
		break;
	case TOKEN_SEMICOLON:
	case TOKEN_COMMA:
	case TOKEN_RIGHT_BRACKET:
	case TOKEN_ASSIGMENT:
		fprintf(yyout2, "Rule (VARIABLE_TAG   -> epsilon)\n");
		current_token=back_token();
		break;
	default:
		errorHandling(eRule_VARIABLE_TAG);
	}
}

void parse_FUNC_DEFINITIONS(){
	// first {'void,real,integer'}, follow {  eof }
	current_token = next_token();

	switch (current_token->kind)
	{
	case TOKEN_VOID:
	case TOKEN_INTEGER:
	case TOKEN_REAL:
		fprintf(yyout2, "Rule (FUNC_DEFINITIONS   -> FUNC_DEFINITION FUNC_DEFINITIONS _TAG)\n");
		current_token = back_token();
		parse_FUNC_DEFINITION();
		parse_FUNC_DEFINITIONS_TAG();
		break;
	default:
		errorHandling(eRule_FUNC_DEFINTIONS);
	}
}


void parse_FUNC_DEFINITIONS_TAG(){
	// first {'void,real,integer'}, follow {  eof }
	current_token = next_token();

	switch (current_token->kind)
	{
	case TOKEN_VOID:
	case TOKEN_INTEGER:
	case TOKEN_REAL:
		fprintf(yyout2, "Rule (FUNC_DEFINITIONS_TAG   -> FUNC_DEFINITION FUNC_DEFINITIONS_TAG)\n");
		current_token=back_token();
		parse_FUNC_DEFINITION();
		parse_FUNC_DEFINITIONS_TAG();
		break;
	case EOFF:
		fprintf(yyout2, "Rule (FUNC_DEFINITIONS_TAG   -> epsilon)\n");
		break;
	default:
		errorHandling(eRule_FUNC_DEFINTIONS_TAG);
	}
}


void parse_FUNC_DEFINITION(){
	// first {VOID,REAL,INTEGER}, follow {EOF, VOID, RAEL,INTEGER}
	current_token = next_token();

	switch (current_token->kind) {
	case TOKEN_VOID:
	case TOKEN_REAL:
	case TOKEN_INTEGER:
		fprintf(yyout2, "Rule (FUNC_DEFINITION -> RETURNED_TYPE  id  (PARAM_DEFINITIONS)  BLOCK ) \n");
		current_token = back_token();
		parse_RETURNED_TYPE();
		match(TOKEN_ID);
		match(TOKEN_LEFT_BRACKET);
		parse_PARAM_DEFINITIONS();
		match(TOKEN_RIGHT_BRACKET);
		parse_BLOCK();
		break;
	default:
		errorHandling(eRule_FUNC_DEFINTION);
	}
}

void parse_RETURNED_TYPE(){
	// first {VOID,REAL,INTEGER}, follow {ID}
	current_token = next_token();

	switch (current_token->kind) {
	case TOKEN_VOID:
		fprintf(yyout2, "Rule (RETURNED_TYPE  -> void) \n");
		break;
	case TOKEN_REAL:
	case TOKEN_INTEGER:
		fprintf(yyout2, "Rule (RETURNED_TYPE  ->  TYPE)\n");
		current_token = back_token();
		parse_TYPE();
		break;
	default:
		errorHandling(eRule_RETURNED_TYPE);
	}
}

void parse_PARAM_DEFINITIONS()
{
	// first {REAL,INTEGER}, follow {')'}
	current_token = next_token();

	switch (current_token->kind) {
	case TOKEN_REAL:
	case TOKEN_INTEGER:
		fprintf(yyout2, "Rule (PARAM_DEFINITIONS -> VAR_DEFINITIONS)\n");
		current_token = back_token();
		parse_VAR_DEFINITIONS();
		break;
	case TOKEN_RIGHT_BRACKET:
		fprintf(yyout2, "Rule (PARAM_DEFINITIONS -> epsilon)\n");
		current_token=back_token();
		break;
	default:
		errorHandling(eRule_PARAM_DEFINITIONS);
	}
}

void parse_STATEMENTS(){
	// first { id, return, '{' }, follow { ')' } 
	current_token = next_token();

	switch (current_token->kind) {
	case TOKEN_ID:
	case TOKEN_RETURN:
	case TOKEN_LEFT_CURLY_BRACE:
		fprintf(yyout2, "Rule (STATEMENTS -> STATEMENT; STATEMENTS_TAG) \n");	
		current_token = back_token();
		parse_STATEMENT();
		match(TOKEN_SEMICOLON);
		parse_STATEMENTS_TAG();			
		break;
	default:
		errorHandling(eRule_STATEMENTS);
	}
}

void parse_STATEMENTS_TAG(){
	// first { id, return, '{' }, follow { '}', end } 
	current_token = next_token();

	switch (current_token->kind) {
	case TOKEN_ID:
	case TOKEN_RETURN:
	case TOKEN_LEFT_CURLY_BRACE:
		fprintf(yyout2, "Rule (STATEMENTS_TAG -> STATEMENTS) \n");	
		current_token = back_token();
		parse_STATEMENTS();		
		break;
	case TOKEN_END:
	case TOKEN_RIGHT_CURLY_BRACE:
		fprintf(yyout2, "Rule (STATEMENTS_TAG -> epsilon) \n");
		current_token=back_token();
		break;
	default:
		errorHandling(eRule_STATEMENTS_TAG);
	}
}

void parse_STATEMENT(){
	// first { id, return, '{' }, follow { ';' } 
	current_token = next_token();

	switch (current_token->kind) {
	case TOKEN_ID:
		fprintf(yyout2, "Rule (STATEMENT ->   id STATEMENT_TAG_2)  \n");
		parse_STATEMENT_TAG_2();
		break;
	case TOKEN_LEFT_CURLY_BRACE:
		fprintf(yyout2, "Rule (STATEMENT ->  BLOCK )   \n");
		current_token = back_token();
		parse_BLOCK();
		break;
	case TOKEN_RETURN:
		fprintf(yyout2, "Rule (STATEMENT -> return  STATEMENT_TAG) \n");	
		parse_STATEMENT_TAG();
		break;
	default:
		errorHandling(eRule_STATEMENT);
	}
}

void parse_STATEMENT_TAG_2()
{
	// first { '(' , '[' , '=' } , follow { ';' }
	current_token = next_token();

		switch (current_token->kind) {
		case TOKEN_LEFT_PARENTHESIS:
			fprintf(yyout2, "Rule (STATEMENT_TAG_2 ->  VARIABLE_TAG = EXPRESSION )\n");	
			current_token=back_token();
			parse_VARIABLE_TAG();
			match(TOKEN_ASSIGMENT);
			parse_EXPRESSION();
		break;
		case TOKEN_ASSIGMENT:
			fprintf(yyout2, "Rule (STATEMENT_TAG_2 ->  id = EXPRESSION )\n");
			parse_EXPRESSION();	
		break;
		case TOKEN_LEFT_BRACKET:
			fprintf(yyout2, "Rule (STATEMENT_TAG_2 ->  FUNCTION_CALL) \n");	
			current_token=back_token();//
			current_token=back_token();//to return to id and parse FUNCTION_CALL
			parse_FUNCTION_CALL();
			break;
		default:
			errorHandling(eRule_STATEMENT_TAG_2);
	}
}
void parse_STATEMENT_TAG(){
	// first { int_num , real_num , id } , follow { ';' }
	current_token = next_token();

	switch (current_token->kind) {
	case TOKEN_INT_NUM:
	case TOKEN_REAL_NUM:
	case TOKEN_ID:
		fprintf(yyout2, "Rule (STATEMENT_TAG -> EXPRESSION )\n");	
		current_token=back_token();
		parse_EXPRESSION();		
		break;
	case TOKEN_SEMICOLON:
		fprintf(yyout2, "Rule (STATEMENT_TAG -> epsilon )\n");
		current_token=back_token();    //to check
		break;
	default:
		errorHandling(eRule_STATEMENT_TAG);
	}
}

void parse_BLOCK(){
	// first { '{' } , follow { ';',void, real ,integer,eof }
	current_token = next_token();

	switch (current_token->kind) {
	case TOKEN_LEFT_CURLY_BRACE:
		fprintf(yyout2, "Rule (BLOCK -> { VAR_DEFINITIONS; STATEMENTS })\n");
		parse_VAR_DEFINITIONS();
		match(TOKEN_SEMICOLON);
		parse_STATEMENTS();
		match(TOKEN_RIGHT_CURLY_BRACE);
		break;
	default:
		errorHandling(eRule_BLOCK);
	}
}

void parse_FUNCTION_CALL(){
	// first { id } , follow { ';'}
	current_token = next_token();

	switch (current_token->kind) {
	case TOKEN_ID:
		fprintf(yyout2, "Rule FUNCTION_CALL -> id (PARAMETERS_LIST))   \n");
		match(TOKEN_LEFT_BRACKET);
		parse_PARAMETERS_LIST();
		match(TOKEN_RIGHT_BRACKET);
		break;
	default:
		errorHandling(eRule_FUNCTION_CALL);
	}
}

void parse_PARAMETERS_LIST(){
	// first { id } , follow { ')' }
	current_token = next_token();

	switch (current_token->kind) {
	case TOKEN_ID:
		fprintf(yyout2, "Rule (PARAMETERS_LIST -> VARIABLES_LIST) \n");	
		current_token=back_token();
		parse_VARIABLES_LIST();		
		break;
	case TOKEN_RIGHT_BRACKET:
		fprintf(yyout2, "Rule PARAMETERS_LIST -> epsilon) \n");
		current_token=back_token();
		break;
	default:
		errorHandling(eRule_PARAMETERS_LIST);
	}
}




void parse_EXPRESSION(){
	// first { id, int-number, real-number } , follow { ';' }
	current_token = next_token();

	switch (current_token->kind) {
	case TOKEN_REAL_NUM:
		fprintf(yyout2, "Rule (EXPRESSION ->  real_number) \n");
		break;
	case TOKEN_INT_NUM:
		fprintf(yyout2, "Rule (EXPRESSION -> int_number)  \n");
		break;
	case TOKEN_ID:
		fprintf(yyout2, "Rule (EXPRESSION -> id  EXPRESSION_TAG)\n");
		parse_EXPRESSION_TAG();
		break;
	default:
		errorHandling(eRule_EXPRESSION);
	}
}

void parse_EXPRESSION_TAG(){
	// first { '[', '*', '/'} , follow {  ';'}  
	current_token = next_token();

	switch (current_token->kind) {
	case TOKEN_LEFT_PARENTHESIS:
		fprintf(yyout2, "Rule (EXPRESSION_TAG -> VARIABLE_TAG)  \n");
		current_token=back_token();
		parse_VARIABLE_TAG();
		break;
	case TOKEN_MULTIPLICATION:
	case TOKEN_DIVISION:
		fprintf(yyout2, "Rule (EXPRESSION_TAG ->ar_op EXPRESSION)  \n");
		current_token=back_token();
		match_ar_op();
		parse_EXPRESSION();
	break;
	default:
		errorHandling(eRule_EXPRESSION_TAG); 
	}
}


/////////////////////////////////////////		Error handling		/////////////////////////////////////////



void errorHandling(eRule current_rule){

	// follow Array
	eTOKENS followPROGRAM[] = {EOFF};
	eTOKENS	followVAR_DEFINITIONS[] = { TOKEN_SEMICOLON,TOKEN_RIGHT_BRACKET };
	eTOKENS	followVAR_DEFINITIONS_TAG[] = { TOKEN_SEMICOLON,TOKEN_RIGHT_BRACKET };
	eTOKENS	followVAR_DEFINITION[] = { TOKEN_SEMICOLON,TOKEN_RIGHT_BRACKET };

	eTOKENS	followTYPE[] = { TOKEN_ID};
	eTOKENS	followVARIABLES_LIST[] = { TOKEN_RIGHT_BRACKET,TOKEN_SEMICOLON };
	eTOKENS	followVARIABLES_LIST_TAG[] = { TOKEN_RIGHT_BRACKET, TOKEN_SEMICOLON };

	eTOKENS	followVARIABLE[] = { TOKEN_SEMICOLON,TOKEN_ASSIGMENT,TOKEN_RIGHT_BRACKET };
	eTOKENS	followVARIABLE_TAG[] = { TOKEN_SEMICOLON,TOKEN_ASSIGMENT,TOKEN_RIGHT_BRACKET};
	eTOKENS	followFUNC_DEFINITIONS[] = { EOFF };
	eTOKENS	followFUNC_DEFINITIONS_TAG[] = { EOFF };
	eTOKENS	followFUNC_DEFINITION[] = { TOKEN_VOID , TOKEN_REAL, TOKEN_INTEGER,EOFF};
	eTOKENS	followRETURNED_TYPE[] = { TOKEN_ID };
	eTOKENS followPARAM_DEFINITIONS[] = {TOKEN_RIGHT_BRACKET};
	eTOKENS followSTATEMENTS[] = { TOKEN_END,TOKEN_RIGHT_CURLY_BRACE };
	eTOKENS followSTATEMENTS_TAG[] = { TOKEN_END,TOKEN_RIGHT_CURLY_BRACE };
	eTOKENS followSTATEMENT[] = { TOKEN_SEMICOLON };
	eTOKENS followSTATEMENT_TAG[] = { TOKEN_SEMICOLON };
	eTOKENS followSTATEMENT_TAG_2[] = { TOKEN_SEMICOLON };
	eTOKENS followBLOCK[] = { TOKEN_VOID,TOKEN_REAL,TOKEN_INTEGER,EOFF,TOKEN_SEMICOLON}; 
	eTOKENS followFUNCTION_CALL[] = { TOKEN_SEMICOLON}; 
	eTOKENS followPARAMETERS_LIST[] = { TOKEN_RIGHT_BRACKET };
	eTOKENS followEXPRESSION[] = { TOKEN_SEMICOLON };
	eTOKENS followEXPRESSIONS_TAG[] = { TOKEN_SEMICOLON  }; 

	// First Array

	eTOKENS firstPROGRAM[] = { TOKEN_PROGRAM};
	eTOKENS firstVAR_DEFINITIONS[] = { TOKEN_REAL, TOKEN_INTEGER };
	eTOKENS firstVAR_DEFINITIONS_TAG[] = {TOKEN_SEMICOLON};
	eTOKENS firstVAR_DEFINITION[] = { TOKEN_REAL , TOKEN_INTEGER }; 
	eTOKENS firstTYPE[] = { TOKEN_REAL , TOKEN_INTEGER }; 
	eTOKENS firstVARIABLES_LIST[] = { TOKEN_ID };
	eTOKENS firstVARIABLES_LIST_TAG[] = { TOKEN_COMMA};
	eTOKENS firstVARIABLE[] = { TOKEN_ID };
	eTOKENS firstVARIABLE_TAG[] = { TOKEN_LEFT_PARENTHESIS };
	eTOKENS firstFUNC_DEFINITIONS[] = { TOKEN_VOID , TOKEN_REAL , TOKEN_INTEGER };
	eTOKENS firstFUNC_DEFINITIONS_TAG[] = { TOKEN_VOID , TOKEN_REAL , TOKEN_INTEGER  };
	eTOKENS firstFUNC_DEFINITION[] = { TOKEN_VOID , TOKEN_REAL , TOKEN_INTEGER };
	eTOKENS firstRETURNED_TYPE[] = { TOKEN_VOID , TOKEN_REAL , TOKEN_INTEGER };
	eTOKENS firstPARAM_DEFINITIONS[] = { TOKEN_REAL, TOKEN_INTEGER }; 
	eTOKENS firstSTATEMENTS[] = { TOKEN_ID, TOKEN_RETURN, TOKEN_LEFT_CURLY_BRACE }; 
	eTOKENS firstSTATEMENTS_TAG[] = { TOKEN_ID, TOKEN_RETURN, TOKEN_LEFT_CURLY_BRACE};
	eTOKENS firstSTATEMENT[] = { TOKEN_ID, TOKEN_RETURN, TOKEN_LEFT_CURLY_BRACE }; 
	eTOKENS firstSTATEMENT_TAG[] = { TOKEN_ID , TOKEN_INT_NUM , TOKEN_REAL_NUM };
	eTOKENS firstSTATEMENT_TAG_2[] = { TOKEN_LEFT_BRACKET , TOKEN_LEFT_PARENTHESIS, TOKEN_ASSIGMENT};
	eTOKENS firstBLOCK[] = { TOKEN_LEFT_CURLY_BRACE}; 
	eTOKENS firstFUNCTION_CALL[] = { TOKEN_ID }; 
	eTOKENS firstPARAMETERS_LIST[] = { TOKEN_ID}; 

	eTOKENS firstEXPRESSIONS[] = { TOKEN_ID , TOKEN_INT_NUM , TOKEN_REAL_NUM }; 
	eTOKENS firstEXPRESSIONS_TAG[] = { TOKEN_LEFT_PARENTHESIS, TOKEN_DIVISION, TOKEN_MULTIPLICATION }; 



	switch (current_rule){
	case eRule_PROGRAM:
		printError(current_token, firstPROGRAM, 1);
		findFollowingToken(followPROGRAM, 1);
		break;
	case eRule_VAR_DEFINITIONS:
		printError(current_token, firstVAR_DEFINITIONS, 2);
		findFollowingToken(followVAR_DEFINITIONS, 2);
		break;
	case eRule_VAR_DEFINITIONS_TAG:
		printError(current_token, firstVAR_DEFINITIONS_TAG, 1);
		findFollowingToken(followVAR_DEFINITIONS_TAG ,2);
		break;
	case eRule_VAR_DEFITION:
		printError(current_token, firstVAR_DEFINITION, 2);
		findFollowingToken(followVAR_DEFINITION, 2);
		break;
	case eRule_TYPE:
		printError(current_token, firstTYPE, 2);
		findFollowingToken(followTYPE, 1);
		break;
	case eRule_VARIABLES_LIST:
		printError(current_token, firstVARIABLES_LIST, 1);
		findFollowingToken(followVARIABLES_LIST, 2);
		break;
	case eRule_VARIABLES_LIST_TAG:
		printError(current_token, firstVARIABLES_LIST_TAG, 1);
		findFollowingToken(followVARIABLES_LIST_TAG, 2);
		break;
	case eRule_VARIABLE:
		printError(current_token, firstVARIABLE, 1);
		findFollowingToken(followVARIABLE, 3);
		break;
	case eRule_VARIABLE_TAG:
		printError(current_token, firstVARIABLE_TAG, 1);
		findFollowingToken(followVARIABLE_TAG, 3);
		break;
	case eRule_FUNC_DEFINTIONS:
		printError(current_token, firstFUNC_DEFINITIONS, 3);
		findFollowingToken(followFUNC_DEFINITIONS, 1);
		break;
	case eRule_FUNC_DEFINTIONS_TAG:
		printError(current_token, firstFUNC_DEFINITIONS_TAG, 3);
		findFollowingToken(followFUNC_DEFINITIONS_TAG, 1);
		break;
	case eRule_FUNC_DEFINTION:
		printError(current_token, firstFUNC_DEFINITION, 3);
		findFollowingToken(followFUNC_DEFINITION, 4);
		break;
	case eRule_RETURNED_TYPE:
		printError(current_token, firstRETURNED_TYPE, 3);
		findFollowingToken(followRETURNED_TYPE, 1);
		break;
	case eRule_PARAM_DEFINITIONS:
		printError(current_token, firstPARAM_DEFINITIONS, 2);
		findFollowingToken(followPARAM_DEFINITIONS, 1);
		break;
	case eRule_STATEMENTS:
		printError(current_token, firstSTATEMENTS, 3);
		findFollowingToken(followSTATEMENTS, 2);
		break;
	case eRule_STATEMENTS_TAG:
		printError(current_token, firstSTATEMENTS_TAG, 3);
		findFollowingToken(followSTATEMENTS_TAG, 2);
		break;
	case eRule_STATEMENT:
		printError(current_token, firstSTATEMENT, 3);
		findFollowingToken(followSTATEMENT, 1);
		break;
	case eRule_STATEMENT_TAG:
		printError(current_token, firstSTATEMENT_TAG, 3);
		findFollowingToken(followSTATEMENT_TAG, 1);
		break;
	case eRule_STATEMENT_TAG_2:
		printError(current_token, firstSTATEMENT_TAG_2, 3);
		findFollowingToken(followSTATEMENT_TAG_2, 1);
		break;
	case eRule_BLOCK:
		printError(current_token, firstBLOCK, 1);
		findFollowingToken(followBLOCK, 5);
		break;
	case eRule_FUNCTION_CALL:
		printError(current_token, firstFUNCTION_CALL, 1);
		findFollowingToken(followFUNCTION_CALL, 1);
		break;
	case eRule_PARAMETERS_LIST:
		printError(current_token, firstPARAMETERS_LIST, 1);
		findFollowingToken(followPARAMETERS_LIST, 1);
		break;
	case eRule_EXPRESSION:
		printError(current_token, firstEXPRESSIONS, 3);
		findFollowingToken(followEXPRESSION, 1);
		break;
	case eRule_EXPRESSION_TAG:
		printError(current_token, firstEXPRESSIONS_TAG, 3);
		findFollowingToken(followEXPRESSIONS_TAG, 1);
		break;
	}

}

void findFollowingToken(eTOKENS* followArray, int size){
	while(!isTokenFollowRule(current_token, followArray, size) && current_token->kind != EOFF){
		current_token = next_token();
	}
	if(isTokenFollowRule(current_token, followArray, size)){
		current_token = back_token();
	}
	else{
		exit(0);
		
	}
}

void printError(Token* token, eTOKENS* followArray, int size)
{
	fprintf(yyout2, "\n****************ERROR****************\n");
	fprintf(yyout2, "Expected one of token\\token's: ");
	printFollowArray(followArray, size);
	fprintf(yyout2, "\nat line: '%d', Actual token '%s', lexeme: '%s'.\n", token->lineNumber, eTokenToString(token->kind), token->lexeme);
	fprintf(yyout2, "****************ERROR****************\n\n");
}

void printFollowArray(eTOKENS* followArray, int size)
{
	int i;

	for (i = 0; i < size; i++){
		fprintf(yyout2, " %s,", eTokenToString(followArray[i]));
	} 
}

/////////////////////////////////////////    	follow(X) function		/////////////////////////////////////////

int isTokenFollowRule(Token* token, eTOKENS* followArray, int size){
	int i;
	int followRule = 0;

	for (i = 0; i < size; i++){
		if (token->kind == followArray[i]){
			followRule = 1;
			break;
		}
	}	 

	return followRule;
}