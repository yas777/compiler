#include "local-environment.hh"
// #include ""
#include <iostream>
#include <stdlib.h>
using namespace std;

void Symbol_Table::create(Local_Environment & local_global_variables_table){
	list<Symbol_Table_Entry *> :: iterator it;

	for(it = variable_table.begin();it!=variable_table.end();it++){
		Eval_Result * result =NULL;
		string reval = (*it)->get_variable_name();
		if((*it)->get_data_type() == int_data_type){
			result=new Eval_Result_Value_Int();
		}
		else if((*it)->get_data_type() == double_data_type){
			result = new Eval_Result_Value_Double();
		}
		// cout<<(*it)->get_symbol_scope()<<"\n";
		if((*it)->get_symbol_scope()==global){
			// cout<<(*it)->get_symbol_scope()<<"\n";
			result->set_variable_status(1);
			result->set_value(0);
		
		}
		local_global_variables_table.put_variable_value(*result,reval);
	}

}

int Eval_Result::get_int_value(){

	// return 1;
}

void Eval_Result::set_value(int value){

}

double Eval_Result::get_double_value(){
	// return 1.0;
}

void Eval_Result::set_value(double value){

}

bool Eval_Result::is_variable_defined(){
	// return true;
}

void Eval_Result::set_variable_status(bool def){

}

//////////////////////////////////////////////

void Eval_Result_Value::set_value(int number){

}

void Eval_Result_Value::set_value(double number){

}

int Eval_Result_Value::get_int_value(){

}

double Eval_Result_Value::get_double_value(){

}

bool Eval_Result_Value::is_variable_defined(){
	return true;
}

//////////////////////////////////////////////////

Eval_Result_Value_Int::Eval_Result_Value_Int(){
	result_type = int_result;
	defined = false;
}

Eval_Result_Value_Int::~Eval_Result_Value_Int(){
	// delete &value;
	// delete &defined;
	// delete ;
	// delete ;

}

void Eval_Result_Value_Int::set_value(int number){
	value = number;
}

void Eval_Result_Value_Int::set_value(double number){
	value = int(number);
}

int Eval_Result_Value_Int::get_int_value(){
	return value;
}

void Eval_Result_Value_Int::set_variable_status(bool def){
	defined = def;
}

bool Eval_Result_Value_Int::is_variable_defined(){
	return defined;
}

void Eval_Result_Value_Int::set_result_enum(Result_Enum res){
	result_type = res;
}

Result_Enum Eval_Result_Value_Int::get_result_enum(){
	return result_type;
}

/////////////////////////////////////////////////////

Eval_Result_Value_Double::Eval_Result_Value_Double(){
	result_type = double_result;
	defined = false;
}

Eval_Result_Value_Double::~Eval_Result_Value_Double(){
	// delete &value;
	// delete &defined;
}

void Eval_Result_Value_Double::set_value(double number){
	value = number;
}

void Eval_Result_Value_Double::set_value(int number){
	value = double(number);
}

double Eval_Result_Value_Double::get_double_value(){
	return value;
}

void Eval_Result_Value_Double::set_variable_status(bool def){
	defined = def;
}

bool Eval_Result_Value_Double::is_variable_defined(){
	return defined;
}

void Eval_Result_Value_Double::set_result_enum(Result_Enum res){
	result_type = res;
}

Result_Enum Eval_Result_Value_Double::get_result_enum(){
	return result_type;
}

////////////////////////////////////////////////////////////

Local_Environment::Local_Environment(){

}

Local_Environment::~Local_Environment(){
	// delete &variable_table;
}

void Local_Environment::print(ostream & file_buffer){
	map <string,Eval_Result *>::iterator it;
	for (it = variable_table.begin();it!=variable_table.end();it++){
			file_buffer<<VAR_SPACE<<(it->first)<<" : ";
			// file_buffer<<(it->second)->value;
			Eval_Result *result = it->second;
			bool is_int = 0;
			if(!(result->is_variable_defined())){
				file_buffer<<"undefined\n";
				continue;
			}
			else if(result->get_result_enum()==int_result)is_int =1;
			if (is_int)file_buffer<<result->get_int_value();
			else file_buffer<<result->get_double_value();
			file_buffer<<"\n";
	}
	
	
}

bool Local_Environment::is_variable_defined(string name){
	return variable_table.find(name)->second->is_variable_defined();
}

Eval_Result * Local_Environment::get_variable_value(string name){
	return (variable_table.find(name)->second);
}

void Local_Environment::put_variable_value(Eval_Result & value, string name){
	
	variable_table[name] = &value;
}

bool Local_Environment::does_variable_exist(string name){
	return (variable_table.find(name) != variable_table.end());
}