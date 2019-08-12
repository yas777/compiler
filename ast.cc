#include "ast.hh"
#include <iostream>
#include <stdlib.h>
#include "icode.hh"
#include "symbol-table.hh"
#include "reg-alloc.hh"

using namespace std;

template class Number_Ast<double>;
template class Number_Ast<int>;

Ast::Ast(){
	// labelCounter = 0;
}

Ast::~Ast(){

}
int Ast::labelCounter=0;
Data_Type Ast::get_data_type(){
	return node_data_type;
}

void Ast::set_data_type(Data_Type dt){
	node_data_type = dt;
}

bool Ast::is_value_zero(){
	return 0;
}

bool Ast::check_ast(){
	return 1;
}

Symbol_Table_Entry & Ast::get_symbol_entry(){
	Symbol_Table_Entry * s = new Symbol_Table_Entry();
	return *s;
}

void Ast::print(ostream & file_buffer){

}
// void Ast::labelCounter(){

// }
// void Ast::ast_push_back(Ast *ast){

// }

////////////////////////////////////////
Assignment_Ast::Assignment_Ast(Ast * temp_lhs, Ast * temp_rhs, int line){
	ast_num_child = binary_arity;
	lhs = temp_lhs;
	rhs = temp_rhs;
	lineno = line;
}

Assignment_Ast::~Assignment_Ast(){
	// lhs = NULL;
	// rhs = NULL;
	delete lhs;
	delete rhs;
}

bool Assignment_Ast::check_ast(){
	if (lhs->get_data_type() == rhs->get_data_type()){
		return 1;
	}
	else{
		cout<<"cs316"<<":"<<" Error Assignment statement data type not compatible\n";
		exit(0);
	}
	// return ();
	// return 1;
}

void Assignment_Ast::print(ostream & file_buffer){
	file_buffer<<"\n         Asgn:\n";
	file_buffer<<"            LHS (";
	lhs->print(file_buffer);
	file_buffer<<")\n";
	file_buffer<<"            RHS (";
	rhs->print(file_buffer);
	file_buffer<<")";
}
////////////////////////////////Print_Ast
Print_Ast::Print_Ast(Ast *v, int line){
	var = v;
	lineno = line;
}
Print_Ast::~Print_Ast(){

}
void Print_Ast::print(ostream & file_buffer){
	var->print(file_buffer);
}
////////////////////////////////
Name_Ast::Name_Ast(string & name, Symbol_Table_Entry & var_entry, int line){
	ast_num_child = zero_arity;
	variable_symbol_entry = &var_entry;
	lineno = line;
	node_data_type = variable_symbol_entry->get_data_type();
}

Name_Ast::~Name_Ast(){
	
}

Data_Type Name_Ast::get_data_type(){
	return node_data_type;
}

Symbol_Table_Entry & Name_Ast::get_symbol_entry(){
	return *variable_symbol_entry;
}

void Name_Ast::set_data_type(Data_Type dt){
	node_data_type = dt;
}

void Name_Ast::print(ostream & file_buffer){
	file_buffer<<"Name : "<<variable_symbol_entry->get_variable_name();
}

//////////////////////////////////////////
template <class T>
Number_Ast<T>::Number_Ast(T number, Data_Type constant_data_type, int line){
	ast_num_child = zero_arity;
	constant = number;
	node_data_type = constant_data_type;
	lineno = line;
}

template <class T>
Number_Ast<T>::~Number_Ast(){

}

template <class T>
Data_Type Number_Ast<T>::get_data_type(){
	return node_data_type;
}

template <class T>
void Number_Ast<T>::set_data_type(Data_Type dt){
	node_data_type = dt;
}

template <class T>
void Number_Ast<T>::print(ostream & file_buffer){
	file_buffer<<"Num : "<<constant;
}

template <class T>
bool Number_Ast<T>::is_value_zero(){
	return 0;
}
/////////////////////////////////////
Data_Type Arithmetic_Expr_Ast::get_data_type(){
	return node_data_type;
}

void Arithmetic_Expr_Ast::set_data_type(Data_Type dt){
	node_data_type = dt;
}

bool Arithmetic_Expr_Ast::check_ast(){
	if (ast_num_child == binary_arity ){
		if(lhs->get_data_type() == rhs->get_data_type()){
			return 1;
		}
		else{
			cout<<"cs316"<<":"<<" Error Arithematic statement data type not compatible\n";
			exit(0);
		}
	}
    else if(ast_num_child==ternary_arity)
    {
		if(lhs->get_data_type() == rhs->get_data_type()){
			return 1;
		}
		else{
			cout<<"cs316"<<":"<<" Error Ternary Arith statement data type not compatible\n";
			exit(0);
		}
    }
	else if(ast_num_child==unary_arity)
    {

    }
    
	else{
		cout<<"Arity Mismatch 2,3 at line: "<<lineno<<" \n";
		return 1;
	} 
}

void Arithmetic_Expr_Ast::print(ostream & file_buffer){

}
/////////////////////////////////////
Plus_Ast::Plus_Ast(Ast * l, Ast * r, int line){
	ast_num_child = binary_arity;
	lhs = l;
	rhs = r;
	lineno = line;
}

void Plus_Ast::print(ostream & file_buffer){
	file_buffer<<"\n            Arith: PLUS\n";
	file_buffer<<"               LHS (";
	lhs->print(file_buffer);
	file_buffer<<")\n";
	file_buffer<<"               RHS (";
	rhs->print(file_buffer);
	file_buffer<<")";
}

////////////////////////////////////
Minus_Ast::Minus_Ast(Ast * l, Ast * r, int line){
	ast_num_child = binary_arity;
	lhs = l;
	rhs = r;
	lineno = line;
}

void Minus_Ast::print(ostream & file_buffer){
	file_buffer<<"\n            Arith: MINUS\n";
	file_buffer<<"               LHS (";
	lhs->print(file_buffer);
	file_buffer<<")\n";
	file_buffer<<"               RHS (";
	rhs->print(file_buffer);
	file_buffer<<")";
}

/////////////////////////////////////
Divide_Ast::Divide_Ast(Ast * l, Ast * r, int line){
	ast_num_child = binary_arity;
	lhs = l;
	rhs = r;
	lineno = line;
}

void Divide_Ast::print(ostream & file_buffer){
	file_buffer<<"\n            Arith: DIV\n";
	file_buffer<<"               LHS (";
	lhs->print(file_buffer);
	file_buffer<<")\n";
	file_buffer<<"               RHS (";
	rhs->print(file_buffer);
	file_buffer<<")";
}

/////////////////////////////////////////
Mult_Ast::Mult_Ast(Ast * l, Ast * r, int line){
	ast_num_child = binary_arity;
	lhs = l;
	rhs = r;
	lineno = line;
}

void Mult_Ast::print(ostream & file_buffer){
	file_buffer<<"\n            Arith: MULT\n";
	file_buffer<<"               LHS (";
	lhs->print(file_buffer);
	file_buffer<<")\n";
	file_buffer<<"               RHS (";
	rhs->print(file_buffer);
	file_buffer<<")";
}

///////////////////////////////////////////
UMinus_Ast::UMinus_Ast(Ast * l, Ast * r, int line){
	ast_num_child = unary_arity;
	lhs = l;
	rhs = r;
	lineno = line;
}

void UMinus_Ast::print(ostream & file_buffer){
	file_buffer<<"\n            Arith: UMINUS\n";
	file_buffer<<"               LHS (";
	lhs->print(file_buffer);
	file_buffer<<")";
}

///////////////////////////////////////////
Conditional_Expression_Ast::Conditional_Expression_Ast(Ast *c, Ast *l, Ast* r, int line){
    ast_num_child = ternary_arity;
    lhs = l;
    rhs = r;
    cond = c;
    lineno = line;
}
Conditional_Expression_Ast::~Conditional_Expression_Ast(){

}
void Conditional_Expression_Ast::print(ostream & file_buffer){
	file_buffer<<"\n"<<AST_SPACE<<"Cond:\n";
	file_buffer<<AST_NODE_SPACE<<"IF_ELSE";
	cond->print(file_buffer);
	file_buffer<<"\n";
	file_buffer<<AST_NODE_SPACE<<"LHS (";
	lhs->print(file_buffer);
	file_buffer<<")\n";
	file_buffer<<AST_NODE_SPACE<<"RHS (";
	rhs->print(file_buffer);
	file_buffer<<")";	

	
} 
// change check ast
/////////////////////////////////////////////
Relational_Expr_Ast::Relational_Expr_Ast(Ast *l,Relational_Op rop,Ast *r,int line ){
	ast_num_child = binary_arity;
    lhs_condition = l;
    rhs_condition = r;
    rel_op = rop;
    lineno = line;
}

Relational_Expr_Ast::~Relational_Expr_Ast(){

}

Data_Type Relational_Expr_Ast::get_data_type(){
    return node_data_type;
}
void Relational_Expr_Ast::set_data_type(Data_Type dt){
    node_data_type = dt;
}

bool Relational_Expr_Ast::check_ast(){
	if (lhs_condition->get_data_type() == rhs_condition->get_data_type()){
		return 1;
	}
	else{
		cout<<"cs316"<<":"<<" Error Relational_Expr statement data type not compatible\n";
		exit(0);
	}
    return 1;
}

// typedef enum
// {
// 	less_equalto,
// 	less_than,
// 	greater_than,
// 	greater_equalto,
// 	equalto,
// 	not_equalto
// } Relational_Op;


// typedef enum
// {
// 	_logical_not,
// 	_logical_or,
// 	_logical_and
// } Logical_Op;

void Relational_Expr_Ast::print(ostream & file_buffer){
	file_buffer<<"\n"<<AST_NODE_SPACE<<"Condition: ";
	if(rel_op==less_equalto)file_buffer<<"LE";
	else if(rel_op==less_than)file_buffer<<"LT";
	else if(rel_op==greater_than)file_buffer<<"GT";
	else if(rel_op==greater_equalto)file_buffer<<"GT";
	else if(rel_op==equalto)file_buffer<<"EQ";
	else if(rel_op==not_equalto)file_buffer<<"NE";
	
	
	file_buffer<<"\n";
	file_buffer<<AST_SUB_NODE_SPACE<<"LHS (";
	lhs_condition->print(file_buffer);
	file_buffer<<")\n";
	file_buffer<<AST_SUB_NODE_SPACE<<"RHS (";
	rhs_condition->print(file_buffer);
	file_buffer<<")";
}
///////////////////////////////////////////////////
Logical_Expr_Ast::Logical_Expr_Ast(Ast * lhs, Logical_Op bop, Ast * rhs, int line){
	if(bop==_logical_not)ast_num_child = unary_arity;
    else ast_num_child = binary_arity;
	lhs_op = lhs;
    rhs_op = rhs;
    bool_op = bop;
	lineno = line;

}
Logical_Expr_Ast::~Logical_Expr_Ast(){

}

Data_Type Logical_Expr_Ast::get_data_type(){
    return node_data_type;
}
void Logical_Expr_Ast::set_data_type(Data_Type dt){
    node_data_type =dt;
}

bool Logical_Expr_Ast::check_ast(){
	if(ast_num_child==unary_arity)return 1;
	if (lhs_op->get_data_type() == rhs_op->get_data_type()){
		return 1;
	}
	else{
		cout<<"cs316"<<":"<<" Error Logical_Expr statement data type not compatible\n";
		exit(0);
	}
    return 1;
}

void Logical_Expr_Ast::print(ostream & file_buffer){
	file_buffer<<"\n"<<AST_NODE_SPACE<<"Condition: ";
	if(bool_op == _logical_not)file_buffer<<"NOT";
	else if(bool_op ==_logical_or)file_buffer<<"OR";
	else if(bool_op ==_logical_and)file_buffer<<"AND";


	file_buffer<<"\n";

	if(bool_op !=_logical_not){
		file_buffer<<AST_SUB_NODE_SPACE<<"LHS (";
		lhs_op->print(file_buffer);
		file_buffer<<")\n";
	}
	file_buffer<<AST_SUB_NODE_SPACE<<"RHS (";
	rhs_op->print(file_buffer);
	// }
	file_buffer<<")";
}
///////////////////////////////////////////////////////////
Selection_Statement_Ast::Selection_Statement_Ast(Ast * c,Ast* then_p, Ast* else_p, int line){
	if(else_p==NULL)ast_num_child =unary_arity;
	else ast_num_child=binary_arity; 
    cond = c;
    then_part = then_p;
    else_part = else_p;
    line = lineno;

}
Selection_Statement_Ast::~Selection_Statement_Ast(){

}
Data_Type Selection_Statement_Ast::get_data_type(){
    return node_data_type;
}
void Selection_Statement_Ast::set_data_type(Data_Type dt){
    node_data_type = dt;
}
bool Selection_Statement_Ast::check_ast(){
	// if (lhs->get_data_type() == rhs->get_data_type()){
	// 	return 1;
	// }
	// else{
	// 	cout<<"cs316"<<":"<<" Error Assignment statement data type not compatible\n";
	// 	exit(0);
	// }
    return 1;
}
void Selection_Statement_Ast::print(ostream & file_buffer){
	file_buffer<<"\n"<<AST_SPACE<<"IF :\n";
	file_buffer<<AST_SPACE<<"CONDITION (";
	cond->print(file_buffer);
	file_buffer<<")";
	file_buffer<<"\n"<<AST_SPACE<<"THEN ( ";
	then_part->print(file_buffer);
	file_buffer<<")";
	if(else_part!=NULL){
		file_buffer<<"\n"<<AST_SPACE<<"ELSE ( \n";
		else_part->print(file_buffer);
		file_buffer<<")";
	}



}
///////////////////////////////////////////////////////
Iteration_Statement_Ast::Iteration_Statement_Ast(Ast * c, Ast* b, int line, bool do_form){
    cond = c;
    lineno = line;
    body = b;
    is_do_form = do_form;
}
Iteration_Statement_Ast::~Iteration_Statement_Ast(){

}
Data_Type Iteration_Statement_Ast::get_data_type(){
    return node_data_type; 
}
void Iteration_Statement_Ast::set_data_type(Data_Type dt){
    node_data_type = dt; 
}
bool Iteration_Statement_Ast::check_ast(){
    return 1;
}
void Iteration_Statement_Ast::print(ostream & file_buffer){
	if(is_do_form==0){
		file_buffer<<"\n"<<AST_SPACE<<"WHILE :\n";
		file_buffer<<AST_SPACE<<"CONDITION (";
		cond->print(file_buffer);
		file_buffer<<")";

		file_buffer<<"\n"<<AST_SPACE<<"BODY ( ";
		body->print(file_buffer);
		file_buffer<<")";
	}
	else{
		file_buffer<<"\n"<<AST_SPACE<<"DO (\n";
		body->print(file_buffer);
		file_buffer<<")";

		file_buffer<<"\n"<<AST_SPACE<<"WHILE CONDITION (";
		cond->print(file_buffer);
		file_buffer<<")";

		
	}


}
///////////////////////////////////////////////////////
Sequence_Ast::Sequence_Ast(int line){
    lineno = line;
    // ast_num_child = zero_arity;
}
Sequence_Ast::~Sequence_Ast(){

}
void Sequence_Ast::ast_push_back(Ast * ast){
    statement_list.push_back(ast);
	// ast_num_child = unary_arity;
}

void Sequence_Ast::print(ostream & file_buffer){
		list<Ast*> ::iterator it; 
		// int c = 0;
		for(it= statement_list.begin();it!= statement_list.end();it++){
			if ((*it) != NULL){
				file_buffer<<"\n";
				// cout<<"--------------"<<c<<"--------\n";
				// c++;
				(*it)->print(file_buffer);
			}
		}
}
/////////////////////////////////////////////
Call_Ast::Call_Ast(string name, int line){
		procedure_name = name;
		lineno = line;
		return_value_reg =NULL;
}
Call_Ast::~Call_Ast(){

}

Data_Type Call_Ast::get_data_type(){
	return node_data_type;
}

void Call_Ast::check_actual_formal_param(Symbol_Table & formal_param_list){
	list <Ast *> :: iterator it_actual_param = actual_param_list.begin();
	list<Symbol_Table_Entry *> & ste_list = formal_param_list.get_table();

	// list <Ast *> :: iterator it_ste_list = ste_list.begin();
	// cout<<" sizes of actual param list "<<actual_param_list.size()<<" sizes of formal param list "<< formal_param_list.get_size()<<"\n";
	if(actual_param_list.size()!= ste_list.size()){
		// cout <<actual_param_list.size()<<" "<< ste_list.size()<<"\n";
		cout<<"cs316: Error "<<"parameter size mismatch \n";
		exit(0);

	}
	for(int i=1;it_actual_param != actual_param_list.end();i++,it_actual_param++){
		if((*it_actual_param)->get_data_type()!=formal_param_list.get_symbol_table_entry_by_index(i).get_data_type()){
			// cout <<(*it_actual_param)->get_data_type()<<" "<< formal_param_list.get_symbol_table_entry_by_index(i).get_data_type()<<"\n";
			// cout<<formal_param_list.get_symbol_table_entry_by_index(i).get_variable_name()<<"\n";
			cout<<"cs316: Error "<<"parameter type mismatch \n";
			exit(0);

		}
	}
	

}
void Call_Ast::set_actual_param_list(list<Ast *> & param_list){
	actual_param_list = param_list;
}

void Call_Ast::print(ostream & file_buffer){

}

///////////////////////////////////////////
Return_Ast::Return_Ast(Ast * ret_val, string name, int line){
	// ast_num_child = unary_arity;
	// lineno = line;
	return_value = ret_val;
	proc_name = name;
	lineno = line;
}

Return_Ast::~Return_Ast(){

}
Data_Type Return_Ast::get_data_type(){
	return node_data_type;
}

void Return_Ast::print(ostream & file_buffer){

}