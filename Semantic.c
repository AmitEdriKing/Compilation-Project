#include "Semantic.h"
//lexema, גודל מערך  ,type//
Token* current_token; 
int match_worked; // match_worked indicate that the match was wrong 
symbol_table* current_table = NULL; // we use this variable to keep tracking of the current table
id_list* listofparameters=NULL;

/////////////////////////////////////////    semantic_Match() functions		/////////////////////////////////////////
void semantic_match(eTOKENS eType){
	current_token = next_token();
	match_worked = 1;
	
	if (current_token->kind != eType){
		match_worked = 0;
	}
}

void semantic_match_ar_op(){
	current_token = next_token();
	match_worked = 1;
	
	if (current_token->kind != TOKEN_MULTIPLICATION && current_token->kind != TOKEN_DIVISION ){
			match_worked = 0;
	}
}

/////////////////////////////////////////    Parse_X() functions		/////////////////////////////////////////
void semantic_parse()
{
	eKind kind=Void;
	parse_BB(&kind);
	semantic_parse_PROGRAM();
	semantic_match(EOFF);
	parse_FB();
}

void parse_BB( eKind *kind)
{
	current_table =  make_table(current_table);
	switch(*kind)
	{
	case Integer:
	case IntegerFunction:
		current_table->returnKind = Integer;
		current_table->returnflag=0;		
		break;
	case Real:
	case RealFunction:
		current_table->returnKind = Real;
		current_table->returnflag=0;
		break;
	case Void:
		current_table->returnKind = Void;
		current_table->returnflag=1;
		break;
	}
}

void parse_FB()
{
	if(!used_all_declarations(current_table))
	{
		fprintf(yyout3,"\r\nERROR!!: not all declerated id's is used in the scope line scope ended:%d \n",current_token->lineNumber);			
	}
	if(current_table->returnflag != 1)
	{
		fprintf(yyout3,"\r\nERROR!!: a value should be returned in the scope!!  \n");			
	
	}
	current_table=pop_table(current_table);
}

void semantic_parse_PROGRAM(){
	// first {program}, follow {EOF}
	eKind returnedType=Void;
	eCommand c=RegularType;
	eKind kind =Void;
	current_token = next_token();

	switch (current_token->kind){
	case TOKEN_PROGRAM:
		parse_BB(&kind);
		//fprintf(yyout2, "Rule (PROGRAM -> program VAR_DEFINITIONS; STATEMENTS end FUNC_DEFINITIONS)\n");
		semantic_parse_VAR_DEFINITIONS(&c);
		semantic_match(TOKEN_SEMICOLON);
		semantic_parse_STATEMENTS(&c,&returnedType);
		semantic_match(TOKEN_END);
		parse_FB();
		semantic_parse_FUNC_DEFINITIONS();
		break;
	default:
		errorHandling(eRule_PROGRAM);
	}
}



void semantic_parse_VAR_DEFINITION(id_list** lst,eCommand *c){
	// first {real,integer}, follow { ';' , ')'  }

	eKind type=NULLKind;
	current_token = next_token();
	switch (current_token->kind)
	{
	case TOKEN_REAL:
	case TOKEN_INTEGER:
		//fprintf(yyout2, "Rule (VAR_DEFINITION -> TYPE  VARIABLES_LIST )\n");
		current_token = back_token();
		switch(*c)
		{
		case FunctionType:
			type=semantic_parse_TYPE();
			semantic_parse_VARIABLES_LIST(&type,lst,c);
			break;
		case RegularType:
			type=semantic_parse_TYPE();
			semantic_parse_VARIABLES_LIST(&type,NULL,c);
			break;
		}

		break;
	default:
		errorHandling(eRule_VAR_DEFITION);
	}
}

id_list* semantic_parse_VAR_DEFINITIONS(eCommand *c){
	// first {REAL,INTEGER}, follow {';',')'}
	id_list* parameterslist=NULL;
	id_list* secondparameterslist=NULL;

	current_token = next_token();

	switch (current_token->kind){
	case TOKEN_REAL:
	case TOKEN_INTEGER:
		//fprintf(yyout2, "Rule (VAR_DEFINITIONS -> VAR_DEFINITION VAR_DEFINITIONS_TAG) \n");
		current_token = back_token();
		switch(*c){
		case FunctionType:
			semantic_parse_VAR_DEFINITION(&parameterslist,c);
			semantic_parse_VAR_DEFINITIONS_TAG(&secondparameterslist,c);
			if(secondparameterslist != NULL)
				parameterslist=concatenate(parameterslist,secondparameterslist);
			return parameterslist;
			break;
		case RegularType:
			semantic_parse_VAR_DEFINITION(NULL,c);
			semantic_parse_VAR_DEFINITIONS_TAG(NULL,c);
			break;
		}
		return parameterslist;
		break;
	default:
		errorHandling(eRule_VAR_DEFINITIONS);
	}
}

void semantic_parse_VAR_DEFINITIONS_TAG(id_list** lst,eCommand *c){
	// first {;}, follow { ';' , ')' }
	current_token = next_token();

	switch (current_token->kind)
	{
	case TOKEN_SEMICOLON:
		current_token = next_token();
		switch (current_token->kind){
		case TOKEN_REAL:
		case TOKEN_INTEGER:
			//fprintf(yyout2, "Rule (VAR_DEFINITIONS_TAG -> ; VAR_DEFINITIONS)\n");
			current_token = back_token();
			switch(*c)
			{
			case FunctionType:
				*lst = semantic_parse_VAR_DEFINITIONS(c);
				break;
			case RegularType:
				semantic_parse_VAR_DEFINITIONS(c);
			}
			break;
		case TOKEN_RIGHT_BRACKET:
			//fprintf(yyout2, "Rule (VAR_DEFINITIONS_TAG  -> epsilon)\n");
			current_token = back_token();
			break;
		case TOKEN_SEMICOLON:
			//fprintf(yyout2, "Rule (VAR_DEFINITIONS_TAG  -> epsilon)\n");
			current_token = back_token();
			break;
		default:
			//fprintf(yyout2, "Rule (VAR_DEFINITIONS_TAG  -> epsilon)\n");
			current_token = back_token();
			current_token = back_token();
		}
	break;
	case TOKEN_RIGHT_BRACKET:
		//fprintf(yyout2, "Rule (VAR_DEFINITIONS_TAG  -> epsilon)\n");
		current_token = back_token();
		break;
	default:
		errorHandling(eRule_VAR_DEFINITIONS_TAG);
	}
}


eKind semantic_parse_TYPE(){
	// first {real,integer}, follow { id }
	current_token = next_token();

	switch (current_token->kind)
	{
	case TOKEN_REAL:
		return Real;
		//fprintf(yyout2, "Rule (TYPE ->  real)  \n");
		break;
	case TOKEN_INTEGER:
		return Integer;
		//fprintf(yyout2, "Rule (TYPE ->  integer)  \n");
		break;
	default:
		errorHandling(eRule_TYPE);

	}
}

int semantic_parse_VARIABLES_LIST(eKind *kind , id_list** lst,eCommand *c)
{
	// first {id}, follow {')'}
	table_entry *entry;
	int size=1,countelements=0,count2=0;
	elm_type *type=new_elmType(); 
	current_token = next_token();
	//insert ,entry
	switch (current_token->kind){
	case TOKEN_ID:
		//fprintf(yyout2, "Rule (VARIABLES_LIST -> VARIABLE VARIABLES_LIST_TAG ) \n");
		switch(*c)
		{
		case FunctionType:
			current_token=back_token();
			semantic_parse_VARIABLE(&size,kind,lst,c,&countelements);
			semantic_parse_VARIABLES_LIST_TAG(&size,kind,lst,c,&count2);
			return countelements+count2;
			break;
		case RegularType:
			current_token=back_token();
			semantic_parse_VARIABLE(&size,kind,NULL,c,&countelements);
			semantic_parse_VARIABLES_LIST_TAG(&size,kind,NULL,c,&count2);
			return countelements+count2;
			break;
		case FunctionCallType:
			current_token=back_token();
			semantic_parse_VARIABLE(&size,kind,lst,c,&countelements);
			semantic_parse_VARIABLES_LIST_TAG(&size,kind,lst,c,&count2);
			return countelements+count2;
			break;
		}
		
		break;
	default:
		errorHandling(eRule_VARIABLES_LIST);
		break;
	}
}

void semantic_parse_VARIABLES_LIST_TAG(int *size,eKind *kind,id_list** lst,eCommand *c,int* count){
	// first {','}, follow {  ';' , ')'  }
	int count2=0;
	current_token = next_token();

	switch (current_token->kind)
	{
	case TOKEN_COMMA:
		//fprintf(yyout2, "Rule (VARIABLES_LIST_TAG ->, VARIABLE VARIABLES_LIST_TAG)\n");
		switch(*c)
		{
		case FunctionType:
		case RegularType:
			semantic_parse_VARIABLE(size,kind,lst,c,count);
			semantic_parse_VARIABLES_LIST_TAG(size,kind,lst,c,&count2);
			
			break;
		case FunctionCallType:
			semantic_parse_VARIABLE(size,kind,lst,c,count);
			semantic_parse_VARIABLES_LIST_TAG(size,kind,lst,c,&count2);
			*count=(*count)+count2;
			break;
		}
		break;
	case TOKEN_SEMICOLON:
		//fprintf(yyout2 , "Rule (VARIABLES_LIST_TAG   -> epsilon)\n");
		current_token=back_token();
		break;
	case TOKEN_RIGHT_BRACKET:
		//fprintf(yyout2, "(Rule VARIABLES_LIST_TAG   -> epsilon)\n");
		current_token=back_token();
		break;
	default:
		errorHandling(eRule_VARIABLES_LIST_TAG);

	}
}

void semantic_parse_VARIABLE(int *size,eKind *kind,id_list** lst,eCommand *c,int* count){
	// first {'id'}, follow {  ';' , '=' , ')') }
	table_entry *entry=new_table_entry();
	char* id_name;
	current_token = next_token();

	switch (current_token->kind)
	{
	case TOKEN_ID:
		switch(*c)
		{
		case FunctionType:
		case RegularType:
			fprintf(yyout2, "Rule (VARIABLE  -> id  VARIABLE_TAG) \n");
			id_name = (char*)malloc(strlen(current_token->lexeme) + 1);
			strcpy(id_name, current_token->lexeme);
	
			semantic_parse_VARIABLE_TAG(id_name,size,kind,lst,c,count);
			break;
		case FunctionCallType:
			id_name = (char*)malloc(strlen(current_token->lexeme) + 1);
			strcpy(id_name, current_token->lexeme);
			semantic_parse_VARIABLE_TAG(id_name,size,kind,lst,c,count);
			break;
		}
		break;
		default:
		errorHandling(eRule_VARIABLE);
	}
}

void semantic_parse_VARIABLE_TAG(char* name, int *size,eKind *kind,id_list** lst,eCommand *c,int *count){
	// first {'['}, follow {  ';' , '=' , ')',',' }
	char* sizeinBrackets;
	eKind kindofarrayitem=NULLKind;
	table_entry *entry=new_table_entry();
	elm_type* element=new_elmType();
	current_token = next_token();

	switch (current_token->kind)
	{
	case TOKEN_LEFT_PARENTHESIS:
		//fprintf(yyout2, "Rule (VARIABLE_TAG   -> [int_number]) \n");
		semantic_match(TOKEN_INT_NUM);
		sizeinBrackets=(char*)malloc(strlen(current_token->lexeme) + 1);
		sizeinBrackets=current_token->lexeme;
		*size = atoi(sizeinBrackets);
		semantic_match(TOKEN_RIGHT_PARENTHESIS);
		*count=*count+1;
		switch(*kind){
		case Integer:
			*kind=IntegerArray;
			break;
		case Real:
			*kind=RealArray;
			break;
		}
		switch(*c)
		{ 
		case FunctionType:
			if(!findId(*lst,name))
			{
				add(lst,*size,*kind,name);
			}
			else
			{
				fprintf(yyout3,"\r\nERROR!!: Duplicated definition of identifier %s at line %d\n", name, current_token->lineNumber);			
			}
			break;
		case RegularType:
			element->kind=*kind;
			element->size=*size;
			if(!lookup(current_table,name))
			{
				entry=insert(current_table,name);
				if (entry == NULL)
				{
					fprintf(yyout3,"\r\nERROR!!: Duplicated definition of identifier %s at line %d\n", name, current_token->lineNumber);
				}
				else
				{
					element->used=0;
					set_id_kind(entry,element);
				}
			}
			else
			{
					fprintf(yyout3,"\r\nERROR!!: the variable: %s at line %d  already exist\n", name, current_token->lineNumber);			
			}
			break;
		case FunctionCallType:
			entry=lookup(current_table,name);
			if(entry)
			{
				if(entry->value->kind == Integer || entry->value->kind == Real || entry->value->kind == RealArray || entry->value->kind == IntegerArray)
				{
					switch(*kind){
					case IntegerArray:
						kindofarrayitem=Integer;
						break;
					case RealArray:
						kindofarrayitem=Real;
						break;
				}
					add(&listofparameters,entry->value->size,kindofarrayitem,name);
				}
				else
				{
					fprintf(yyout3,"\r\nERROR!!: the variable: %s at line %d doesnt exist as a variable in the scope\n", name, current_token->lineNumber);							
				}
		break;
			}
			else
			{
				fprintf(yyout3,"\r\nERROR!!: the variable: %s at line %d doesnt exist in the scope\n", name, current_token->lineNumber);							
			}
			break;
		}
		break;
	case TOKEN_SEMICOLON:
	case TOKEN_COMMA:
	case TOKEN_RIGHT_BRACKET:
		//fprintf(yyout2, "Rule (VARIABLE_TAG   -> epsilon)\n");
		current_token=back_token();
		element->kind=*kind;
		element->size=*size;
		*count=*count+1;
		switch(*c)
		{
		case FunctionType:
			if(!findId(*lst,name))
			{
				add(lst,*size,*kind,name);
			}
			else
			{
				fprintf(yyout3,"\r\nERROR!!: Duplicated definition of identifier %s at line %d\n", name, current_token->lineNumber);			
			}
			break;
		case RegularType:
		if(!lookup(current_table,name))// scope checking 
		{
			entry=insert(current_table,name);
			if (entry == NULL )
			{
				fprintf(yyout3,"\r\nERROR!!: Duplicated definition of identifier %s at line %d\n", name, current_token->lineNumber);
			}
			else
			{
				element->used=0;
				set_id_kind(entry,element);
			}
		}
		else
		{
				fprintf(yyout3,"\r\nERROR!!: Duplicated definition of identifier %s at line %d\n", name, current_token->lineNumber);		
		}
		break;
		case FunctionCallType:
			entry=lookup(current_table,name);
			if(entry)
			{
				if(entry->value->kind == Integer || entry->value->kind == Real || entry->value->kind == RealArray || entry->value->kind == IntegerArray )
				{
					add(&listofparameters,entry->value->size,entry->value->kind,name);
				}
				else
				{
					fprintf(yyout3,"\r\nERROR!!: the variable: %s at line %d doesnt exist as a variable in the scope\n", name, current_token->lineNumber);							
				}
			}
			else
			{
					fprintf(yyout3,"\r\nERROR!!: the variable: %s at line %d doesnt exist in the scope\n", name, current_token->lineNumber);											
			}
		break;
		}
		break;
	case TOKEN_ASSIGMENT:
		current_token=back_token();
		element->kind=*kind;
		element->size=*size;
		if(!lookup(current_table,name))// scope checking 
		{
			entry=insert(current_table,name);
			if (entry == NULL)
			{
				fprintf(yyout3,"\r\nERROR!!: Duplicated definition of identifier %s at line %d\n", name, current_token->lineNumber);
			}
			else
			{
				element->used=1;
				set_id_kind(entry,element);
			}
		}
		else
		{
				fprintf(yyout3,"\r\nERROR!!: Duplicated definition of identifier %s at line %d\n", name, current_token->lineNumber);				
		}
		break;
	default:
		errorHandling(eRule_VARIABLE_TAG);
	}
}

void semantic_parse_FUNC_DEFINITIONS(){
	// first {'void,real,integer'}, follow {  eof }
	current_token = next_token();

	switch (current_token->kind)
	{
	case TOKEN_VOID:
	case TOKEN_INTEGER:
	case TOKEN_REAL:
		//fprintf(yyout2, "Rule (FUNC_DEFINITIONS   -> FUNC_DEFINITION FUNC_DEFINITIONS _TAG)\n");
		current_token = back_token();
		semantic_parse_FUNC_DEFINITION();
		semantic_parse_FUNC_DEFINITIONS_TAG();
		break;
	default:
		errorHandling(eRule_FUNC_DEFINTIONS);
	}
}


void semantic_parse_FUNC_DEFINITIONS_TAG(){
	// first {'void,real,integer'}, follow {  eof }
	current_token = next_token();

	switch (current_token->kind)
	{
	case TOKEN_VOID:
	case TOKEN_INTEGER:
	case TOKEN_REAL:
		//fprintf(yyout2, "Rule (FUNC_DEFINITIONS_TAG   -> FUNC_DEFINITION FUNC_DEFINITIONS_TAG)\n");
		current_token=back_token();
		semantic_parse_FUNC_DEFINITION();
		semantic_parse_FUNC_DEFINITIONS_TAG();
		break;
	case EOFF:
		//fprintf(yyout2, "Rule (FUNC_DEFINITIONS_TAG   -> epsilon)\n");
		break;
	}
}


void semantic_parse_FUNC_DEFINITION(){
	// first {VOID,REAL,INTEGER}, follow {EOF, VOID, RAEL,INTEGER}
	elm_type *element=new_elmType();
	eCommand lastBlockc=FunctionType;
	table_entry *entry;
	id_list* parametersList;
	eKind funcReturnType,elmkind;
	int parametersCount;
	char* func_name;
	eCommand c=FunctionType;
	current_token = next_token();
	func_name = (char*)malloc(strlen(current_token->lexeme) + 1);
	strcpy(func_name, current_token->lexeme);
	entry=new_table_entry();

	switch (current_token->kind) {
	case TOKEN_VOID:
	case TOKEN_REAL:
	case TOKEN_INTEGER:
		//fprintf(yyout2, "Rule (FUNC_DEFINITION -> RETURNED_TYPE  id  (PARAM_DEFINITIONS)  BLOCK ) \n");
		current_token = back_token();
		funcReturnType = semantic_parse_RETURNED_TYPE();
		semantic_match(TOKEN_ID);
		func_name = (char*)malloc(strlen(current_token->lexeme) + 1);
		strcpy(func_name, current_token->lexeme);
		semantic_match(TOKEN_LEFT_BRACKET);
		parametersList = semantic_parse_PARAM_DEFINITIONS();
		if(parametersList != NULL)
		{
			parametersCount=List_Size(parametersList);
		}
		else
		{
			parametersCount=0;
		}
		semantic_match(TOKEN_RIGHT_BRACKET);
		entry=find(current_table,func_name);
		if(!entry)
		{
			switch(funcReturnType)
			{
			case Integer:
				element->kind=IntegerFunction;
				break;
			case Real:
				element->kind=RealFunction;
				break;
			case Void:
				element->kind=Void;
				break;
			default:
				element->kind=Error_type;
			}
			if(element->kind!= Error_type)
			{
				element->numOfFuncParameters=parametersCount;
				if(parametersList)
				{
					element->parameters_id=parametersList;
				}
				entry=insert(current_table,func_name);
				set_id_kind(entry,element);
			}
			else
			{
				fprintf(yyout3,"\r\nERROR!!: the function should return int real or void funcname::%s at line %d\n", func_name, current_token->lineNumber);		
			}
		}
		else
		{
			fprintf(yyout3,"\r\nERROR!!: Duplicated function name :%s at line %d\n", func_name, current_token->lineNumber);		
		}
		c=FunctionType;
		semantic_parse_BLOCK(&c,&funcReturnType,&parametersList);
		break;
		default:
		errorHandling(eRule_FUNC_DEFINTIONS_TAG);
	}
}

eKind semantic_parse_RETURNED_TYPE(){
	// first {VOID,REAL,INTEGER}, follow {ID}
	eKind kind;

	current_token = next_token();
	switch (current_token->kind) {
	case TOKEN_VOID:
		current_table->returnflag=1;
		return Void;
		//fprintf(yyout2, "Rule (RETURNED_TYPE  -> void) \n");
		break;
	case TOKEN_REAL:
		kind = Real;
		return kind;
		break;
	case TOKEN_INTEGER:
		//fprintf(yyout2, "Rule (RETURNED_TYPE  ->  TYPE)\n");
		kind=Integer;
		return kind;
		break;
		default:
		errorHandling(eRule_RETURNED_TYPE);
	}
}

id_list* semantic_parse_PARAM_DEFINITIONS()
{ 
	// first {REAL,INTEGER}, follow {')'}
	eCommand c=FunctionType;
	id_list *parametersList = NULL;

	current_token = next_token();

	switch (current_token->kind) {
	case TOKEN_REAL:
	case TOKEN_INTEGER:
		//fprintf(yyout2, "Rule (PARAM_DEFINITIONS -> VAR_DEFINITIONS)\n");
		current_token = back_token();
		parametersList=(id_list*)malloc(sizeof(id_list));
		parametersList=semantic_parse_VAR_DEFINITIONS(&c);
		return parametersList;
		break;
	case TOKEN_RIGHT_BRACKET:
		//fprintf(yyout2, "Rule (PARAM_DEFINITIONS -> epsilon)\n");
		current_token=back_token();
		return parametersList;
		break;
	default:
		errorHandling(eRule_PARAM_DEFINITIONS);
	}
}

void semantic_parse_STATEMENTS(eCommand *c,eKind *returnedType){
	// first { id, return, '{' }, follow { ')' } 
	current_token = next_token();

	switch (current_token->kind) {
	case TOKEN_ID:
	case TOKEN_RETURN:
	case TOKEN_LEFT_CURLY_BRACE:
		//fprintf(yyout2, "Rule (STATEMENTS -> STATEMENT; STATEMENTS_TAG) \n");	
		current_token = back_token();
		semantic_parse_STATEMENT(c,returnedType);
		semantic_match(TOKEN_SEMICOLON);
		semantic_parse_STATEMENTS_TAG(c,returnedType);			
		break;
	default:
		errorHandling(eRule_STATEMENTS);
	}
}

void semantic_parse_STATEMENTS_TAG(eCommand *c, eKind *returnedType){
	// first { id, return, '{' }, follow { '}', end } 
	current_token = next_token();

	switch (current_token->kind) {
	case TOKEN_ID:
	case TOKEN_RETURN:
	case TOKEN_LEFT_CURLY_BRACE:
		//fprintf(yyout2, "Rule (STATEMENTS_TAG -> STATEMENTS) \n");	
		current_token = back_token();
		semantic_parse_STATEMENTS(c,returnedType);		
		break;
	case TOKEN_END:
	case TOKEN_RIGHT_CURLY_BRACE:
		//fprintf(yyout2, "Rule (STATEMENTS_TAG -> epsilon) \n");
		current_token=back_token();
		break;
	default:
		errorHandling(eRule_STATEMENTS_TAG);
	}
}

void semantic_parse_STATEMENT(eCommand *c,eKind *returnedType){
	// first { id, return, '{' }, follow { ';' } 
	char* id_name;
	eCommand lastblockc;
	eKind kind;
	table_entry* entry=new_table_entry();
	current_token = next_token();

	switch (current_token->kind) {
	case TOKEN_ID:
		//fprintf(yyout2, "Rule (STATEMENT ->   id STATEMENT_TAG_2)  \n");
		id_name = (char*)malloc(strlen(current_token->lexeme) + 1);
		strcpy(id_name, current_token->lexeme);
		entry=find(current_table,id_name);
		if(!entry)
		{
			fprintf(yyout3,"\r\nERROR!!:can't find id, id name:%s line %d is not as the id  kind\n",current_token->lexeme, current_token->lineNumber);
			semantic_parse_STATEMENT_TAG_2(id_name,&kind,c);
		}
		else
		{
			entry->value->used=1;
			kind =entry->value->kind;
			semantic_parse_STATEMENT_TAG_2(id_name,&kind,c);
		}
		break;
	case TOKEN_LEFT_CURLY_BRACE:
		//fprintf(yyout2, "Rule (STATEMENT ->  BLOCK )   \n");
		current_token = back_token();
		kind=Void;
		lastblockc=*c;
		*c=RegularType;
		semantic_parse_BLOCK(c, &kind,NULL);
		kind=*returnedType;
		*c=lastblockc;
		break;
	case TOKEN_RETURN:
		//fprintf(yyout2, "Rule (STATEMENT -> return  STATEMENT_TAG) \n");	
		switch(*c)
		{
		case FunctionType:
			semantic_parse_STATEMENT_TAG(c,returnedType);
			break;
		case RegularType:
			semantic_parse_STATEMENT_TAG(c,returnedType);
			break;
		}
		
		break;
	default:
		errorHandling(eRule_STATEMENT);
	}
}
//to check
void semantic_parse_STATEMENT_TAG_2(char *id_name,eKind *idkind,eCommand *c)
{
	// first { '(' , '[' , '=' } , follow { ';' }
	int index;
	int size=-1;
	table_entry *entry=new_table_entry();
	eKind expressionKind;
	current_token = next_token();

		switch (current_token->kind) {
		case TOKEN_LEFT_PARENTHESIS:
			//fprintf(yyout2, "Rule (STATEMENT_TAG_2 ->  VARIABLE_TAG = EXPRESSION )\n");	
			current_token=back_token();
			entry = find(current_table,current_token->lexeme);
			if(entry)
			{
				if((entry->value->kind) != IntegerArray && (entry->value->kind) != RealArray)
				{
					fprintf(yyout3,"\r\nERROR!!:can't insert to a array kind without using specific index id name:%s line %d is not as the id  kind\n",current_token->lexeme, current_token->lineNumber);
					
				}
				
					current_token=next_token();
					current_token=next_token();
					index=atoi(current_token->lexeme);
					current_token=back_token();
					semantic_match(TOKEN_INT_NUM);
					if((!match_worked) || ((entry->value->size) < index))
					{
						fprintf(yyout3,"\r\nERROR!!:can't insert to a array kind with index that bigger than the size of an array , id name:%s line %d is not as the id  kind\n",id_name, current_token->lineNumber);
					}
						match(TOKEN_RIGHT_PARENTHESIS);
						semantic_match(TOKEN_ASSIGMENT);
						entry->value->used=1;
						expressionKind = semantic_parse_EXPRESSION();
						if(*idkind == Real || *idkind == RealArray )
						{
							if(expressionKind != Real && expressionKind!=Integer)
							{
								fprintf(yyout3,"\r\nERROR!!:cant match between th expression kind and the id kind ,id name:%s  at line %d is not as the same as the expression kind\n", id_name, current_token->lineNumber);
							}
						}
						else
						{
							if(*idkind == IntegerArray || *idkind == Integer)
							{
								if(expressionKind != Integer)
								{
									fprintf(yyout3,"\r\nERROR!!:cant match between th expression kind and the id kind ,id name:%s at line %d is not the same as the expression kind\n", id_name, current_token->lineNumber);
								}
							}
						}

					

				
			}
			else
			{
				expressionKind=Error_type;
			}
		break;
		case TOKEN_ASSIGMENT:
			//fprintf(yyout2, "Rule (STATEMENT_TAG_2 ->  id = EXPRESSION )\n");
			entry=find(current_table,id_name);
			if(entry && entry->value->kind!=IntegerFunction && entry->value->kind!=RealFunction&& entry->value->kind!=Void)
			{
				entry->value->used=1;
				*idkind=entry->value->kind;
				expressionKind =  semantic_parse_EXPRESSION();
				if(*idkind==RealArray|| *idkind==IntegerArray)
				{
					fprintf(yyout3,"\r\nERROR!!:cant assign to an array without an index line number:%d\n",current_token->lineNumber);
					
				}
				else
				{
					if(*idkind != expressionKind )
					{
						if(*idkind != Real && expressionKind!=Integer)
							fprintf(yyout3,"\r\nERROR!!:the expression kind in the expression at line %d is not as the id  kind\n", current_token->lineNumber);
						
					}
				}
			}
			else
			{
				fprintf(yyout3,"\r\nERROR!!:the id %s at line %d is not declared as a identifier\n", id_name, current_token->lineNumber);
				expressionKind=Error_type;
			}
			semantic_parse_EXPRESSION();	
		break;
		case TOKEN_LEFT_BRACKET://to dooooo!!!!!!!
			//fprintf(yyout2, "Rule (STATEMENT_TAG_2 ->  FUNCTION_CALL) \n");	
			current_token=back_token();//
			current_token=back_token();//to return to id and parse FUNCTION_CALL
			semantic_parse_FUNCTION_CALL();
			break;
		default:
			errorHandling(eRule_STATEMENT_TAG_2);
	}
}
void semantic_parse_STATEMENT_TAG(eCommand *c,eKind *kind){
	// first { int_num , real_num , id } , follow { ';' }
	eKind exType;
	current_token = next_token();

	switch (current_token->kind) {
	case TOKEN_INT_NUM:
	case TOKEN_REAL_NUM:
	case TOKEN_ID:
		//fprintf(yyout2, "Rule (STATEMENT_TAG -> EXPRESSION )\n");	
		switch(*c)
		{
		case FunctionType:
			current_token=back_token();
			exType=semantic_parse_EXPRESSION();
			if(exType != current_table->returnKind)
			{
				fprintf(yyout3,"\r\nERROR!!:the function returned value is not matched with the expression type! line:%d \n",current_token->lineNumber);				
			}
			else
				if(exType == current_table->returnKind)
				{
					current_table->returnflag=1;
				}
			break;
		case RegularType:
			current_token=back_token();
			exType=semantic_parse_EXPRESSION();
			if(exType == current_table->returnKind)
			{
				current_table->returnflag=1;
			}	
			break;
		}
			
		break;
	case TOKEN_SEMICOLON:
		//fprintf(yyout2, "Rule (STATEMENT_TAG -> epsilon )\n");
		if(current_table->returnKind == Void)
		{
			current_table->returnflag=1;			
		}
		current_token=back_token();    //to check
		break;
	default:
		errorHandling(eRule_STATEMENT_TAG);
	}
}

void semantic_parse_BLOCK(eCommand *c,eKind *returnedType,id_list** parametersList){
	// first { '{' } , follow { ';',void, real ,integer,eof }
	id_list* lstOfIds;
	eKind kind; 
	eCommand lastblockc=*c;
	current_token = next_token();

	switch (current_token->kind) {
	case TOKEN_LEFT_CURLY_BRACE:
		//fprintf(yyout2, "Rule (BLOCK -> { VAR_DEFINITIONS; STATEMENTS })\n");
		parse_BB(returnedType);
		switch(*c)
		{
		case FunctionType:
			//to check
			if(*parametersList)
			{
				InsertlistToTable(current_table,*parametersList);
			}
			*c=RegularType;
			semantic_parse_VAR_DEFINITIONS(c);
			*c=lastblockc;
			break;
		case RegularType:
			*c=RegularType;
			semantic_parse_VAR_DEFINITIONS(c);
			*c=lastblockc;
			break;
		}
		semantic_match(TOKEN_SEMICOLON);
		semantic_parse_STATEMENTS(c,returnedType);
		semantic_match(TOKEN_RIGHT_CURLY_BRACE);
		if(*returnedType!=Void)
		{
					
			if(*returnedType==current_table->returnKind && current_table->returnflag)
			{
				current_table->returnflag=1;		

			}
			if(!current_table->returnflag)
			{
				fprintf(yyout3,"\r\nERROR!!:the function should return value line:%d\n", current_token->lineNumber);				
			}
		}
		else
		{
			current_table->returnflag=1;		
		}
		parse_FB();
		break;
	default:
		errorHandling(eRule_BLOCK);
	}
}

void semantic_parse_FUNCTION_CALL(){
	// first { id } , follow { ';'}
	id_list* listofIds;
	table_entry *entry=new_table_entry();
	int numOfParameters=0;
	char *id_name;
	int parameterListSize;
	eKind k=NULLKind;
	current_token = next_token();

	switch (current_token->kind) {
	case TOKEN_ID:
		//fprintf(yyout2, "Rule FUNCTION_CALL -> id (PARAMETERS_LIST))   \n");
		id_name = (char*)malloc(strlen(current_token->lexeme) + 1);
		strcpy(id_name, current_token->lexeme);
		semantic_match(TOKEN_LEFT_BRACKET);
		numOfParameters=semantic_parse_PARAMETERS_LIST();
		semantic_match(TOKEN_RIGHT_BRACKET);
		entry=find(current_table,id_name);
		if(entry && ((entry->value->kind!=IntegerFunction)||(entry->value->kind!=RealFunction)||entry->value->kind!=Void))
		{
			if(numOfParameters != entry->value->numOfFuncParameters )
			{
				fprintf(yyout3,"\r\nERROR!!:the function declared at line %d ,id name:%s number of parameters didnt match to func declaration number should be:%d\n", current_token->lineNumber, id_name,entry->value->numOfFuncParameters);		
			
			}
			else
			{
				if(numOfParameters!= 0 && entry->value->numOfFuncParameters != 0)
				{
					if(!checktwolistskind(listofparameters,entry->value->parameters_id))
					{
						fprintf(yyout3,"\r\nERROR!!:the function declared at line %d ,id name:%s parameters didnt match to func declaration \n", current_token->lineNumber, id_name,entry->value->numOfFuncParameters);		
					
					}
					free_listOfIds(listofparameters);
					listofparameters=NULL;
				}
			}
		}
		else
		{
			fprintf(yyout3,"\r\nERROR!!:the function declared at line %d ,id name:%s is not defined\n", current_token->lineNumber, id_name);		
		}
		break;
	default:
		errorHandling(eRule_FUNCTION_CALL);
	}
}

int semantic_parse_PARAMETERS_LIST(){
	// first { id } , follow { ')' }
	id_list* lst;
	int count;
	eCommand c=FunctionCallType;
	eKind kind = NULLKind;

	current_token = next_token();
	

	switch (current_token->kind) {
	case TOKEN_ID:
		//fprintf(yyout2, "Rule (PARAMETERS_LIST -> VARIABLES_LIST) \n");	
		current_token=back_token();
		count=semantic_parse_VARIABLES_LIST(&kind,&lst,&c);
		return count;
		/*return lst;*/
		break;
	case TOKEN_RIGHT_BRACKET:
		//fprintf(yyout2, "Rule PARAMETERS_LIST -> epsilon) \n");
		current_token=back_token();
		return 0;
		break;
	default:
		errorHandling(eRule_PARAMETERS_LIST);
	}
}




eKind semantic_parse_EXPRESSION(){
	// first { id, int-number, real-number } , follow { ';' }
	char *id_name;
	eKind extagkind;
	table_entry *entry = new_table_entry();
	eKind id_kind=NULLKind;
	current_token = next_token();

	switch (current_token->kind) {
	case TOKEN_REAL_NUM:
		//fprintf(yyout2, "Rule (EXPRESSION ->  real_number) \n");
		return Real;
		break;
	case TOKEN_INT_NUM:
		//fprintf(yyout2, "Rule (EXPRESSION -> int_number)  \n");
		return Integer;
		break;
	case TOKEN_ID:
		//fprintf(yyout2, "Rule (EXPRESSION -> id  EXPRESSION_TAG)\n");
		id_name = (char*)malloc(strlen(current_token->lexeme) + 1);
		strcpy(id_name, current_token->lexeme);
		entry=find(current_table,id_name);
		if(!entry)
		{
			fprintf(yyout3,"\r\nERROR!!:the identifier  at line %d ,id name:%s is not defined\n", current_token->lineNumber, current_token->lexeme);
			id_kind=Error_type;
			extagkind = semantic_parse_EXPRESSION_TAG(&id_kind);
			return Error_type;
			break;
		}
		else
		{
			id_kind=(entry->value->kind);
			extagkind = semantic_parse_EXPRESSION_TAG(&id_kind);
			if(id_kind == Real || extagkind == Real)
			{
				return Real;
			}
			else 
			{
				if((id_kind == Integer || id_kind == IntegerArray) && extagkind == Integer)
				{
					return Integer;
				}
				else
				{
					fprintf(yyout3,"\r\nERROR!!:the type of exprression is Error type line:%d\n", current_token->lineNumber);

					return Error_type;
				}
			}
		}
		break;
	default:
		errorHandling(eRule_EXPRESSION);
	}
}

eKind semantic_parse_EXPRESSION_TAG(eKind *lastElementKind)
{
	// first { '[', '*', '/'} , follow {  ';'} 
	table_entry* entry;
	char *id_name;
	int index;
	eKind nextElementkind;
	current_token = next_token();

	switch (current_token->kind) {
	case TOKEN_LEFT_PARENTHESIS:
		//fprintf(yyout2, "Rule (EXPRESSION_TAG -> VARIABLE_TAG)  \n");
		current_token=back_token();
		id_name = (char*)malloc(strlen(current_token->lexeme) + 1);
		strcpy(id_name, current_token->lexeme);
		entry=find(current_table,id_name);
		if(!entry)
		{
			
			match(TOKEN_LEFT_PARENTHESIS);
			semantic_match(TOKEN_INT_NUM);
			match(TOKEN_RIGHT_PARENTHESIS);
			fprintf(yyout3,"\r\nERROR!!:the identifier is not recognize line:%d idname:%s\n", current_token->lineNumber,id_name);
			return Error_type;
		}
		else
		{
			nextElementkind=entry->value->kind;
			match(TOKEN_LEFT_PARENTHESIS);
			semantic_match(TOKEN_INT_NUM);
			index=atoi(current_token->lexeme);
			if((!match_worked) || ((entry->value->size) < index))
			{
				fprintf(yyout3,"\r\nERROR!!:can't insert to a array kind with index that bigger than the size of an array , id name:%s line %d is not as the id  kind\n",id_name, current_token->lineNumber);
				break;
			}
			else
			{
				match(TOKEN_RIGHT_PARENTHESIS);
				if(*lastElementKind == Real)
				{
					return Real;
				}
				else
				{
					if(nextElementkind == Real||nextElementkind == RealArray)
						return Real;
					else 
						if(nextElementkind == Integer ||nextElementkind == IntegerArray)
							return Integer;
						else
						{
							return Error_type;
						}
				}
			}
		}
		break;
	case TOKEN_MULTIPLICATION:
	case TOKEN_DIVISION:
		//fprintf(yyout2, "Rule (EXPRESSION_TAG ->ar_op EXPRESSION)  \n");
		current_token=back_token();
		semantic_match_ar_op();
		nextElementkind= semantic_parse_EXPRESSION();
		if(*lastElementKind == Real)
		{
			return Real;
		}
		else
		{
			if(nextElementkind == Real)
				return Real;
			else 
				if(*lastElementKind != Error_type && nextElementkind != Error_type)
				{
					return Integer;
				}
				else
				{

					return Error_type;
				}
		}
	break;
	case TOKEN_SEMICOLON:
		current_token=back_token();
		id_name = (char*)malloc(strlen(current_token->lexeme) + 1);
		strcpy(id_name, current_token->lexeme);
		entry=find(current_table,id_name);
		if(!entry)
		{
			
			fprintf(yyout3,"\r\nERROR!!:the identifier is not recognize line:%d idname:%s\n", current_token->lineNumber,id_name);
			return Error_type;
		}
		else
		{
			if(*lastElementKind == Real||*lastElementKind == RealArray)
			{
				return Real;
			}
			else
			{
				if(*lastElementKind == Integer||*lastElementKind == IntegerArray)
					return Integer;
				else 
					return Error_type;
			}
		}
		break;
	default:
		errorHandling(eRule_EXPRESSION_TAG); 
	}
}

/////////////////////////////////////////		semantic_Error_handling		/////////////////////////////////////////
void semantic_errorHandling(eRule current_rule){

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
		findFollowingToken(followPROGRAM, 1);
		break;
	case eRule_VAR_DEFINITIONS:
		findFollowingToken(followVAR_DEFINITIONS, 2);
		break;
	case eRule_VAR_DEFINITIONS_TAG:
		findFollowingToken(followVAR_DEFINITIONS_TAG ,2);
		break;
	case eRule_VAR_DEFITION:
		findFollowingToken(followVAR_DEFINITION, 2);
		break;
	case eRule_TYPE:
		findFollowingToken(followTYPE, 1);
		break;
	case eRule_VARIABLES_LIST:
		findFollowingToken(followVARIABLES_LIST, 2);
		break;
	case eRule_VARIABLES_LIST_TAG:
		findFollowingToken(followVARIABLES_LIST_TAG, 2);
		break;
	case eRule_VARIABLE:
		findFollowingToken(followVARIABLE, 3);
		break;
	case eRule_VARIABLE_TAG:
		printError(current_token, firstVARIABLE_TAG, 1);
		findFollowingToken(followVARIABLE_TAG, 3);
		break;
	case eRule_FUNC_DEFINTIONS:
		findFollowingToken(followFUNC_DEFINITIONS, 1);
		break;
	case eRule_FUNC_DEFINTIONS_TAG:
		findFollowingToken(followFUNC_DEFINITIONS_TAG, 1);
		break;
	case eRule_FUNC_DEFINTION:
		findFollowingToken(followFUNC_DEFINITION, 4);
		break;
	case eRule_RETURNED_TYPE:
		findFollowingToken(followRETURNED_TYPE, 1);
		break;
	case eRule_PARAM_DEFINITIONS:
		findFollowingToken(followPARAM_DEFINITIONS, 1);
		break;
	case eRule_STATEMENTS:
		findFollowingToken(followSTATEMENTS, 2);
		break;
	case eRule_STATEMENTS_TAG:
		findFollowingToken(followSTATEMENTS_TAG, 2);
		break;
	case eRule_STATEMENT:
		findFollowingToken(followSTATEMENT, 1);
		break;
	case eRule_STATEMENT_TAG:
		findFollowingToken(followSTATEMENT_TAG, 1);
		break;
	case eRule_STATEMENT_TAG_2:
		findFollowingToken(followSTATEMENT_TAG_2, 1);
		break;
	case eRule_BLOCK:
		findFollowingToken(followBLOCK, 5);
		break;
	case eRule_FUNCTION_CALL:
		findFollowingToken(followFUNCTION_CALL, 1);
		break;
	case eRule_PARAMETERS_LIST:
		findFollowingToken(followPARAMETERS_LIST, 1);
		break;
	case eRule_EXPRESSION:
		findFollowingToken(followEXPRESSION, 1);
		break;
	case eRule_EXPRESSION_TAG:
		findFollowingToken(followEXPRESSIONS_TAG, 1);
		break;
	}

}



