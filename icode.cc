#include "ast.hh"
#include "icode.hh"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "reg-alloc.hh"
#include "symbol-table.hh"
using namespace std;

template class Const_Opd<double>;
template class Const_Opd<int>;
/////////////////////////// 
Instruction_Descriptor::Instruction_Descriptor(Tgt_Op op, string name, string mnn, string ics, Icode_Format icf, Assembly_Format af){
	this->inst_op = op;
	this->mnemonic = mnn;
	this->ic_symbol = ics;
	this->name = name;
	this->ic_format = icf;
	this->assem_format = af;
}

Instruction_Descriptor::Instruction_Descriptor(){

}

Tgt_Op Instruction_Descriptor::get_op(){
	return this->inst_op;
}

string Instruction_Descriptor::get_name(){
	return this->name;
}

string Instruction_Descriptor::get_mnemonic(){
	return this->mnemonic;
}

string Instruction_Descriptor::get_ic_symbol(){
	return this->ic_symbol;
}

Icode_Format Instruction_Descriptor::get_ic_format(){
	return this->ic_format;
}

Assembly_Format Instruction_Descriptor::get_assembly_format(){
	return this->assem_format;
}

void Instruction_Descriptor::print_instruction_descriptor(ostream & file_buffer){

}
/////////////////////

Register_Descriptor *Ics_Opd::get_reg(){}

//////////////////////////////
Mem_Addr_Opd::Mem_Addr_Opd(Symbol_Table_Entry & se){
	this->symbol_entry = & se;
}

void Mem_Addr_Opd::print_ics_opd(ostream & file_buffer){
	file_buffer<<symbol_entry->get_variable_name()<<"";
}

void Mem_Addr_Opd::print_asm_opd(ostream & file_buffer){
	if(symbol_entry->get_ref_offset()==sp_ref)file_buffer<<symbol_entry->get_start_offset()<<"($sp)";
	else if (symbol_entry->get_symbol_scope() == local||symbol_entry->get_symbol_scope() == formal){
		file_buffer<<symbol_entry->get_start_offset()<<"($fp)";
	}
	else{
		file_buffer<<symbol_entry->get_variable_name()<<"";
	}
}
Mem_Addr_Opd & Mem_Addr_Opd:: operator= (const Mem_Addr_Opd & rhs){
	if(this!=&rhs){
		*symbol_entry = *(rhs.symbol_entry);
	}
	return  *this;
}

////////////////////////////////
Register_Addr_Opd::Register_Addr_Opd(Register_Descriptor * rd){
	this->register_description = rd;
}
// Register_Addr_Opd::~Register_Addr_Opd(){

// }
Register_Descriptor * Register_Addr_Opd::get_reg(){
	return this->register_description;
}
void Register_Addr_Opd::print_ics_opd(ostream & file_buffer){
	file_buffer<<register_description->get_name();
}
void Register_Addr_Opd::print_asm_opd(ostream & file_buffer){
	file_buffer<<"$"<<register_description->get_name();
}
Register_Addr_Opd & Register_Addr_Opd:: operator=(const Register_Addr_Opd & rhs){
	if(this!=&rhs){
		*register_description = *(rhs.register_description);
	}
	return *this;
}

//////////////////////////////////
template <class T>
Const_Opd<T>::Const_Opd(T num){
	this->num = num;
}
// template <class T>
// Const_Opd<T>::~Const_Opd(){
	
// }
template <class T>
void Const_Opd<T>::print_ics_opd(ostream & file_buffer){
	file_buffer<<num;
}

template <class T>
void Const_Opd<T>::print_asm_opd(ostream & file_buffer){
	file_buffer<<num;
}
template <class T>
Const_Opd<T> &  Const_Opd<T>::operator=(const Const_Opd<T> & rhs){
	if(this!=&rhs){
		num = rhs.num;//check
	}
	return *this;
}

////////////////////////////////
Instruction_Descriptor & Icode_Stmt::get_op(){
	return this->op_desc;
}

Ics_Opd * Icode_Stmt::get_opd1(){}
Ics_Opd * Icode_Stmt::get_opd2(){}
Ics_Opd * Icode_Stmt::get_result(){}
void Icode_Stmt::set_opd1(Ics_Opd * io){}
void Icode_Stmt::set_opd2(Ics_Opd * io){}
void Icode_Stmt::set_result(Ics_Opd * io){}

// Icode_Stmt::~Icode_Stmt(){
// 
// }

////////////////////////// Pinr
Print_IC_Stmt::Print_IC_Stmt(){}
Print_IC_Stmt::~Print_IC_Stmt(){}
void Print_IC_Stmt::print_icode(ostream & file_buffer){
	file_buffer<<"\tprint"<<"\n";
}
void Print_IC_Stmt::print_assembly(ostream & file_buffer){
	file_buffer<<"\tsyscall"<<"\n";
}
///////////////////////////////
Move_IC_Stmt::Move_IC_Stmt(Tgt_Op inst_op, Ics_Opd * opd1, Ics_Opd * result){
	// (this->op_desc).inst_op = inst_op;
	this->op_desc = *(machine_desc_object.spim_instruction_table[inst_op]);// check
	this->opd1 = opd1;
	this->result = result;
}

Instruction_Descriptor & Move_IC_Stmt::get_inst_op_of_ics(){
	return this->op_desc;
}

Ics_Opd * Move_IC_Stmt::get_opd1(){
	return this->opd1;
}

void Move_IC_Stmt::set_opd1(Ics_Opd * io){
	this->opd1 = io;
}

Ics_Opd * Move_IC_Stmt::get_result(){
	return this->result;
}

void Move_IC_Stmt::set_result(Ics_Opd *io){
	this->result = io;
}

void Move_IC_Stmt::print_icode(ostream & file_buffer){
	// cout<<"move enter:\n";
	file_buffer<<"\t"<<op_desc.get_name()<<":\t\t";
	
	result->print_ics_opd(file_buffer);
	// cout<<"move2\n";
	file_buffer<<" <- ";
	opd1->print_ics_opd(file_buffer);
	file_buffer<<"\n";
}

void Move_IC_Stmt::print_assembly(ostream & file_buffer){
	file_buffer<<"\t"<<op_desc.get_mnemonic()<<" ";
	if(op_desc.get_op() == load||op_desc.get_op()==load_d||op_desc.get_op() == imm_load||op_desc.get_op()==imm_load_d){
		result->print_asm_opd(file_buffer);
		file_buffer<<", ";
		opd1->print_asm_opd(file_buffer);
		file_buffer<<"\n";

	}else{
		opd1->print_asm_opd(file_buffer);
		file_buffer<<", ";
		result->print_asm_opd(file_buffer);
		file_buffer<<"\n";
	}
	
}

/////////////////////////////////
Compute_IC_Stmt::Compute_IC_Stmt(Tgt_Op inst_op, Ics_Opd * opd1, Ics_Opd * opd2, Ics_Opd * result){
	this->op_desc = *(machine_desc_object.spim_instruction_table[inst_op]);// check
	this->opd1 = opd1;
	this->opd2 = opd2;
	this->result = result;
}

Instruction_Descriptor & Compute_IC_Stmt::get_inst_op_of_ics(){
	return this->op_desc;
}

Ics_Opd * Compute_IC_Stmt::get_opd1(){
	return this->opd1;
}

void Compute_IC_Stmt::set_opd1(Ics_Opd * io){
	this->opd1 = io;
}

Ics_Opd * Compute_IC_Stmt::get_opd2(){
	return this->opd2;
}

void Compute_IC_Stmt::set_opd2(Ics_Opd * io){
	this->opd2 = io;
}

Ics_Opd * Compute_IC_Stmt::get_result(){
	return this->result;
}

void Compute_IC_Stmt::set_result(Ics_Opd * io){
	this->result = result;
}

void Compute_IC_Stmt::print_icode(ostream & file_buffer){
	// cout<<"comp enter\n";
	file_buffer<<"\t"<<op_desc.get_name()<<":\t\t";
	result->print_ics_opd(file_buffer);
	file_buffer<<" <- ";
	
	if (opd1 != NULL){
		// cout<<"opd1\n";
		opd1->print_ics_opd(file_buffer);
		if (opd2 != NULL){
			// cout<<"opd12\n";
			file_buffer<<" , ";
			opd2->print_ics_opd(file_buffer);
		}
	}
	else if (opd2 != NULL){
		// cout<<"opd2\n";
		opd2->print_ics_opd(file_buffer);
	}
	file_buffer<<"\n";
}

void Compute_IC_Stmt::print_assembly(ostream & file_buffer){
	file_buffer<<"\t"<<op_desc.get_mnemonic()<<" ";
	if (result !=NULL){
		result->print_asm_opd(file_buffer);
		file_buffer<<", ";
	}
	if (opd1 != NULL){
		// cout<<"opd1\n";
		opd1->print_asm_opd(file_buffer);
		if (opd2 != NULL){
			// cout<<"opd12\n";
			file_buffer<<" , ";
			opd2->print_asm_opd(file_buffer);
		}
	}
	else if (opd2 != NULL){
		// cout<<"opd2\n";
		opd2->print_asm_opd(file_buffer);
	}
	file_buffer<<"\n";
}
///////////////////////////////////////Control_Flow

// Control_Flow_IC_Stmt::Control_Flow_IC_Stmt(Tgt_Op inst_op, Ics_Opd * opd1, string label){

Control_Flow_IC_Stmt::Control_Flow_IC_Stmt(Tgt_Op op, Ics_Opd * o1,Ics_Opd * o2, string label, int size){
	// this->op_desc = *(machine_desc_object.spim_instruction_table[inst_op]);// check
	// this->opd1 = opd1;
	// this->label = label;
	op_desc =  *(machine_desc_object.spim_instruction_table[op]);// check
	opd1 = o1;
	opd2 = o2;
	offset = label;
	actual_param_size = size;
}

Instruction_Descriptor & Control_Flow_IC_Stmt::get_inst_op_of_ics(){
	return this->op_desc;
}
Ics_Opd *Control_Flow_IC_Stmt::get_opd1(){
	return this->opd1;
}
void Control_Flow_IC_Stmt::set_opd1(Ics_Opd *io){
	this->opd1 = io;
}

Ics_Opd * Control_Flow_IC_Stmt::get_opd2(){
	return this->opd2;
}
void Control_Flow_IC_Stmt::set_opd2(Ics_Opd * io){
	this->opd2 = io;
}
string Control_Flow_IC_Stmt::get_Offset(){
	return offset;
}
void Control_Flow_IC_Stmt::set_Offset(string label){
	offset = label;
}

// string Control_Flow_IC_Stmt::get_label(){
// 	return this->label;
// }
// void Control_Flow_IC_Stmt::set_label(string label){
// 	this->label = label;
// }

void Control_Flow_IC_Stmt::print_icode(ostream & file_buffer){
	file_buffer<<"\t"<<op_desc.get_name()<<":\t\t";
	opd1->print_ics_opd(file_buffer);
	file_buffer<<", ";
	opd2->print_ics_opd(file_buffer);
	file_buffer<<" : goto "<<offset<<"\n";
}

void Control_Flow_IC_Stmt::print_assembly(ostream & file_buffer){
	file_buffer<<"\t"<<op_desc.get_mnemonic()<<" ";
	opd1->print_asm_opd(file_buffer);
	file_buffer<<", ";
	opd2->print_asm_opd(file_buffer);
	file_buffer<<", "<<offset<<"\n";

}
///////////////////////////////Label_IC_Stmt


Label_IC_Stmt::Label_IC_Stmt(Tgt_Op inst_op, string label){
	this->op_desc = *(machine_desc_object.spim_instruction_table[inst_op]);// check
	this->label =label;
} 
// Label_IC_Stmt::~Label_IC_Stmt() {

// } 

Label_IC_Stmt & Label_IC_Stmt:: operator=(const Label_IC_Stmt & rhs){
	if(this!=&rhs){
		this->label = rhs.label;
		this->op_desc = rhs.op_desc;
	}
	return *this;
}

Instruction_Descriptor & Label_IC_Stmt:: get_inst_op_of_ics(){
	return this->op_desc;
}
string Label_IC_Stmt:: get_label(){
	return this->label;
}
void Label_IC_Stmt:: set_label(string label){
	this->label = label;
}

void Label_IC_Stmt:: print_icode(ostream & file_buffer){
	if(op_desc.get_name() == ""){
		file_buffer<<label<<":\n";
	}
	else if(op_desc.get_name() == "jump"){
		file_buffer<<"\tgoto "<<label<<"\n";
	}
}
void Label_IC_Stmt:: print_assembly(ostream & file_buffer){
	if(op_desc.get_mnemonic() == ""){
		file_buffer<<label<<":\n";
	}
	else{
		file_buffer<<"\t"<<op_desc.get_mnemonic()<<" "<<label<<"\n";
	}
}
////////// Intermediate code for Ast statements


Code_For_Ast::Code_For_Ast(){
	// ics_list = [];
	result_register = new Register_Descriptor();
}
Code_For_Ast::Code_For_Ast(list<Icode_Stmt *> & ic_l, Register_Descriptor * reg){
	this->ics_list = ic_l;
	this->result_register = reg;
}
// Code_For_Ast::~Code_For_Ast() {

// }

void Code_For_Ast::append_ics(Icode_Stmt & ics){
	ics_list.push_back(&ics);
}
list<Icode_Stmt *> & Code_For_Ast::get_icode_list(){
	return this->ics_list;
}

Register_Descriptor * Code_For_Ast::get_reg(){
	return this->result_register;
}
void Code_For_Ast::set_reg(Register_Descriptor * reg){
	this->result_register =reg;
}

Code_For_Ast & Code_For_Ast::operator=(const Code_For_Ast & rhs){
	if(this != &rhs){
		ics_list = rhs.ics_list; //??
		*result_register = *(rhs.result_register);
	}
	return *this;
}
