%{
#include <stdio.h>	
#include <list>
#include <stdlib.h>
#include "symbol-table.hh"
Symbol_Table * gst = new Symbol_Table();
Symbol_Table * lst = new Symbol_Table();
int type;
int sc=0;
extern "C" void yyerror(string s);
extern int yylex(void);
extern int yylineno;
string present_function;

Symbol_Table_Entry & my_get_symbol_table_entry(string name){
	Procedure * t =  program_object.get_procedure_prototype(present_function);
	if(lst->variable_in_symbol_list_check(name) != 0) 
	return lst->get_symbol_table_entry(name);
	else if (t->variable_in_formal_list_check(name)!=0)
	return t->get_formal_param_entry(name);
	else if(gst->variable_in_symbol_list_check(name) != 0)
	return gst->get_symbol_table_entry(name);
}
bool Symbol_Table_Check (string name){
	// cout<<"Symbol_Table_Check0\n";
	Procedure * t =  program_object.get_procedure_prototype(present_function);
	// cout<<"Symbol_Table_Check1\n";
	// cout<<name<<"\n";
	// cout<<name<<" t  "<<t->variable_in_formal_list_check(name)<<"\n  lst";
	// cout<<lst->variable_in_symbol_list_check(name)<<" \n gst";
	// cout<<gst->variable_in_symbol_list_check(name)<<" \n";
	if(lst->variable_in_symbol_list_check(name) != 0||
	t->variable_in_formal_list_check(name)!=0||
	gst->variable_in_symbol_list_check(name) != 0)
	{
		// cout<<"Symbol_Table_Check2\n";
		return 1; 
	}
	else {
		// cout<<"Symbol_Table_Check3\n";
		return 0;
	}
}
%}

%union {
	int integer_value;
	double double_value;
	std::string * string_value;
	list<Ast *> * ast_list;
	Ast * ast;
	Sequence_Ast * sast;
	Symbol_Table * symbol_table;
	Symbol_Table_Entry * symbol_entry;
	Basic_Block * basic_block;
	Procedure * procedure;
	Table_Scope get_symbol_scope();
	Sequence_Ast * seq_ast;
	Data_Type data_type;
	Call_Ast * call_ast;
}

%token <integer_value> INTEGER_NUMBER
%token <double_value> DOUBLE_NUMBER
%token INTEGER
%token FLOAT
%token VOID
%token RETURN
%token <string_value> NAME
%token ASSIGN
%token PRINT

%token DO
%token WHILE
%token IF
%token ELSE

%token LESS_THAN
%token GREATER_THAN
%token LESS_THAN_EQUAL
%token GREATER_THAN_EQUAL
%token EQUAL
%token NOT_EQUAL
%token OR
%token AND
%token NOT



/*%type <basic_block> program*/

// %type <symbol_table> declaration_list
%type <procedure> procedure_definition
%type <procedure> function_header
%type <symbol_table> optional_parameter_decl
%type <symbol_table> parameter_decl
%type <symbol_table> optional_variable_declaration_list
%type <symbol_table> global_declaration
%type <symbol_table> type_decl
%type <symbol_table> variable_declaration_list
%type <symbol_table> variable_declaration
%type <symbol_table> declaration
%type <data_type> T
%type <symbol_table> variable_list
%type <ast_list> statement_list
%type <ast> optionalE
%type <ast> return_statement
%type <ast> print_statement
%type <ast> assignment_statement
%type <call_ast> function_call
%type <ast_list> optional_parameter_list
%type <ast_list> parameter_list
%type <ast> conditional_statement
%type <ast> statement


%type <ast> if_else_statement
%type <ast> if_else_with
%type <ast> if_else_without
%type <ast> other_if_stat
%type <sast> other_if_statement_list
%type <sast> other_if1 
%type <sast> if_statement_list

%type <ast> while_statement
%type <ast> while_with
%type <ast> while_without
%type <ast> other_while_statement
%type <sast> other_while_statement_list
%type <sast> other_while1
%type <sast> while_statement_list
	
%type <ast> do_while_statement
%type <ast> do_while_with
%type <ast> do_while_without
%type <ast> other_do_while_statement
%type <sast> other_do_while_statement_list
%type <sast> other_do1
%type <sast> do_while_statement_list

%type <ast> variable
%type <ast> E
// %type <ast> UOPT

%left '?' ':'
%left OR
%left AND
%left EQUAL NOT_EQUAL
%left LESS_THAN GREATER_THAN LESS_THAN_EQUAL GREATER_THAN_EQUAL 
%left '+' '-'
%left '*' '/'
%left UMINUS /* Negation--unary minus*/
%left NOT

%%
program: optional_global_declarations procedure_definitions
		// variable_declaration_list  procedure_definitions
				{	
					// gst = $1;
					// sc = 1;
					program_object.set_global_table(*gst);
					if(!program_object.is_procedure_exists("main")){
						cout<<"cs316: Error "<<"main not defined \n";
						exit(0);
					}
					// program_object.set_procedure($2, yylineno);

				}
				
			;
optional_global_declarations: /*empty*/ 
				|
				optional_global_declarations global_declaration
				;
global_declaration: variable_declaration{
					$$=$1;
					// cout<<"global_declaration\n";
					gst->append_list(*$1,yylineno);
					// cout<<"global_declaration\n";
				}
				| 
				T NAME '(' optional_parameter_decl ')'';'{
						Procedure *p = new Procedure($1,*$2,yylineno);/* ?($2) */				p->set_formal_param_list(*$4);
						program_object.set_proc_to_map(*$2,p);


				}
				|
				T NAME '(' type_decl ')'';'{
					
						Procedure *p = new Procedure($1,*$2,yylineno);/* ?($2) */				p->set_formal_param_list(*$4);	
						program_object.set_proc_to_map(*$2,p);			
				}

				;


type_decl: T {

				$$ = new Symbol_Table();
				$$->set_table_scope(formal);
				string s = "dummy";
				Symbol_Table_Entry * temp = new Symbol_Table_Entry(s, $1, yylineno);
				temp->set_symbol_scope(formal);
				$$->push_symbol(temp);
			}
			|
			type_decl','T{

				string s = "dummy";
				Symbol_Table_Entry * temp = new 
				Symbol_Table_Entry(s, $3, yylineno);
				temp->set_symbol_scope(formal);
				$1->push_symbol(temp);
				$$ = $1;

			}
		;
procedure_definitions: procedure_definitions procedure_definition {

				}
				| procedure_definition {

				}
				;


procedure_definition: 
					function_header
					optional_variable_declaration_list
					statement_list
						'}'
						   
					{
							//    $$ = new Procedure(void_data_type,*$2,yylineno);
							//    $$->set_ast_list(*$8);
							//    $$->set_local_list(*$7);
						Procedure * t =  program_object.get_procedure_prototype(present_function);
	
						// cout<<"procedure_definition\n";
						$$=$1;
						// cout<<"procedure_definition1\n";
						t->set_ast_list(*$3);
						// cout<<"procedure_definition2\n";
						t->set_local_list(*$2);
						// cout<<"procedure_definition3\n";
						// program_object.global_list_in_proc_map_check();
						
					}
					
					;
function_header: T  NAME '(' optional_parameter_decl ')' '{'{
		// cout<<"function_header enter\n";
		if (program_object.is_procedure_exists(*$2)){
			Procedure *t = program_object.get_procedure_prototype(*$2);
			if(t->is_proc_defined()){
				cout<<"cs316: Error "<<"Function declared more than once \n";
				exit(0);
			}
			present_function = t->get_proc_name();
			
			t->set_formal_param_list(*$4);
			// cout<<"function_header size pre " <<$4->get_size()<<"\n";
			// cout<<"function_header exit2\n";
			t->set_proc_is_defined();
			// cout<<"function_header size pre " <<$4->get_size()<<"\n";
			// cout<<"function_header exit3\n";
			// program_object.set_proc_to_map(*$2,$$);
			
		}
		else{
			sc=1;
			$$ = new Procedure($1,*$2,yylineno);/* ?($2) */
			
			present_function = *$2;
			
			$$->set_formal_param_list(*$4);
			// cout<<"function_header size " <<$4->get_size()<<"\n";
			// cout<<"function_header exit2\n";
			$$->set_proc_is_defined();
			// cout<<"function_header exit3\n";
			program_object.set_proc_to_map(*$2,$$);
			// cout<<"function_header exit4\n";
		}
	}
	;
optional_parameter_decl: /*empty*/{
								$$=new Symbol_Table();
								$$->set_table_scope(formal); 
								 /*??*/}
					| parameter_decl {$$ = $1;}
					;

parameter_decl: 		T NAME{
							$$ = new Symbol_Table();
							$$->set_table_scope(formal);
							Symbol_Table_Entry * temp = new Symbol_Table_Entry(*($2), $1, yylineno);
							temp->set_symbol_scope(formal);
							$$->push_symbol(temp);
							
						}
					|
						parameter_decl ',' T NAME
						{
							if($$->variable_in_formal_list_check(*$4)){
								cout<<"cs316: Error "<<"Variable declared already in param list \n";
								exit(0);
							}
							Symbol_Table_Entry * temp = new Symbol_Table_Entry(*($4), $3, yylineno);
							temp->set_symbol_scope(formal);
							$1->push_symbol(temp);
							$$ = $1;

						}
					;

optional_variable_declaration_list:	/* empty */
					{
						$$ = new Symbol_Table();
						lst = $$;
						// printf("No variable decls\n");
					}
					|	variable_declaration_list
					{
						// printf("variable decls\n");
						$$ = $1;lst=$$;
					}
					;

variable_declaration_list:	variable_declaration
						{
							$$ = new Symbol_Table();
							$$->append_list(*$1, yylineno);
							// if (sc==0){
							// 	gst = $$;
							// }
							// else{
							// 	printf("local variables decls\n %d",sc);
							// }
						}
					|	variable_declaration_list 
						variable_declaration
					{
						$1->append_list(*($2),yylineno);
						$$ = $1;
						// if (sc==0){
						// 	gst = $$;
						// 	printf("global variables decls\n");
						// }
						// else{
						// 	printf("local variables decls\n %d",sc);
						// }
					}
					;

variable_declaration:	declaration ';'
						{$$ = $1;}
					;

declaration:	T variable_list
					{$$ = $2;}
          ;

T:	VOID {$$= void_data_type;}
	|INTEGER{type = 0; $$= int_data_type;}
	|	FLOAT{type = 1;$$ = double_data_type;}
	;
variable_list:       NAME
					{	if (type == 0){
							$$ = new Symbol_Table();
							$$->push_symbol(new Symbol_Table_Entry(*($1), int_data_type, yylineno));
						}
						else {
							$$ = new Symbol_Table();
							$$->push_symbol(new Symbol_Table_Entry(*($1), double_data_type, yylineno));
						}
						if(sc == 0){$$->set_table_scope(global);}
						else {$$->set_table_scope(local);}
					}
        | variable_list ',' NAME
				   {
					   if (type == 0){
							$1->push_symbol(new Symbol_Table_Entry(*($3), int_data_type, yylineno));
						}
						else {
							$1->push_symbol(new Symbol_Table_Entry(*($3), double_data_type, yylineno));
						}
						// printf("typpppe %d\n",type);
						$$ = $1;
				   }
                   ;                

statement_list:	/* empty */
					{ 	$$ = new list<Ast *>();}
				|	
				statement_list 
				statement 
				{ 
					$1->push_back($2);
					$$=$1;
					// printf("Local assign stats\n");
				}
			
				;
statement: assignment_statement	{$$=$1;}		
						| if_else_statement			{$$=$1;}
						| while_statement				{$$=$1;}
						| do_while_statement		{$$=$1;}
						| print_statement	{$$=$1;}
						| function_call  {$$ = $1;}
						| return_statement {$$=$1;}
						;	
return_statement:      RETURN optionalE ';'{$$ = new Return_Ast($2,present_function,yylineno);
							Procedure * t =  program_object.get_procedure_prototype(present_function);

						
							$2==NULL?$$->set_data_type(void_data_type):$$->set_data_type($2->get_data_type());
							// cout<<"//////////////////////////////////data_type "<<$$->get_data_type()<<"\n";
							if(t->get_return_type()!=$$->get_data_type()){
								cout<<"cs316"<<":"<<" Error Return Type Mismatch\n";
								exit(0);
											
							}
						}
						;	
optionalE: /* empty */ {$$=NULL;}
			| E {$$= $1;}
			;
print_statement: PRINT NAME ';'
									{	
										if(Symbol_Table_Check(*$2) == 0){
											cout<<"cs316"<<":"<<" Error Not in Symbol_Table E\n";
											exit(0);
										}
										$$ = new Print_Ast(new Name_Ast(*$2,my_get_symbol_table_entry(*$2),yylineno), yylineno);
										// $$->set_data_type($1->get_data_type());
									}
								;
assignment_statement:	variable ASSIGN E ';' {
												$$ = new Assignment_Ast($1, $3, yylineno);
												$$->set_data_type($1->get_data_type()); 
												$$->check_ast();
												}
											| variable ASSIGN function_call
											{	Register_Descriptor *temp = new Register_Descriptor();
												$3->set_register(temp);
												$$ = new Assignment_Ast($1,$3,yylineno);
												$$->set_data_type($1->get_data_type());
												$$->check_ast();
												
											}
												
							;

function_call: NAME '(' optional_parameter_list ')'';'{
				// cout<<"enter call_ast\n";
				$$ = new Call_Ast(*$1,yylineno);
				// cout<<"enter call_ast1 \n";
				Procedure * t =  program_object.get_procedure_prototype(*$1);
				// cout<<"enter call_ast2 \n";
				$$->set_data_type(t->get_return_type());
				// cout<<"enter call_ast3 \n";
				$$->set_actual_param_list(*$3);
				// cout<<"enter call_ast4 \n";
				$$->check_actual_formal_param(t->get_formal_param_list());
				// cout<<"enter call_ast5\n";

				
				
			}
			;

optional_parameter_list: /*empty*/{$$=new list<Ast *>();}
					| parameter_list {$$ = $1;}
					;

parameter_list: 	 E{
						$$ = new list<Ast *>();
						$$->push_back($1);
						// cout<<"exit parameter_list1\n";
					}
					|	parameter_list ',' E 
					{
							$1->push_back($3);
							// cout<<"exit parameter_list2\n";
					}
					;


E: 	INTEGER_NUMBER {$$ = new Number_Ast<int >($1, int_data_type, yylineno);  }
				|	DOUBLE_NUMBER {$$ = new Number_Ast<double >($1,double_data_type, yylineno);}
				|	NAME	{
					if(Symbol_Table_Check(*$1) == 0){
						cout<<"cs316"<<":"<<" Error Not in Symbol_Table E\n";
						exit(0);
					}
					else {$$ = new Name_Ast(*$1,my_get_symbol_table_entry(*$1),yylineno);}
					}
				|   E '+' E {$$ = new Plus_Ast($1,$3,yylineno);$$->set_data_type($3->get_data_type());$$->check_ast();}
				|   E '-' E {$$ = new Minus_Ast($1,$3,yylineno);$$->set_data_type($3->get_data_type());$$->check_ast();}
				|	E '/' E {$$ = new Divide_Ast($1,$3,yylineno);$$->set_data_type($3->get_data_type());$$->check_ast();}
				|	'-' E  %prec UMINUS {$$ = new UMinus_Ast($2,NULL,yylineno);$$->set_data_type($2->get_data_type());$$->check_ast();}
				| '(' E ')' {$$ = $2;}
				|   E '*' E {$$ = new Mult_Ast($1,$3,yylineno);$$->set_data_type($3->get_data_type());$$->check_ast();}
				| conditional_statement '?' E ':' E 
				 {$$ = new Conditional_Expression_Ast($1, $3, $5, yylineno);
					$$->set_data_type($3->get_data_type());
					$$->check_ast();}
			;

// UOPT :	INTEGER_NUMBER {$$ = new Number_Ast<int >($1,int_data_type, yylineno);   }
// 		|	DOUBLE_NUMBER {$$ = new Number_Ast<double >($1,double_data_type, yylineno);}
// 		|	NAME	{
// 			if(Symbol_Table_Check(*$1) == 0){
// 						cout<<"cs316"<<":"<<" Error Not in Symbol_Table UOPT\n";
// 						exit(0);
// 					}
// 			else $$ = new Name_Ast(*$1,my_get_symbol_table_entry(*$1),yylineno); }
// 		|'(' E ')'{$$=$2;}
				 

variable:	NAME {
	if(Symbol_Table_Check(*$1) == 0){
						cout<<"cs316"<<":"<<" Error Not in Symbol_Table variable\n";
						exit(0);
	}
	else $$ = new Name_Ast(*$1,my_get_symbol_table_entry(*$1),yylineno);
	}
			;


conditional_statement:  NOT conditional_statement
						{ $$ = new Logical_Expr_Ast(NULL, _logical_not,$2 , yylineno);
						  $$->set_data_type($2->get_data_type()); 
							$$->check_ast();}

						| conditional_statement AND conditional_statement
						{ $$ = new Logical_Expr_Ast($1, _logical_and, $3, yylineno);
						  $$->set_data_type($1->get_data_type()); 
							$$->check_ast();}

						| conditional_statement OR conditional_statement
						{ $$ = new Logical_Expr_Ast($1, _logical_or, $3, yylineno);
						  $$->set_data_type($1->get_data_type()); $$->check_ast();}

						| E EQUAL E
						{ $$ = new Relational_Expr_Ast($1, equalto, $3, yylineno);
						  $$->set_data_type($1->get_data_type()); $$->check_ast();}

						| E NOT_EQUAL E
						{ $$ = new Relational_Expr_Ast($1, not_equalto, $3, yylineno);
						  $$->set_data_type($1->get_data_type()); $$->check_ast();}

						| E LESS_THAN E
						{ $$ = new Relational_Expr_Ast($1, less_than, $3, yylineno);
						  $$->set_data_type($1->get_data_type());
							$$->check_ast();}

						| E GREATER_THAN E
						{ $$ = new Relational_Expr_Ast($1, greater_than, $3, yylineno);
						  $$->set_data_type($1->get_data_type()); $$->check_ast();}

						| E LESS_THAN_EQUAL E
						{ $$ = new Relational_Expr_Ast($1, less_equalto, $3, yylineno);
						  $$->set_data_type($1->get_data_type()); $$->check_ast();}

						| E GREATER_THAN_EQUAL E
						{ $$ = new Relational_Expr_Ast($1, greater_equalto, $3, yylineno);
						  $$->set_data_type($1->get_data_type()); $$->check_ast();}
						| '(' conditional_statement ')' 
						{$$ = $2;
						}
						;
						
if_else_statement: if_else_with{$$=$1;}						
										| if_else_without{$$=$1;}										
							;
other_if_stat: while_statement{$$=$1;}
			| do_while_statement{$$=$1;}
			| assignment_statement{$$=$1;}
			| print_statement{$$ = $1;}
			| return_statement{$$=$1;}
			| function_call{$$=$1;}
			;

other_if_statement_list: 	other_if_statement_list other_if_stat
													{ 
													//Sequence_Ast *haha = new Sequence_Ast(yylineno);
														//haha->ast_push_back($1);
														//haha->ast_push_back($2);
														$$ = $1;
														$$->ast_push_back($2);
														//$$ = haha;
													}
													|other_if_stat{ $$ = new Sequence_Ast(yylineno);
													$$->ast_push_back($1);
													} 
													;


other_if1: 	other_if_statement_list{$$ = $1;}
			| {$$ = new Sequence_Ast(yylineno);}
			;
if_statement_list: if_statement_list if_else_statement other_if1 
										{ //Sequence_Ast *haha = new Sequence_Ast(yylineno);
											//haha->ast_push_back($1);
											//haha->ast_push_back($2);
											//haha->ast_push_back($3);
											$$ = $1;
											$$->ast_push_back($2);
											$$->ast_push_back($3);
											//$$ = haha;
											}
										| other_if1 if_else_statement other_if1 
										{ //Sequence_Ast *haha = new Sequence_Ast(yylineno);
											//haha->ast_push_back($1);
											//haha->ast_push_back($2);
											//haha->ast_push_back($3);
											$$ = $1;
											$$->ast_push_back($2);
											$$->ast_push_back($3);}
								;

if_else_with: IF '(' conditional_statement ')' '{' if_statement_list '}' ELSE '{' if_statement_list '}'
							{ $$ = new Selection_Statement_Ast($3, $6, $10, yylineno);
								$$->set_data_type($3->get_data_type());$$->check_ast();}

							| IF '(' conditional_statement ')' '{' other_if_statement_list '}' ELSE '{' if_statement_list '}'
							{ $$ = new Selection_Statement_Ast($3, $6, $10, yylineno);
								$$->set_data_type($3->get_data_type());$$->check_ast();}

							| IF '(' conditional_statement ')' '{' if_statement_list '}' ELSE '{' other_if_statement_list '}'
							{ $$ = new Selection_Statement_Ast($3, $6, $10, yylineno);
								$$->set_data_type($3->get_data_type());$$->check_ast();}

							| IF '(' conditional_statement ')' '{' if_statement_list '}'
							{ $$ = new Selection_Statement_Ast($3, $6, NULL, yylineno);
								$$->set_data_type($3->get_data_type());$$->check_ast();}

							| IF '(' conditional_statement ')' '{' other_if_statement_list '}' ELSE '{' other_if_statement_list '}'
							{ $$ = new Selection_Statement_Ast($3, $6, $10, yylineno);
								$$->set_data_type($3->get_data_type());$$->check_ast();}

							| IF '(' conditional_statement ')' '{' other_if_statement_list '}'
							{ $$ = new Selection_Statement_Ast($3, $6, NULL, yylineno);
								$$->set_data_type($3->get_data_type());$$->check_ast();}
							;
if_else_without: IF '(' conditional_statement ')' if_else_statement ELSE if_else_statement
									{ $$ = new Selection_Statement_Ast($3, $5, $7, yylineno);
										$$->set_data_type($3->get_data_type());$$->check_ast();}

									| IF '(' conditional_statement ')' if_else_statement ELSE '{' other_if_statement_list '}'
									{ $$ = new Selection_Statement_Ast($3, $5, $8, yylineno);
										$$->set_data_type($3->get_data_type());$$->check_ast();}

									| IF '(' conditional_statement ')' if_else_statement ELSE '{' if_statement_list '}'
									{ $$ = new Selection_Statement_Ast($3, $5, $8, yylineno);
										$$->set_data_type($3->get_data_type());$$->check_ast();}

									| IF '(' conditional_statement ')' '{' other_if_statement_list '}' ELSE if_else_statement
									{ $$ = new Selection_Statement_Ast($3, $6, $9, yylineno);
										$$->set_data_type($3->get_data_type());$$->check_ast();}

									| IF '(' conditional_statement ')' '{' if_statement_list '}' ELSE if_else_statement
									{ $$ = new Selection_Statement_Ast($3, $6, $9, yylineno);
										$$->set_data_type($3->get_data_type());$$->check_ast();}

									| IF '(' conditional_statement ')' if_else_statement
									{ $$ = new Selection_Statement_Ast($3, $5, NULL, yylineno);
										$$->set_data_type($3->get_data_type());$$->check_ast();}

									| IF '(' conditional_statement ')' other_if_stat
									{ $$ = new Selection_Statement_Ast($3, $5, NULL, yylineno);
										$$->set_data_type($3->get_data_type());$$->check_ast();}

									| IF '(' conditional_statement ')' other_if_stat ELSE other_if_stat
									{ $$ = new Selection_Statement_Ast($3, $5, $7, yylineno);
										$$->set_data_type($3->get_data_type());$$->check_ast();}

									| IF '(' conditional_statement ')' other_if_stat ELSE if_else_statement
									{	$$ = new Selection_Statement_Ast($3, $5, $7, yylineno);
										$$->set_data_type($3->get_data_type());$$->check_ast();
									}

									| IF '(' conditional_statement ')' if_else_statement ELSE other_if_stat
									{	$$ = new Selection_Statement_Ast($3, $5, $7, yylineno);
										$$->set_data_type($3->get_data_type());$$->check_ast();
									}

									| IF '(' conditional_statement ')' '{' other_if_statement_list '}' ELSE other_if_stat
									{ $$ = new Selection_Statement_Ast($3, $6, $9, yylineno);
										$$->set_data_type($3->get_data_type());$$->check_ast();}

									| IF '(' conditional_statement ')' '{' if_statement_list '}' ELSE other_if_stat
									{ $$ = new Selection_Statement_Ast($3, $6, $9, yylineno);
										$$->set_data_type($3->get_data_type());$$->check_ast();}

									| IF '(' conditional_statement ')' other_if_stat ELSE '{' other_if_statement_list '}'
									{ $$ = new Selection_Statement_Ast($3, $5, $8, yylineno);
										$$->set_data_type($3->get_data_type());$$->check_ast();}

									| IF '(' conditional_statement ')' other_if_stat ELSE '{' if_statement_list '}'
									{ $$ = new Selection_Statement_Ast($3, $5, $8, yylineno);
										$$->set_data_type($3->get_data_type());$$->check_ast();}
									
									;

while_statement: while_with{$$=$1;}
									| while_without{$$=$1;}
								;
other_while_statement: if_else_statement{$$=$1;}
												| do_while_statement{$$=$1;}
												| assignment_statement{$$=$1;}
												| print_statement{$$ = $1;}
												| function_call{$$=$1;}
												| return_statement{$$=$1;}
											;
other_while_statement_list: other_while_statement_list other_while_statement
														{ //Sequence_Ast *haha = new Sequence_Ast(yylineno);
															//haha->ast_push_back($1);
															//haha->ast_push_back($2);
															//$$ = haha;
															$$ = $1;
															$$->ast_push_back($2);}
														|other_while_statement{ $$ = new Sequence_Ast(yylineno);
														$$->ast_push_back($1);
														}
												;


other_while1: 	other_while_statement_list{$$ = $1;}
				| 	{$$ = new Sequence_Ast(yylineno);}
				;
while_statement_list: while_statement_list while_statement other_while1
											{ //Sequence_Ast *haha = new Sequence_Ast(yylineno);
												//haha->ast_push_back($1);
												//haha->ast_push_back($2);
												//haha->ast_push_back($3);
												//$$ = haha;
												$$ = $1;
												$$->ast_push_back($2);
												$$->ast_push_back($3);}
											| other_while1 while_statement other_while1
											{ //Sequence_Ast *haha = new Sequence_Ast(yylineno);
												//haha->ast_push_back($1);
												//haha->ast_push_back($2);
												//haha->ast_push_back($3);
												//$$ = haha;
												$$ = $1;
												$$->ast_push_back($2);
												$$->ast_push_back($3);}
											;
while_with: WHILE '(' conditional_statement ')' '{' while_statement_list '}'
						{ $$ = new Iteration_Statement_Ast($3, $6, yylineno, 0);
							$$->set_data_type($3->get_data_type()); $$->check_ast();}

						| WHILE '(' conditional_statement ')' '{' other_while_statement_list '}'
						{ $$ = new Iteration_Statement_Ast($3, $6, yylineno, 0);
							$$->set_data_type($3->get_data_type()); $$->check_ast();}
						;
while_without: WHILE '(' conditional_statement ')' while_statement
								{ $$ = new Iteration_Statement_Ast($3, $5, yylineno, 0);
									$$->set_data_type($3->get_data_type()); $$->check_ast();}
								| WHILE '(' conditional_statement ')' other_while_statement
								{ $$ = new Iteration_Statement_Ast($3, $5, yylineno, 0);
									$$->set_data_type($3->get_data_type()); $$->check_ast();}
							;
do_while_statement: do_while_with {$$=$1;}
										| do_while_without {$$=$1;}
								;
other_do_while_statement: if_else_statement {$$=$1;}
													| while_statement {$$=$1;}
													| assignment_statement {$$=$1;}
													| print_statement{$$ = $1;}
													| return_statement{$$=$1;}
													| function_call{$$=$1;}
											;
other_do_while_statement_list: other_do_while_statement_list   other_do_while_statement 
															{ //Sequence_Ast *haha = new Sequence_Ast(yylineno);
															//haha->ast_push_back($1);
															//haha->ast_push_back($2);
															//$$ = haha;
															$$ = $1;
															$$->ast_push_back($2);}
															| other_do_while_statement
															{ $$ = new Sequence_Ast(yylineno);
														$$->ast_push_back($1);} 
											;
other_do1: 	other_do_while_statement_list{$$ = $1;}
			| {$$ = new Sequence_Ast(yylineno);}
			;
do_while_statement_list: do_while_statement_list do_while_statement other_do1
													{ //Sequence_Ast *haha = new Sequence_Ast(yylineno);
												//haha->ast_push_back($1);
												//haha->ast_push_back($2);
												//haha->ast_push_back($3);
												//$$ = haha;
												$$ = $1;
												$$->ast_push_back($2);
												$$->ast_push_back($3);}
													| other_do1 do_while_statement other_do1
														{ //Sequence_Ast *haha = new Sequence_Ast(yylineno);
												//haha->ast_push_back($1);
												//haha->ast_push_back($2);
												//haha->ast_push_back($3);
												//$$ = haha;
												$$ = $1;
												$$->ast_push_back($2);
												$$->ast_push_back($3);}	
												;
do_while_with: DO '{' do_while_statement_list '}' WHILE '(' conditional_statement ')'';'
								{ $$ = new Iteration_Statement_Ast($7, $3, yylineno, 1);
									$$->set_data_type($3->get_data_type()); $$->check_ast();}

								| DO '{' other_do_while_statement_list '}' WHILE '('conditional_statement ')' ';'
								{ $$ = new Iteration_Statement_Ast($7, $3, yylineno, 1);
									$$->set_data_type($3->get_data_type()); $$->check_ast();
									}
								;
do_while_without: DO do_while_statement WHILE '(' conditional_statement ')' ';'
									{ $$ = new Iteration_Statement_Ast($5, $2, yylineno, 1);
										$$->set_data_type($5->get_data_type()); $$->check_ast();}

									| DO other_do_while_statement WHILE '(' conditional_statement ')' ';'
									{ $$ = new Iteration_Statement_Ast($5, $2, yylineno, 1);
										$$->set_data_type($5->get_data_type()); $$->check_ast();}
								;
%%

/*
TODO-
1. NOT
2. CHECK AST
3. DIVIDE BY 0
4. Deletion of unused eval_results
5. name string in nameast
6.
*/