#include "ast.hh"
#include <iostream>
#include <stdlib.h>
#include "icode.hh"
#include "symbol-table.hh"
#include "reg-alloc.hh"
#include "procedure.hh"
#include "program.hh"
#include <iostream>
using namespace std;

template class Number_Ast<double>;
template class Number_Ast<int>;

////////////////////////////////////////

Relational_Op glob_comp;
// final list a followed by b
Code_For_Ast * my_merge(Code_For_Ast &a, Code_For_Ast &b,Register_Descriptor * reg){
	// cout<<"merge\n";
	list<Icode_Stmt *> alist = a.get_icode_list();
	list<Icode_Stmt *> blist = b.get_icode_list();
	// alist.merge(blist);
	alist.insert(alist.end(), blist.begin(), blist.end());

	if(reg==NULL ){
		reg = new Register_Descriptor();
	}
	Code_For_Ast * result = new Code_For_Ast(alist,reg);
	// cout<<"merge return\n";
	// cout<< result->get_reg()->get_name()<<"\n";
	return result;
}
Register_Descriptor * my_new_reg(Data_Type node_data_type){
	Register_Descriptor * res_reg = NULL;
	if(node_data_type == int_data_type){
		res_reg = machine_desc_object.get_new_register<int_reg>();
	}
	else if(node_data_type == double_data_type){
		res_reg = machine_desc_object.get_new_register<float_reg>();
	}
	return res_reg;

}
/////Ast

Code_For_Ast & Ast::create_store_stmt(Register_Descriptor * store_register){
}
//////
Code_For_Ast & Assignment_Ast::compile(){
	
	// cout<<"Assigment Ast\n";
	
	Code_For_Ast * rhs_cast = &(rhs->compile());
	Register_Descriptor * rhs_reg = rhs_cast->get_reg();
	Code_For_Ast * lhs_cast = &(lhs->create_store_stmt(rhs_reg));
	Code_For_Ast * result = (my_merge(*rhs_cast,*lhs_cast,rhs_reg));
	(lhs_cast->get_reg())->reset_use_for_expr_result();
	(rhs_cast->get_reg())->reset_use_for_expr_result();
	(result->get_reg())->reset_use_for_expr_result();
	// cout<<"Assigment Ast return\n";
	
	return *result;    
}
Code_For_Ast & Assignment_Ast::compile_and_optimize_ast(Lra_Outcome & lra){

}
//////////////////////
Code_For_Ast & Print_Ast::compile(){
	// freeing the registers here
	Code_For_Ast * result = new Code_For_Ast();
	Register_Descriptor * reg1 = new Register_Descriptor();
	Register_Descriptor * reg2 = new Register_Descriptor();
	// cout<<var->get_data_type();
	if(var->get_data_type() == int_data_type){
		// cout<<"int print";
		reg1 = machine_desc_object.spim_register_table[v0];
		Register_Addr_Opd *reg1_rd = new Register_Addr_Opd(reg1);
		Const_Opd<int>* ca1 = new Const_Opd<int>(1);
		Move_IC_Stmt * mst1 = new Move_IC_Stmt(imm_load, ca1, reg1_rd);
		result->append_ics(*mst1);

		reg2 = machine_desc_object.spim_register_table[a0];
		Register_Addr_Opd *reg2_rd = new Register_Addr_Opd(reg2);
		Mem_Addr_Opd * mem1 = new Mem_Addr_Opd(var->get_symbol_entry());
		Move_IC_Stmt * mst2 = new Move_IC_Stmt(load, mem1, reg2_rd);
		result->append_ics(*mst2);
	}
	else if (var->get_data_type()== double_data_type){
		reg1 = machine_desc_object.spim_register_table[v0];
		Register_Addr_Opd *reg1_rd = new Register_Addr_Opd(reg1);
		Const_Opd<int>* ca1 = new Const_Opd<int>(3);//? int or double
		Move_IC_Stmt * mst1 = new Move_IC_Stmt(imm_load, ca1, reg1_rd);
		result->append_ics(*mst1);

		reg2 = machine_desc_object.spim_register_table[f12];
		Register_Addr_Opd *reg2_rd = new Register_Addr_Opd(reg2);
		Mem_Addr_Opd * mem1 = new Mem_Addr_Opd(var->get_symbol_entry());
		Move_IC_Stmt * mst2 = new Move_IC_Stmt(load_d, mem1, reg2_rd);
		result->append_ics(*mst2);		
	}
	Print_IC_Stmt * pst = new Print_IC_Stmt();
	result->append_ics(*pst);
	return *result;
}
////////////////////////////////Name_Ast
Code_For_Ast & Name_Ast::compile(){
	// cout<<"Name Ast enter\n";
	Move_IC_Stmt* mic1 = NULL;

	Mem_Addr_Opd* ma1 = new Mem_Addr_Opd(get_symbol_entry());
	Register_Descriptor * rd1 = NULL;
	if(node_data_type == int_data_type){
		rd1 = machine_desc_object.get_new_register<int_reg>();
		Register_Addr_Opd* ra1 = new Register_Addr_Opd(rd1);
		mic1 =new Move_IC_Stmt(load,ma1, ra1);
	}
	else if(node_data_type == double_data_type){
		rd1 = machine_desc_object.get_new_register<float_reg>();
		Register_Addr_Opd* ra1 = new Register_Addr_Opd(rd1);
		mic1 =new Move_IC_Stmt(load_d, ma1, ra1);
	}
	Code_For_Ast * result = new Code_For_Ast();
	result->append_ics(*mic1);
	result->set_reg(rd1);
	// cout<<"Name Ast return\n";
	return *result;
}
Code_For_Ast & Name_Ast::compile_and_optimize_ast(Lra_Outcome & lra){
//lra
}
Code_For_Ast & Name_Ast::create_store_stmt(Register_Descriptor * store_register){
	// cout<<"Name Ast create_store_stmt\n";
	Register_Addr_Opd* ra1 = new Register_Addr_Opd(store_register);
	Mem_Addr_Opd* ma1 = new Mem_Addr_Opd(*variable_symbol_entry);
	Move_IC_Stmt* mic1 = NULL;
	
	if(node_data_type == int_data_type){
		// cout<<node_data_type<<"\n";
		mic1 =new Move_IC_Stmt(store,ra1,ma1);
	}
	else if(node_data_type == double_data_type){
		mic1 =new Move_IC_Stmt(store_d, ra1, ma1);
	}
	Code_For_Ast * result = new Code_For_Ast();
	result->append_ics(*mic1);
	return *result;

}

//////////////////////////////////Number_Ast
template <class T>
Code_For_Ast & Number_Ast<T>::compile(){
	// cout<<"Number_Ast enter\n";
	Move_IC_Stmt* mic1 = NULL;

	Const_Opd<T>* ca1 = new Const_Opd<T>(constant);
	Register_Descriptor * rd1 = NULL;
	if(node_data_type == int_data_type){
		rd1 = machine_desc_object.get_new_register<int_reg>();
		Register_Addr_Opd* ra1 = new Register_Addr_Opd(rd1);
		mic1 =new Move_IC_Stmt(imm_load, ca1, ra1);
	}
	else if(node_data_type == double_data_type){
		rd1 = machine_desc_object.get_new_register<float_reg>();
		Register_Addr_Opd* ra1 = new Register_Addr_Opd(rd1);
		mic1 =new Move_IC_Stmt(imm_load_d, ca1, ra1);
	}
	Code_For_Ast * result = new Code_For_Ast();
	result->append_ics(*mic1);
	result->set_reg(rd1);
	// cout<<"Number_Ast return\n";
	return *result;
}
template <class T>
Code_For_Ast & Number_Ast<T>::compile_and_optimize_ast(Lra_Outcome & lra){

}
/////////////////////////////////////Arithmetic_Expr_Ast

/////////////////////////////////////Plus_Ast-left associative
Code_For_Ast & Plus_Ast::compile(){
	Code_For_Ast * lhs_cast = &(lhs->compile());
	Code_For_Ast * rhs_cast = &(rhs->compile());
	Register_Descriptor * res_reg = NULL; 
	Register_Addr_Opd * res_rd = NULL;
	Register_Addr_Opd * lhs_rd = NULL;
	Register_Addr_Opd * rhs_rd = NULL;
	Compute_IC_Stmt *cic1 = NULL;
	if(node_data_type == int_data_type){
		res_reg = machine_desc_object.get_new_register<int_reg>();
		res_rd = new Register_Addr_Opd(res_reg);
		lhs_rd = new Register_Addr_Opd(lhs_cast->get_reg());
		rhs_rd = new Register_Addr_Opd(rhs_cast->get_reg());
		cic1= new Compute_IC_Stmt(add,lhs_rd,rhs_rd,res_rd);
	}
	else if(node_data_type == double_data_type){
		res_reg = machine_desc_object.get_new_register<float_reg>();
		res_rd = new Register_Addr_Opd(res_reg);
		lhs_rd = new Register_Addr_Opd(lhs_cast->get_reg());
		rhs_rd = new Register_Addr_Opd(rhs_cast->get_reg());
		cic1 = new Compute_IC_Stmt(add_d,lhs_rd,rhs_rd,res_rd);
	}
	
	// Compute_IC_Stmt cic1 = new 
	Code_For_Ast * result = (my_merge(*lhs_cast,*rhs_cast,res_reg));
	result->append_ics(*cic1);

	// (result->get_reg())->reset_use_for_expr_result();
	(lhs_cast->get_reg())->reset_use_for_expr_result();
	(rhs_cast->get_reg())->reset_use_for_expr_result();
	return *result;

	// return *result;
	
}
Code_For_Ast & Plus_Ast::compile_and_optimize_ast(Lra_Outcome & lra){

}
////////////////////////////////////Minus_Ast
Code_For_Ast & Minus_Ast::compile(){
	Code_For_Ast * lhs_cast = &(lhs->compile());
	Code_For_Ast * rhs_cast = &(rhs->compile());
	Register_Descriptor * res_reg = NULL; 
	Register_Addr_Opd * res_rd = NULL;
	Register_Addr_Opd * lhs_rd = NULL;
	Register_Addr_Opd * rhs_rd = NULL;
	Compute_IC_Stmt *cic1 = NULL;
	if(node_data_type == int_data_type){
		res_reg = machine_desc_object.get_new_register<int_reg>();
		res_rd = new Register_Addr_Opd(res_reg);
		lhs_rd = new Register_Addr_Opd(lhs_cast->get_reg());
		rhs_rd = new Register_Addr_Opd(rhs_cast->get_reg());
		cic1= new Compute_IC_Stmt(sub,lhs_rd,rhs_rd,res_rd);
	}
	else if(node_data_type == double_data_type){
		res_reg = machine_desc_object.get_new_register<float_reg>();
		res_rd = new Register_Addr_Opd(res_reg);
		lhs_rd = new Register_Addr_Opd(lhs_cast->get_reg());
		rhs_rd = new Register_Addr_Opd(rhs_cast->get_reg());
		cic1 = new Compute_IC_Stmt(sub_d,lhs_rd,rhs_rd,res_rd);
	}
	
	// Compute_IC_Stmt cic1 = new 
	Code_For_Ast * result = (my_merge(*lhs_cast,*rhs_cast,res_reg));
	result->append_ics(*cic1);

	// (result->get_reg())->reset_use_for_expr_result();
	(lhs_cast->get_reg())->reset_use_for_expr_result();
	(rhs_cast->get_reg())->reset_use_for_expr_result();
	return *result;
}
Code_For_Ast & Minus_Ast::compile_and_optimize_ast(Lra_Outcome & lra){

}
/////////////////////////////////////Divide_Ast
Code_For_Ast & Divide_Ast::compile(){
	Code_For_Ast * lhs_cast = &(lhs->compile());
	Code_For_Ast * rhs_cast = &(rhs->compile());
	Register_Descriptor * res_reg = NULL; 
	Register_Addr_Opd * res_rd = NULL;
	Register_Addr_Opd * lhs_rd = NULL;
	Register_Addr_Opd * rhs_rd = NULL;
	Compute_IC_Stmt *cic1 = NULL;
	if(node_data_type == int_data_type){
		res_reg = machine_desc_object.get_new_register<int_reg>();
		res_rd = new Register_Addr_Opd(res_reg);
		lhs_rd = new Register_Addr_Opd(lhs_cast->get_reg());
		rhs_rd = new Register_Addr_Opd(rhs_cast->get_reg());
		cic1= new Compute_IC_Stmt(divd,lhs_rd,rhs_rd,res_rd);
	}
	else if(node_data_type == double_data_type){
		res_reg = machine_desc_object.get_new_register<float_reg>();
		res_rd = new Register_Addr_Opd(res_reg);
		lhs_rd = new Register_Addr_Opd(lhs_cast->get_reg());
		rhs_rd = new Register_Addr_Opd(rhs_cast->get_reg());
		cic1 = new Compute_IC_Stmt(div_d,lhs_rd,rhs_rd,res_rd);
	}
	
	// Compute_IC_Stmt cic1 = new 
	Code_For_Ast * result = (my_merge(*lhs_cast,*rhs_cast,res_reg));
	result->append_ics(*cic1);

	// (result->get_reg())->reset_use_for_expr_result();
	(lhs_cast->get_reg())->reset_use_for_expr_result();
	(rhs_cast->get_reg())->reset_use_for_expr_result();
	return *result;
}
Code_For_Ast & Divide_Ast::compile_and_optimize_ast(Lra_Outcome & lra){

}
/////////////////////////////////////////Mult_Ast
Code_For_Ast & Mult_Ast::compile(){
	Code_For_Ast * lhs_cast = &(lhs->compile());
	Code_For_Ast * rhs_cast = &(rhs->compile());
	Register_Descriptor * res_reg = NULL; 
	Register_Addr_Opd * res_rd = NULL;
	Register_Addr_Opd * lhs_rd = NULL;
	Register_Addr_Opd * rhs_rd = NULL;
	Compute_IC_Stmt *cic1 = NULL;
	if(node_data_type == int_data_type){
		res_reg = machine_desc_object.get_new_register<int_reg>();
		res_rd = new Register_Addr_Opd(res_reg);
		lhs_rd = new Register_Addr_Opd(lhs_cast->get_reg());
		rhs_rd = new Register_Addr_Opd(rhs_cast->get_reg());
		cic1= new Compute_IC_Stmt(mult,lhs_rd,rhs_rd,res_rd);
	}
	else if(node_data_type == double_data_type){
		res_reg = machine_desc_object.get_new_register<float_reg>();
		res_rd = new Register_Addr_Opd(res_reg);
		lhs_rd = new Register_Addr_Opd(lhs_cast->get_reg());
		rhs_rd = new Register_Addr_Opd(rhs_cast->get_reg());
		cic1 = new Compute_IC_Stmt(mult_d,lhs_rd,rhs_rd,res_rd);
	}
	
	// Compute_IC_Stmt cic1 = new 
	Code_For_Ast * result = (my_merge(*lhs_cast,*rhs_cast,res_reg));
	result->append_ics(*cic1);

	// (result->get_reg())->reset_use_for_expr_result();
	(lhs_cast->get_reg())->reset_use_for_expr_result();
	(rhs_cast->get_reg())->reset_use_for_expr_result();
	return *result;
}
Code_For_Ast & Mult_Ast::compile_and_optimize_ast(Lra_Outcome & lra){
	
}
///////////////////////////////////////////UMinus_Ast
Code_For_Ast & UMinus_Ast::compile(){
	Code_For_Ast * lhs_cast = &(lhs->compile());
	// Code_For_Ast * rhs_cast = &(rhs->compile());
	Register_Descriptor * res_reg = new Register_Descriptor(); 
	Register_Addr_Opd * res_rd = NULL;
	Register_Addr_Opd * lhs_rd = NULL;
	// Register_Addr_Opd * rhs_rd = NULL;
	Compute_IC_Stmt *cic1 = NULL;
	if(node_data_type == int_data_type){
		res_reg = machine_desc_object.get_new_register<int_reg>();
		res_rd = new Register_Addr_Opd(res_reg);
		lhs_rd = new Register_Addr_Opd(lhs_cast->get_reg());
		// rhs_rd = new Register_Addr_Opd(rhs_cast->get_reg());
		// cout<<"compute before\n";
		cic1= new Compute_IC_Stmt(uminus,lhs_rd,NULL,res_rd);
	}
	else if(node_data_type == double_data_type){
		res_reg = machine_desc_object.get_new_register<float_reg>();
		res_rd = new Register_Addr_Opd(res_reg);
		lhs_rd = new Register_Addr_Opd(lhs_cast->get_reg());
		// rhs_rd = new Register_Addr_Opd(rhs_cast->get_reg());
		cic1= new Compute_IC_Stmt(uminus_d,lhs_rd,NULL,res_rd);
	}
	
	// Compute_IC_Stmt cic1 = new 
	(lhs_cast->get_reg())->reset_use_for_expr_result();
	Code_For_Ast * result = lhs_cast;
	result->append_ics(*cic1);
	result->set_reg(res_reg);
	// (result->get_reg())->reset_use_for_expr_result();
	// cout<<lhs_cast->get_reg()->get_name()<<"\n";
	
	return *result;
}
Code_For_Ast & UMinus_Ast::compile_and_optimize_ast(Lra_Outcome & lra){

}
////////////////////////////////////////Conditional_Expression_Ast
Code_For_Ast & Conditional_Expression_Ast::compile(){
	//  b = (2>4)?2:3;
	// cout<<"Conditional_Expression_Ast enter\n";
	Register_Descriptor * zero_reg =  machine_desc_object.spim_register_table[zero];
	Register_Addr_Opd  *zero_rd = new Register_Addr_Opd(zero_reg);
	Register_Descriptor * res_reg = NULL; 

	Code_For_Ast * result = NULL;
	Code_For_Ast * result1 = NULL;
	Code_For_Ast * result2 = NULL;
	
	Code_For_Ast * cond_cast = &(cond->compile());
	
	Register_Addr_Opd* cond_rd = new Register_Addr_Opd(cond_cast->get_reg());
	string lab1 = get_new_label();
	
	Control_Flow_IC_Stmt * cfl = new Control_Flow_IC_Stmt(beq, cond_rd,zero_rd, lab1);
	cond_cast->append_ics(*cfl);
	Code_For_Ast * lhs_cast = &(lhs->compile());
	Code_For_Ast * rhs_cast = &(rhs->compile());
	Register_Addr_Opd* lhs_rd = new Register_Addr_Opd(lhs_cast->get_reg());
	Register_Addr_Opd* rhs_rd = new Register_Addr_Opd(rhs_cast->get_reg());
	
	result = my_merge(*cond_cast, *lhs_cast, lhs_cast->get_reg());
	res_reg = my_new_reg(node_data_type);

	Register_Addr_Opd* res_rd = new Register_Addr_Opd(res_reg);
	Compute_IC_Stmt * cmpst = new Compute_IC_Stmt(or_t, lhs_rd, zero_rd, res_rd);
	result->append_ics(*cmpst);
	result->set_reg(res_reg); 
	
	string lab2 = get_new_label();
	Label_IC_Stmt * gtst1 = new Label_IC_Stmt(j, lab2);
	Label_IC_Stmt * gtst2 = new Label_IC_Stmt(label, lab1);
	result->append_ics(*gtst1);
	result->append_ics(*gtst2);
	

	result = (my_merge(*result, *rhs_cast, rhs_cast->get_reg()));
	Label_IC_Stmt * gtst3 = new Label_IC_Stmt(label, lab2);
	Compute_IC_Stmt * cmpst1 = new Compute_IC_Stmt(or_t, rhs_rd, zero_rd, res_rd);
	result->append_ics(*cmpst1);
	result->set_reg(res_reg);
	result->append_ics(*gtst3);

	(cond_cast->get_reg())->reset_use_for_expr_result();
	(lhs_cast->get_reg())->reset_use_for_expr_result();
	(rhs_cast->get_reg())->reset_use_for_expr_result();
	// cout<<"Conditional_Expression_Ast return\n";
	return *result;

}
// Code_For_Ast & Conditional_Expression_Ast::compile_and_optimize_ast(Lra_Outcome & lra){

////////////////////////////////////Relational_Expr_Ast
Code_For_Ast & Relational_Expr_Ast::compile(){
	// Code
	// cout<<"Relational_Expr_Ast enter\n";
	Code_For_Ast * lhs_cast = &(lhs_condition->compile());
	Code_For_Ast * rhs_cast = &(rhs_condition->compile());

	Compute_IC_Stmt *cic1 = NULL;
	Register_Descriptor * res_reg = machine_desc_object.get_new_register<int_reg>();
	Register_Addr_Opd * res_rd = new Register_Addr_Opd(res_reg);
	Register_Addr_Opd * lhs_rd = new Register_Addr_Opd(lhs_cast->get_reg());
	Register_Addr_Opd * rhs_rd = new Register_Addr_Opd(rhs_cast->get_reg());
	if (node_data_type == int_data_type){
		if(rel_op == less_equalto){
			cic1= new Compute_IC_Stmt(sle,lhs_rd,rhs_rd,res_rd);
		}
		else if(rel_op == less_than){
			cic1= new Compute_IC_Stmt(slt,lhs_rd,rhs_rd,res_rd);
		}
		else if(rel_op == greater_than){
			cic1= new Compute_IC_Stmt(sgt,lhs_rd,rhs_rd,res_rd);
		}
		else if(rel_op == greater_equalto){
			cic1= new Compute_IC_Stmt(sge,lhs_rd,rhs_rd,res_rd);
		}
		else if(rel_op == not_equalto){
			cic1= new Compute_IC_Stmt(sne,lhs_rd,rhs_rd,res_rd);
		}
		else if(rel_op == equalto){
			cic1= new Compute_IC_Stmt(seq,lhs_rd,rhs_rd,res_rd);
		}
	}
	else if (node_data_type == double_data_type){
		if(rel_op == less_equalto){
			cic1= new Compute_IC_Stmt(sle_d,lhs_rd,rhs_rd,NULL);
			glob_comp = less_equalto;
		}
		else if(rel_op == less_than){
			cic1= new Compute_IC_Stmt(slt_d,lhs_rd,rhs_rd,NULL);
			glob_comp = less_than;
		}
		else if(rel_op == greater_than){
			cic1= new Compute_IC_Stmt(sgt_d,lhs_rd,rhs_rd,NULL);
			glob_comp = greater_than;
		}
		else if(rel_op == greater_equalto){
			cic1= new Compute_IC_Stmt(sge_d,lhs_rd,rhs_rd,NULL);
			glob_comp = greater_equalto;
		}
		else if(rel_op == not_equalto){
			cic1= new Compute_IC_Stmt(sne_d,lhs_rd,rhs_rd,NULL);
			glob_comp = not_equalto;
		}
		else if(rel_op == equalto){
			cic1= new Compute_IC_Stmt(seq_d,lhs_rd,rhs_rd,NULL);
			glob_comp = equalto;
		}

		// Label_IC_Stmt * bcl = new Label_IC_Stmt(bc1f, )
	}
	// Compute_IC_Stmt cic1 = new 
		Code_For_Ast * result = (my_merge(*lhs_cast,*rhs_cast,res_reg));
		result->append_ics(*cic1);
		(lhs_cast->get_reg())->reset_use_for_expr_result();
		(rhs_cast->get_reg())->reset_use_for_expr_result();
		// cout<<"Relational_Expr_Ast return\n";
		return *result;
}
// Code_For_Ast & Relational_Expr_Ast::compile_and_optimize_ast(Lra_Outcome & lra){

// }
//////////////////////////////////////Logical_Expr_Ast
Code_For_Ast & Logical_Expr_Ast::compile(){
	
	// cout<<"Logical_Expr_Ast enter\n";

	Code_For_Ast * lhs_cast = new Code_For_Ast();
	// Register_Addr_Opd * lhs_rd = NULL;
	// Code_For_Ast * result  = NULL;
	if(bool_op != _logical_not){
		// cout<<"not null\n";
		lhs_cast = &(lhs_op->compile());
		// lhs_rd = new Register_Addr_Opd(lhs_cast->get_reg());
	}
	else{
		// cout<<"null\n";
		Register_Descriptor *reg1 = machine_desc_object.get_new_register<int_reg>();
		Register_Addr_Opd *reg1_rd = new Register_Addr_Opd(reg1);
		Const_Opd<int>* ca1 = new Const_Opd<int>(1);
		// cout<<"null11111\n";
		Move_IC_Stmt * mst1 = new Move_IC_Stmt(imm_load, ca1, reg1_rd);

		lhs_cast->append_ics(*mst1);
		lhs_cast->set_reg(reg1);
		
	}
	Code_For_Ast * rhs_cast = &(rhs_op->compile());

	Compute_IC_Stmt *cic1 = NULL;
	Register_Descriptor * res_reg = machine_desc_object.get_new_register<int_reg>();
	Register_Addr_Opd * res_rd = new Register_Addr_Opd(res_reg);
	Register_Addr_Opd * lhs_rd = new Register_Addr_Opd(lhs_cast->get_reg());
	Register_Addr_Opd * rhs_rd = new Register_Addr_Opd(rhs_cast->get_reg());
	if(bool_op == _logical_not){
		// cout<<"nulll\n";
		cic1= new Compute_IC_Stmt(not_t,rhs_rd,lhs_rd,res_rd);
	}
	else if(bool_op == _logical_or){
		cic1= new Compute_IC_Stmt(or_t,lhs_rd,rhs_rd,res_rd);
	}
	else if(bool_op == _logical_and){
		cic1= new Compute_IC_Stmt(and_t,lhs_rd,rhs_rd,res_rd);
	} 

	Code_For_Ast * result = (my_merge(*lhs_cast,*rhs_cast,res_reg));
	result->append_ics(*cic1);
	(lhs_cast->get_reg())->reset_use_for_expr_result();
	(rhs_cast->get_reg())->reset_use_for_expr_result();

	
	// cout<<"Logical_Expr_Ast return\n";
	return *result;
	

}
/////////////////////////////////////////Selection_Statement_Ast
Code_For_Ast & Selection_Statement_Ast::compile(){
	// Register_Descriptor * res_reg = NULL;
	// cout<<"Selection_Statement_Ast enter\n";
	Code_For_Ast * cond_cast = &(cond->compile());
	Register_Addr_Opd* cond_rd = new Register_Addr_Opd(cond_cast->get_reg());
	Register_Descriptor * zero_reg =  machine_desc_object.spim_register_table[zero];
	Register_Addr_Opd  *zero_rd = new Register_Addr_Opd(zero_reg);
		

	string lab0 = get_new_label();
	if (cond->get_data_type() == int_data_type){
		Control_Flow_IC_Stmt* cflst = new Control_Flow_IC_Stmt(beq, cond_rd,zero_rd, lab0);
		cond_cast->append_ics(*cflst);
	}
	else if(cond->get_data_type() == double_data_type){
		if (glob_comp == greater_than || glob_comp == greater_equalto || glob_comp == not_equalto){
			Label_IC_Stmt * lbst1 = new Label_IC_Stmt(bc1t, lab0);
			cond_cast->append_ics(*lbst1);
		}
		else{
			Label_IC_Stmt * lbst1 = new Label_IC_Stmt(bc1f, lab0);
			cond_cast->append_ics(*lbst1);
		}
	}
	// (cond_cast->get_reg())->reset_use_for_expr_result();
	Code_For_Ast * then_cast = &(then_part->compile());
	
	Code_For_Ast * result = my_merge(*cond_cast, *then_cast, NULL);
	if(else_part == NULL){
		// cout<<"else_part NULL\n";
		Label_IC_Stmt * gtst1 = new Label_IC_Stmt(label, lab0);
		result->append_ics(*gtst1);
	}
	else{
		// cout<<"else_part +\n";
		string lab1 = get_new_label();
		Label_IC_Stmt * gtst1 = new Label_IC_Stmt(j, lab1);
		Label_IC_Stmt * gtst2 = new Label_IC_Stmt(label, lab0);
		result->append_ics(*gtst1);
		result->append_ics(*gtst2);
		// result->append_ics(else_cast);
		Code_For_Ast * else_cast = &(else_part->compile());
		result = (my_merge(*result, *else_cast, NULL));
		Label_IC_Stmt * gtst3 = new Label_IC_Stmt(label, lab1);
		result->append_ics(*gtst3);
		(else_cast->get_reg())->reset_use_for_expr_result();
	}
	(cond_cast->get_reg())->reset_use_for_expr_result();
	(then_cast->get_reg())->reset_use_for_expr_result();
	
	
	// cout<<"Selection_Statement_Ast return\n";
	return *result;
}
// Code_For_Ast & Selection_Statement_Ast::compile_and_optimize_ast(Lra_Outcome & lra){

// }
/////////////////////////////////////////Iteration_Statement_Ast
Code_For_Ast & Iteration_Statement_Ast::compile(){
	// cout<<"Iteration_Statement_Ast enter\n";
	Code_For_Ast *result = new Code_For_Ast();
	Register_Descriptor * zero_reg =  machine_desc_object.spim_register_table[zero];
	Register_Addr_Opd  *zero_rd = new Register_Addr_Opd(zero_reg);
	string lab1 = get_new_label();
	string lab2 = get_new_label();
	if (!is_do_form){
		Label_IC_Stmt * gtst2 = new Label_IC_Stmt(j, lab2);
		result->append_ics(*gtst2);
	}
	Label_IC_Stmt * lbst1 = new Label_IC_Stmt(label, lab1);
	result->append_ics(*lbst1);
	Code_For_Ast * cond_cast = &(cond->compile());
	Register_Addr_Opd* cond_rd = new Register_Addr_Opd(cond_cast->get_reg());
	Label_IC_Stmt * lbst3 = NULL;
	Control_Flow_IC_Stmt * cfl = NULL;
	if (cond->get_data_type() == int_data_type){
		 cfl = new Control_Flow_IC_Stmt(bne, cond_rd, zero_rd,lab1);
		// result->append_ics(*cfl1);
	}
	else if(cond->get_data_type() == double_data_type){
		if (glob_comp == greater_than || glob_comp == greater_equalto || glob_comp == not_equalto){
			lbst3 = new Label_IC_Stmt(bc1f, lab1);
			// result->append_ics(*lbst1);
		}
		else{
			 lbst3 = new Label_IC_Stmt(bc1t, lab1);
			// result->append_ics(*lbst1);
		}
	}
	Code_For_Ast * body_cast = &(body->compile());
	result = my_merge(*result, *body_cast, NULL);
	Label_IC_Stmt * lbst2 = new Label_IC_Stmt(label, lab2);
	result->append_ics(*lbst2);

	result = my_merge(*result, *cond_cast, NULL);

	if(cond->get_data_type() == int_data_type){
		result->append_ics(*cfl);
	}
	else if(cond->get_data_type() == double_data_type){
		result->append_ics(*lbst3);
	}
	
	(body_cast->get_reg())->reset_use_for_expr_result();	
	(cond_cast->get_reg())->reset_use_for_expr_result();	
	(result->get_reg())->reset_use_for_expr_result();	
	// cout<<"Iteration_Statement_Ast return\n";

	return *result;
}
// Code_For_Ast & Iteration_Statement_Ast::compile_and_optimize_ast(Lra_Outcome & lra){

// }
/////////////////////////////////////////Sequence_Ast
Code_For_Ast & Sequence_Ast::compile(){
	// cout<<"Sequence Ast enter\n";
	Code_For_Ast * result = new Code_For_Ast();
	list<Ast*> :: iterator it;
	for (it=statement_list.begin();it!=statement_list.end();it++){
		Code_For_Ast * ast_cast = &((*it)->compile());
		(ast_cast->get_reg())->reset_use_for_expr_result();	
		result = my_merge(*result,*ast_cast,NULL);
	}
	(result->get_reg())->reset_use_for_expr_result();
	// cout<<"Sequence Ast return\n";
	return *result;
}
// Code_For_Ast & Sequence_Ast::compile_and_optimize_ast(Lra_Outcome & lra){

// }
// Code_For_Ast & Sequence_Ast::create_store_stmt(Register_Descriptor * store_register){

// }
///////////////////////////////////////////Call_Ast
// lw $v0, b_
// sw $v0, 0($sp)
// li.d $f2, 1.10
// s.d $f2, -4($sp)
// sub $sp, $sp, 12
// jal f_
// add $sp, $sp, 12
// move $v0, $v1
// sw $v0, b_
// b = f(b,a,c);
// load:    	v0 <- c_
// store:    	c_ <- v0
// load:    	v0 <- a_
// store:    	b_ <- v0
// load:    	v0 <- b_
// store:    	a_ <- v0
// call f_
// move:    	v0 <- v1

void Call_Ast::set_register(Register_Descriptor * reg){
	return_value_reg = reg;
}
Code_For_Ast & Call_Ast::compile(){
	Procedure * proc=  program_object.get_procedure_prototype(procedure_name);
				
	// cout<<"Call_Ast enter\n";
	Code_For_Ast * result = new Code_For_Ast();
	list<Ast*> :: reverse_iterator it = actual_param_list.rbegin();
	int len = actual_param_list.size();
	int sp_size = 0;
	Symbol_Table & formal_param_list = proc->get_formal_param_list();
	
	for (int i =len;it!=actual_param_list.rend()&&i>=1;it++,i--){
		Code_For_Ast * param_cast = &((*it)->compile());
		Register_Addr_Opd * param_reg = new Register_Addr_Opd(param_cast->get_reg());
		Symbol_Table_Entry & formal_param = formal_param_list.get_symbol_table_entry_by_index(i);
		// Symbol_Table_Entry(string & name, Data_Type new_data_type, int line, Offset_With_Ref_To ro = fp_ref);

		string name = formal_param.get_variable_name();
		Symbol_Table_Entry * formal_param_dummy = new Symbol_Table_Entry(name,formal_param.get_data_type(),formal_param.get_lineno(),sp_ref);
		Move_IC_Stmt* mic1 = NULL;
		if((*it)->get_data_type()==int_data_type){
			formal_param_dummy->set_start_offset(-sp_size);
			Mem_Addr_Opd* ma1 = new Mem_Addr_Opd(*formal_param_dummy);
			mic1 =new Move_IC_Stmt(store,param_reg,ma1);
			sp_size += 4;
		}
		else if((*it)->get_data_type()==double_data_type){
			formal_param_dummy->set_start_offset(-sp_size);
			Mem_Addr_Opd* ma1 = new Mem_Addr_Opd(*formal_param_dummy);
			mic1 =new Move_IC_Stmt(store_d,param_reg,ma1);
			sp_size +=8;
		}
		param_cast->append_ics(*mic1);
		(param_cast->get_reg())->reset_use_for_expr_result();	
		result = my_merge(*result,*param_cast,NULL);
	}
	if(len){
		Compute_IC_Stmt *cic1 = NULL;
		Compute_IC_Stmt *cic2 = NULL;
		Register_Addr_Opd * sp_reg = new Register_Addr_Opd( machine_desc_object.spim_register_table[sp]);
		Const_Opd<int>* ca1 = new Const_Opd<int>(sp_size);
		cic1= new Compute_IC_Stmt(sub,sp_reg,ca1,sp_reg);
		result->append_ics(*cic1);
		Label_IC_Stmt * gtst1 = new Label_IC_Stmt(jal,procedure_name);
		result->append_ics(*gtst1);
		cic2= new Compute_IC_Stmt(add,sp_reg,ca1,sp_reg);
		result->append_ics(*cic2);
	}
	else{
		Label_IC_Stmt * gtst1 = new Label_IC_Stmt(jal,procedure_name);
		result->append_ics(*gtst1);
	}
	if(proc->get_return_type()!=void_data_type){
		Move_IC_Stmt* mic1 = NULL;
		Register_Descriptor *  return_reg = NULL;
		if(proc->get_return_type()==double_data_type){
			return_reg =  machine_desc_object.spim_register_table[f0];
		}
		else {
			return_reg =  machine_desc_object.spim_register_table[v1];
		}
		Register_Addr_Opd  *return_rd = new Register_Addr_Opd(return_reg);
		Register_Descriptor * res_reg = my_new_reg(proc->get_return_type());
		Register_Addr_Opd  *res_rd = new Register_Addr_Opd(res_reg);
		if(proc->get_return_type()==int_data_type)mic1 = new Move_IC_Stmt(mov,res_rd,return_rd);
		if(proc->get_return_type()==double_data_type)mic1 = new Move_IC_Stmt(move_d,res_rd,return_rd);
		result->append_ics(*mic1);
		result->set_reg(res_reg);
	}
	if(return_value_reg==NULL)(result->get_reg())->reset_use_for_expr_result();
	// cout<<"Call Ast return\n";
	return *result;
}
Code_For_Ast & Call_Ast::compile_and_optimize_ast(Lra_Outcome & lra){}

///////////////////////////////////////////Return_Ast
Code_For_Ast & Return_Ast::compile(){
	string epilogue_fn = "epilogue_"+proc_name;
	Label_IC_Stmt * gtst1 = new Label_IC_Stmt(j, epilogue_fn);
	
	if(node_data_type == void_data_type){
		Code_For_Ast * result = new Code_For_Ast();
		result->append_ics(*gtst1);
		return *result;
	}


	Code_For_Ast * return_value_cast = &(return_value->compile());
	Register_Addr_Opd* return_value_rd = new Register_Addr_Opd(return_value_cast->get_reg());
	Move_IC_Stmt* mic1 = NULL;
	Register_Descriptor * res_reg = machine_desc_object.spim_register_table[v1]; 
	if(node_data_type == int_data_type){
		res_reg =  machine_desc_object.spim_register_table[v1];
		Register_Addr_Opd  *res_rd = new Register_Addr_Opd(res_reg);
		mic1 = new Move_IC_Stmt(mov,res_rd,return_value_rd);
		//mov or move??
	}
	else if(node_data_type == double_data_type){
		res_reg =  machine_desc_object.spim_register_table[f0];
		Register_Addr_Opd  *res_rd = new Register_Addr_Opd(res_reg);
		mic1 = new Move_IC_Stmt(move_d,res_rd,return_value_rd);
	}

	(return_value_cast->get_reg())->reset_use_for_expr_result();
	Code_For_Ast * result = return_value_cast;
	
	result->append_ics(*mic1);
	result->set_reg(res_reg);
	
	
	result->append_ics(*gtst1);
	// j epilogoue_f1_
	return *result;	

}
Code_For_Ast & Return_Ast::compile_and_optimize_ast(Lra_Outcome & lra){
	
}
