#include "ast.hh"
#include <iostream>
#include <stdlib.h>
using namespace std;

template class Number_Ast<double>;
template class Number_Ast<int>;

void Ast::print_value(Local_Environment & eval_env, ostream & file_buffer){

}

Eval_Result & Ast::get_value_of_evaluation(Local_Environment & eval_env){

}
void Ast::set_value_of_evaluation(Local_Environment & eval_env, Eval_Result & result){

}
Eval_Result & Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
}
///////////////////////

Eval_Result & Assignment_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
		// Eval_Result & x1 = lhs->evaluate(eval_env,file_buffer);

		Eval_Result  & x2 = rhs->evaluate(eval_env,file_buffer);
        // if(!(x2.is_variable_defined())){
        //     cout<<"cs316"<<":"<<" Error Local Variable not defined\n";
        //     exit(0);
        // }
        lhs->set_value_of_evaluation(eval_env,x2);
        print(file_buffer);
        lhs->print_value(eval_env,file_buffer);
		return x2;

}
//////////////

void Name_Ast::print_value(Local_Environment & eval_env, ostream & file_buffer){
        string reval = variable_symbol_entry->get_variable_name();
        // double output;
        file_buffer<<"\n"<<AST_SPACE<<reval<<" : ";
        if(!eval_env.does_variable_exist(reval)){
            if(interpreter_global_table.does_variable_exist(reval)){
                Eval_Result * result = &get_value_of_evaluation(interpreter_global_table);
                bool is_int = 0;

                if(!(result->is_variable_defined())){
                    file_buffer<<"Undefined";
                    return;
                }
                else if(result->get_result_enum()==int_result)is_int =1;
                if (is_int)file_buffer<<result->get_int_value();
                else file_buffer<<result->get_double_value();
            }
            else{
                cout<<"cs316"<<":"<<" Error Variable not defined\n";
                exit(0);
            }
        }
        else{
            Eval_Result * result = &get_value_of_evaluation(eval_env);
            bool is_int = 0;

            if(!(result->is_variable_defined())){
                file_buffer<<"Undefined";
                return;
            }
            else if(result->get_result_enum()==int_result)is_int =1;
            if (is_int)file_buffer<<result->get_int_value();
            else file_buffer<<result->get_double_value();
        }
}
Eval_Result & Name_Ast::get_value_of_evaluation(Local_Environment & eval_env){
    string reval = variable_symbol_entry->get_variable_name();
    Eval_Result * result = eval_env.get_variable_value(reval);
    if(!(result->is_variable_defined())){
        cout<<"cs316"<<":"<<" Error Local Variable not defined\n";
        exit(0);
    }
    return *(result);
}
void Name_Ast::set_value_of_evaluation(Local_Environment & eval_env, Eval_Result & result){
    string reval = variable_symbol_entry->get_variable_name();
    if(!eval_env.does_variable_exist(reval)){
        if(interpreter_global_table.does_variable_exist(reval)){
                interpreter_global_table.put_variable_value(result,reval);
        }
        else{
            cout<<"cs316"<<":"<<" Error Variable not defined\n";
            exit(0);
        }
    }
    // cout<<reval<<" "<<result.get_int_value()<<" "<<result.get_double_value()<<" rrr\n";
    else {
        result.set_variable_status(1);
        eval_env.put_variable_value(result,reval);
    }
}
Eval_Result & Name_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
    
    string reval = variable_symbol_entry->get_variable_name();
    Eval_Result * result = NULL;

    if(!eval_env.does_variable_exist(reval)){
        if(interpreter_global_table.does_variable_exist(reval)){
                result =&get_value_of_evaluation(interpreter_global_table);
        }
        // cout<<interpreter_global_table.does_variable_exist(reval)<<" hbdsjv\n";
        else{
            cout<<"cs316"<<":"<<" Error Variable not defined\n";
            exit(0);
        }
    }
    else result =&get_value_of_evaluation(eval_env);
    // if(eval_env.is_variable_defined(reval)){
        
    // }else{
    //     // result->set_variable_status(0);
    //     // set_value_of_evaluation(eval_env,*result);
    //     // print_value(eval_env,file_buffer);
    // }
   
    return *result;
}

///////////////////////////
template <class T>
Eval_Result & Number_Ast<T>::evaluate(Local_Environment & eval_env, ostream & file_buffer){
    Eval_Result * result =NULL;
    if(node_data_type==int_data_type){
        result = new Eval_Result_Value_Int();
        result->set_variable_status(1);
        result->set_value(constant);  
        result->set_result_enum(int_result);
    }
    else if(node_data_type==double_data_type){
        result = new Eval_Result_Value_Double();
        result->set_variable_status(1);
        result->set_value(constant);  
        result->set_result_enum(double_result);
    }
    return *result;
}
/////////////////////////
Eval_Result & Arithmetic_Expr_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
    

}
/////////////////////////

Eval_Result & Plus_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
    Eval_Result & lhs_result= lhs->evaluate(eval_env,file_buffer);
    Eval_Result & rhs_result= rhs->evaluate(eval_env,file_buffer);
    Eval_Result * result  =NULL;
     
    bool is_int = 0;
    double output,output1,output2;
    if(lhs_result.get_result_enum()==int_result)is_int =1;
    if(is_int){
        int o1,o2,o;
        o1 = lhs_result.get_int_value();
        o2 = rhs_result.get_int_value();
        o = o1+o2;
        // output = (int)output;
        result = new Eval_Result_Value_Int();
        result->set_variable_status(1);
        result->set_value(o);  
        result->set_result_enum(int_result);
    }
    else{
        output1 = lhs_result.get_double_value();
        output2 = rhs_result.get_double_value();
        output = output1+output2;
        result = new Eval_Result_Value_Double();
        result->set_variable_status(1);
        result->set_value(output);  
        result->set_result_enum(double_result);
    }
    // cout<<"\no o1 o2 "<<output<<" "<<output1<<" "<<output2<<"\n";
    return *result;
}

////////////

Eval_Result & Minus_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
    Eval_Result & lhs_result= lhs->evaluate(eval_env,file_buffer);
    Eval_Result & rhs_result= rhs->evaluate(eval_env,file_buffer);
    Eval_Result * result  =NULL;
     
    bool is_int = 0;
    double output,output1,output2;
    if(lhs_result.get_result_enum()==int_result)is_int =1;
    if(is_int){
        int o1,o2,o;
        o1 = lhs_result.get_int_value();
        o2 = rhs_result.get_int_value();
        o = o1-o2;
        // output = (int)output;
        result = new Eval_Result_Value_Int();
        result->set_variable_status(1);
        result->set_value(o);  
        result->set_result_enum(int_result);
    }
    else{
        output1 = lhs_result.get_double_value();
        output2 = rhs_result.get_double_value();
        output = output1-output2;
        result = new Eval_Result_Value_Double();
        result->set_variable_status(1);
        result->set_value(output);  
        result->set_result_enum(double_result);
    }
    // cout<<"\no o1 o2 "<<output<<" "<<output1<<" "<<output2<<"\n";
    return *result;
}

////////////
Eval_Result & Divide_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
    Eval_Result & lhs_result= lhs->evaluate(eval_env,file_buffer);
    Eval_Result & rhs_result= rhs->evaluate(eval_env,file_buffer);
    Eval_Result * result  =NULL;
     
    bool is_int = 0;
    double output,output1,output2;
    if(lhs_result.get_result_enum()==int_result)is_int =1;
    if(is_int){
        // output1 = lhs_result.get_int_value();
        // output2 = rhs_result.get_int_value();
        
        int o1,o2,o;
        o1 = lhs_result.get_int_value();
        o2 = rhs_result.get_int_value();
        if(o2 == 0){
            cout<<"cs316"<<":"<<" Error Divide by 0\n";
            exit(0);
        }
        o = o1/o2;
        // output = (int)output;
        result = new Eval_Result_Value_Int();
        result->set_variable_status(1);
        result->set_value(o);  
        result->set_result_enum(int_result);
    }
    else{
        output1 = lhs_result.get_double_value();
        output2 = rhs_result.get_double_value();
        if(output2 == 0){
            cout<<"cs316"<<":"<<" Error Divide by 0\n";
            exit(0);
        }
        output = output1/output2;
        result = new Eval_Result_Value_Double();
        result->set_variable_status(1);
        result->set_value(output);  
        result->set_result_enum(double_result);
    }
    // cout<<"\no o1 o2 "<<output<<" "<<output1<<" "<<output2<<"\n";
    return *result;
    // if(output2 == 0){
    //     cout<<"cs316"<<":"<<" Error Divide by 0\n";
    //     exit(0);
    // }
}
//////////
Eval_Result & Mult_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
    Eval_Result & lhs_result= lhs->evaluate(eval_env,file_buffer);
    Eval_Result & rhs_result= rhs->evaluate(eval_env,file_buffer);
    Eval_Result * result  =NULL;
     
    bool is_int = 0;
    double output,output1,output2;
    if(lhs_result.get_result_enum()==int_result)is_int =1;
    if(is_int){
        
        int o1,o2,o;
        o1 = lhs_result.get_int_value();
        o2 = rhs_result.get_int_value();
        o = o1*o2;
        // output = (int)output;
        result = new Eval_Result_Value_Int();
        result->set_variable_status(1);
        result->set_value(o);  
        result->set_result_enum(int_result);
    }
    else{
        output1 = lhs_result.get_double_value();
        output2 = rhs_result.get_double_value();
        output = output1*output2;
        result = new Eval_Result_Value_Double();
        result->set_variable_status(1);
        result->set_value(output);  
        result->set_result_enum(double_result);
    }
    // cout<<"\no o1 o2 "<<output<<" "<<output1<<" "<<output2<<"\n";
    return *result;
}
///////////
Eval_Result & UMinus_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
    Eval_Result & lhs_result= lhs->evaluate(eval_env,file_buffer);
    Eval_Result * result  =NULL;

    bool is_int = 0;
    double output;
    if(lhs_result.get_result_enum()==int_result)is_int =1;
    if(is_int){
        int o;
        o = -lhs_result.get_int_value();
        result = new Eval_Result_Value_Int();
        result->set_variable_status(1);
        result->set_value(o);  
        result->set_result_enum(int_result);
    }
    else{
        output = -lhs_result.get_double_value();
        result = new Eval_Result_Value_Double();
        result->set_variable_status(1);
        result->set_value(output);  
        result->set_result_enum(double_result);
    }
    return *result;
}
/////////
Eval_Result & Conditional_Expression_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
    Eval_Result & cond_result= cond->evaluate(eval_env,file_buffer);
    Eval_Result * result = NULL;
    if(cond_result.get_int_value()){    
            result = &(lhs->evaluate(eval_env,file_buffer));
    }
    else{
        result = &(rhs->evaluate(eval_env,file_buffer));
    }
    return * result;
}

//////////////
Eval_Result & Relational_Expr_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
    Eval_Result & lhs_result= lhs_condition->evaluate(eval_env,file_buffer);
    Eval_Result & rhs_result= rhs_condition->evaluate(eval_env,file_buffer);
    Eval_Result * result =NULL;
    bool is_int = 0;
    if(lhs_result.get_result_enum()==int_result)is_int =1;
    int output;
    if (rel_op == less_equalto){
        if(is_int) output = lhs_result.get_int_value()<=rhs_result.get_int_value();
        else output = lhs_result.get_double_value()<=rhs_result.get_double_value();
    }
    else if (rel_op == less_than){
        if(is_int) output = lhs_result.get_int_value()<rhs_result.get_int_value();
        else output = lhs_result.get_double_value()<rhs_result.get_double_value();
    }
    else if (rel_op == greater_than){
        if(is_int) output = lhs_result.get_int_value()>rhs_result.get_int_value();
        else output = lhs_result.get_double_value()>rhs_result.get_double_value();
    }
    else if (rel_op == greater_equalto){
        if(is_int) output = lhs_result.get_int_value()>=rhs_result.get_int_value();
        else output = lhs_result.get_double_value()>=rhs_result.get_double_value();
    }
    else if (rel_op == equalto){
        if(is_int) output = lhs_result.get_int_value()==rhs_result.get_int_value();
        else output = lhs_result.get_double_value()==rhs_result.get_double_value();
    }
    else if (rel_op == not_equalto){
        if(is_int) output = lhs_result.get_int_value()!=rhs_result.get_int_value();
        else output = lhs_result.get_double_value()!=rhs_result.get_double_value();
    }
    // lhs_result.set_result_enum(int_result);
    // lhs_result.set_value(output);
    
    result = new Eval_Result_Value_Int();
    result->set_variable_status(1);
    result->set_value(output);  
    result->set_result_enum(int_result);

    return *result;
}
////////////
Eval_Result & Logical_Expr_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
    // Eval_Result * result=NULL;
    Eval_Result * result=NULL;
    Eval_Result & rhs_result= rhs_op->evaluate(eval_env,file_buffer);
    int output;
    if (bool_op==_logical_not){
        output = !rhs_result.get_int_value();
        result = new Eval_Result_Value_Int();
        result->set_variable_status(1);
        result->set_value(output);  
        result->set_result_enum(int_result);
        return *result;
       
    }
    Eval_Result & lhs_result= lhs_op->evaluate(eval_env,file_buffer);
    
    
    if(bool_op == _logical_or){
        output = lhs_result.get_int_value() || rhs_result.get_int_value();
    }
    else if(bool_op == _logical_and){
        output = lhs_result.get_int_value() && rhs_result.get_int_value();
    }	

    result = new Eval_Result_Value_Int();
    result->set_variable_status(1);
    result->set_value(output);  
    result->set_result_enum(int_result);

    return *result;
}
//////////
Eval_Result & Selection_Statement_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){

    Eval_Result & cond_result= cond->evaluate(eval_env,file_buffer);
    // file_buffer<<"\n"<<AST_NODE_SPACE<<"IF_ELSE";
	Eval_Result * result = NULL;
    if(cond_result.get_int_value()){    
    	    result = &(then_part->evaluate(eval_env,file_buffer));
    }
    else if(else_part ==NULL){
            return * result;
    }
    else{
    	// file_buffer<<"\n"<<AST_NODE_SPACE<<"IF_ELSE";
            result = &(else_part->evaluate(eval_env,file_buffer));
    }
    return * result;

}
///////////
Eval_Result & Iteration_Statement_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
    // Eval_Result * result = NULL;

    // Eval_Result& cond_result= cond->evaluate(eval_env,file_buffer);
    // // cout<<"cond_result.get_int_value(): "<<cond_result.get_int_value()<<"\n";
    // while(cond_result.get_int_value()){
    // 	result = &(body->evaluate(eval_env,file_buffer));
    //     cond_result = cond->evaluate(eval_env,file_buffer);
    // }
    // return *result;
    Eval_Result * result = NULL;
    Eval_Result * cond_result=NULL;
    if(is_do_form){
        result = &(body->evaluate(eval_env,file_buffer));
        cond_result= &cond->evaluate(eval_env,file_buffer);
    }
    else{
        cond_result= &cond->evaluate(eval_env,file_buffer);
    }
    while(cond_result->get_int_value()){
    	result = &(body->evaluate(eval_env,file_buffer));
        cond_result= &cond->evaluate(eval_env,file_buffer);
    }
    return *result;
}
////////////
Eval_Result & Sequence_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
    list<Ast*> ::iterator it; 
    Eval_Result *result=NULL;
	for(it= statement_list.begin();it!= statement_list.end();it++){
		// file_buffer<<"\n";
		result = &((*it)->evaluate(eval_env,file_buffer));
	}
	return *result;
}

//////////////Call_Ast
Eval_Result & Call_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){

}
/////////Return_Ast
Eval_Result & Return_Ast::evaluate(Local_Environment & eval_env, ostream & file_buffer){
    
}