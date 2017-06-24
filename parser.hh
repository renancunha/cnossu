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

	void stack_push();
	void code_atribuicao();
	void code_algebrica();
	void code_logica();
	void code_if();
	void code_if_else();
	void code_if_fim();

	void print_int_code();

private:
	std::map<std::string, symrec> _tab_sim;
	std::stack<std::string> _stack;

	std::stack<int> _stack_backpatching;
	

	std::string tipo_atual;
	std::list<quadrupla> _cod_int;
	int _g_idx = 0;
	int _g_label_idx = 0;

};


#endif /* !PARSER_HH_ */