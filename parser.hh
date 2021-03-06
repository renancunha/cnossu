#ifndef PARSER_HH_
# define PARSER_HH_

#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <stack>
#include <list>

extern char* yytext;
extern void yyerror(const char *);

struct symrec {
	std::string tipo;
	std::string id;
};

struct if_instr {
    int goto_false;
    int goto_end_true;
};

struct while_instr {
	int goto_start;
	int goto_false;
};

struct quadrupla {
	std::string op;
	std::string arg1;
	std::string arg2;
	std::string resultado;
};

class Parser
{
public:
	

	void seta_tipo();
	void declara_var();
	void existe_var();
	void verifica_existe();

	void stack_push();
	void code_atribuicao();
	void code_algebrica();
	void code_logica();

	int code_if();
	int code_while();

	void code_switch_case();
	void code_switch_label_case();
	void code_switch_break();
	void code_switch_default_break();
	void code_switch_inicio();
	void code_switch_fim();

	void print_int_code();

	if_instr* get_if_instr();
	while_instr* get_while_instr();
	int get_code_offset();
	int gen_label();
	int gen_goto();
	int gen_goto(int cod_pos);
	void backpatch(int pos_corrigir, int label);

private:
	std::map<std::string, symrec> _tab_sim;
	std::stack<std::string> _stack;
	std::stack<int> _stack_switch_fim;
	std::stack<int> _stack_backpatching;
	
	std::string tipo_atual;
	std::list<quadrupla> _cod_int;

	int _g_idx = 0;
	int _g_label_idx = 0;
	int _g_switch_idx = 0;

};


#endif /* !PARSER_HH_ */