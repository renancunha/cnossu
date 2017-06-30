#include "parser.hh"

void Parser::seta_tipo ()
{
	tipo_atual = yytext;
}

void Parser::verifica_existe()
{
	std::string id = yytext;
	std::map<std::string, symrec>::iterator i = _tab_sim.find(id);
	if(i == _tab_sim.end()) {
		std::string erro = "Variavel "  + id + " não declarada.";
		yyerror(erro.c_str());
		return;
	}
}

void Parser::declara_var () 
{
	std::string id = yytext;
	std::map<std::string, symrec>::iterator i = _tab_sim.find(id);

	// Variavel ja foi declarada
	if(i != _tab_sim.end()) {
		std::string erro = "Variavel "  + id + " já foi declarada.";
		yyerror(erro.c_str());
		return;
	}

	// Add o novo simbolo
	symrec sym;
	sym.id = id;
	sym.tipo = tipo_atual;

	_tab_sim[id] = sym;

}

void Parser::existe_var()
{
	std::string id = yytext;
	std::map<std::string, symrec>::iterator i = _tab_sim.find(id);
	if(i == _tab_sim.end()) {
		std::string erro = "Variavel nao foi declarada: " + id;
		yyerror(erro.c_str());
		return;
	}
}

void Parser::stack_push()
{
	_stack.push(yytext);
}

void Parser::code_atribuicao()
{
	std::string arg2 = _stack.top(); _stack.pop();
	_stack.pop();
	std::string arg1 = _stack.top(); _stack.pop();
	std::string str = arg1 + "\t=\t" + arg2;

	quadrupla quad;
	quad.op = "=";
	quad.arg1 = arg2;
	quad.resultado = arg1;
	_cod_int.push_back(quad);

	//std::cout << str << "\n";
}

void Parser::code_algebrica()
{
	std::string tmp = "t" + std::to_string(_g_idx);
	std::string arg2 = _stack.top(); _stack.pop();
	std::string sinal = _stack.top(); _stack.pop();
	std::string arg1 = _stack.top(); _stack.pop();
	//std::string str = tmp + "\t=\t" + arg1 + "\t" + sinal + "\t" + arg2;
	_stack.push(tmp);

	//std::cout << str << "\n";

	quadrupla quad;
	quad.op = sinal;
	quad.arg1 = arg1;
	quad.arg2 = arg2;
	quad.resultado = tmp;
	_cod_int.push_back(quad);

	_g_idx++;
}

void Parser::code_logica()
{
	std::string tmp = "t" + std::to_string(_g_idx);
	std::string arg2 = _stack.top(); _stack.pop();
	std::string sinal = _stack.top(); _stack.pop();
	std::string arg1 = _stack.top(); _stack.pop();
	//std::string str = tmp + "\t=\t" + arg1 + "\t" + sinal + "\t" + arg2;
	_stack.push(tmp);

	//std::cout << str << "\n";

	quadrupla quad;
	quad.op = sinal;
	quad.arg1 = arg1;
	quad.arg2 = arg2;
	quad.resultado = tmp;
	_cod_int.push_back(quad);

	_g_idx++;
}

void Parser::code_if()
{
	// Adiciona o IF na lista de quadruplas
	std::string exp = _stack.top(); _stack.pop();
	quadrupla quad_if;
	quad_if.op = "IF";
	quad_if.arg1 = exp;
	std::string label = "L" + std::to_string(_g_label_idx);
	_g_label_idx++;
	quad_if.resultado = "GOTO " + label;
	_cod_int.push_back(quad_if);

	// Adiciona um GOTO caso a condicao nao foi satisfeita
	// Aqui precisamos tratar o label usando backpatching
	quadrupla quad_goto;
	quad_goto.op = "GOTO";
	quad_goto.arg1 = "XX";
	_cod_int.push_back(quad_goto);	
	// Adiciona o indice atual da lista _cod_int para ser corrigida no backpatching
	_stack_backpatching.push(_cod_int.size() - 1);


	// Adiciona o label de quando a condicao foi satisfeita
	quadrupla quad_label;
	quad_label.op = "LABEL";
	quad_label.arg1 = label;
	_cod_int.push_back(quad_label);

}

void Parser::code_if_else()
{
	// Gera um label que sera corrigido via backpatching
	std::string label = "L" + std::to_string(_g_label_idx);
	_g_label_idx++;
	quadrupla quad_label;
	quad_label.op = "LABEL";
	quad_label.arg1 = label;
	_cod_int.push_back(quad_label);

	// Agora que geramos a label, vamos pegar o GOTO que precisa ser corrigido via backpatching
	int pos_retrocorrigir = _stack_backpatching.top();
	_stack_backpatching.pop();

	std::list<quadrupla>::iterator it = std::next(_cod_int.begin(), pos_retrocorrigir);
	(*it).arg1 = label;
}

void Parser::code_if_fim()
{
	code_if_else();
}

void Parser::code_while_inicio()
{
	// Gera uma label para o inicio do código
	std::string label_inicio = "L" + std::to_string(_g_label_idx);
	_g_label_idx++;
	quadrupla quad_label;
	quad_label.op = "LABEL";
	quad_label.arg1 = label_inicio;
	_cod_int.push_back(quad_label);

	// Adiciona o endereco dessa quadrupla na pilha para o loop do while
	// A ultima coisa dentro do corpo do while é um GOTO para este label, logo,
	// precisamos guardar este label em algum local...
	_stack_while_retorno.push(_cod_int.size() - 1);
}

void Parser::code_while()
{
	// Adiciona o IF na lista de quadruplas
	std::string exp = _stack.top(); _stack.pop();
	quadrupla quad_if;
	quad_if.op = "IF";
	quad_if.arg1 = exp;
	std::string label = "L" + std::to_string(_g_label_idx);
	_g_label_idx++;
	quad_if.resultado = "GOTO " + label;
	_cod_int.push_back(quad_if);

	// Adiciona um GOTO caso a condicao nao foi satisfeita
	// Aqui precisamos tratar o label usando backpatching
	quadrupla quad_goto;
	quad_goto.op = "GOTO";
	quad_goto.arg1 = "XX";
	_cod_int.push_back(quad_goto);	
	// Adiciona o indice atual da lista _cod_int para ser corrigida no backpatching
	_stack_backpatching.push(_cod_int.size() - 1);


	// Adiciona o label de quando a condicao foi satisfeita
	quadrupla quad_label;
	quad_label.op = "LABEL";
	quad_label.arg1 = label;
	_cod_int.push_back(quad_label);
}

void Parser::code_while_fim()
{
	// Precisa adicionar um GOTO para o inicio do while
	// Pega o LABEL para o inicio do while atual
	int pos_while_retorno = _stack_while_retorno.top();
	_stack_while_retorno.pop();

	// Procura o local que esta este LABEL
	std::list<quadrupla>::iterator it_while_retorno = std::next(_cod_int.begin(), pos_while_retorno);
	quadrupla quad_goto;
	quad_goto.op = "GOTO";
	quad_goto.arg1 = (*it_while_retorno).arg1;
	_cod_int.push_back(quad_goto);	

	// Gera um label que sera corrigido via backpatching
	std::string label = "L" + std::to_string(_g_label_idx);
	_g_label_idx++;
	quadrupla quad_label;
	quad_label.op = "LABEL";
	quad_label.arg1 = label;
	_cod_int.push_back(quad_label);

	// Agora que geramos a label, vamos pegar o GOTO que precisa ser corrigido via backpatching
	int pos_retrocorrigir = _stack_backpatching.top();
	_stack_backpatching.pop();

	std::list<quadrupla>::iterator it_backpatching = std::next(_cod_int.begin(), pos_retrocorrigir);
	(*it_backpatching).arg1 = label;
}

void Parser::code_switch_case()
{
	std::string exp = _stack.top(); 
	std::string case_val = yytext;

 	// Gera uma expressao para cada case
	quadrupla quad_exp;
	quad_exp.op = "==";
	quad_exp.arg1 = exp;
	quad_exp.arg2 = case_val;
	std::string tmp = "t" + std::to_string(_g_idx);
	_g_idx++;
	quad_exp.resultado = tmp;
	_cod_int.push_back(quad_exp);
	_stack.push(tmp);

	// Gera o IF
	std::string expif = _stack.top(); _stack.pop();
	quadrupla quad_if;
	quad_if.op = "IF";
	quad_if.arg1 = expif;
	std::string label = "L" + std::to_string(_g_label_idx);
	_g_label_idx++;
	quad_if.resultado = "GOTO " + label;
	_cod_int.push_back(quad_if);

	// Gera o GOTO caso o IF nao for satisfeito
	quadrupla quad_goto;
	quad_goto.op = "GOTO";
	quad_goto.arg1 = "XX";
	_cod_int.push_back(quad_goto);	
	// Adiciona o indice atual da lista _cod_int para ser corrigida no backpatching
	_stack_backpatching.push(_cod_int.size() - 1);

	quadrupla quad_label;
	quad_label.op = "LABEL";
	quad_label.arg1 = label;
	_cod_int.push_back(quad_label);
}

void Parser::code_switch_break()
{
	quadrupla quad_goto;
	quad_goto.op = "GOTO";
	quad_goto.arg1 = "XX" + std::to_string(_g_switch_idx);
	_cod_int.push_back(quad_goto);	
	// Adiciona o indice atual da lista _cod_int para ser corrigida no backpatching
	_stack_switch_fim.push(_cod_int.size() - 1);
}

void Parser::code_switch_label_case()
{
	if(_stack_backpatching.size() > 0)
	{

		int pos_retrocorrigir = _stack_backpatching.top();
		_stack_backpatching.pop();

		std::string label = "L" + std::to_string(_g_label_idx);
		_g_label_idx++;

		quadrupla quad_label;
		quad_label.op = "LABEL";
		quad_label.arg1 = label;
		_cod_int.push_back(quad_label);

		std::list<quadrupla>::iterator it_backpatching = std::next(_cod_int.begin(), pos_retrocorrigir);
		(*it_backpatching).arg1 = label;
	}

}

void Parser::code_switch_inicio()
{
	_g_switch_idx++;
}

void Parser::code_switch_fim()
{
	std::string label = "L" + std::to_string(_g_label_idx);
	_g_label_idx++;
	quadrupla quad_label;
	quad_label.op = "LABEL";
	quad_label.arg1 = label;
	_cod_int.push_back(quad_label);

	if(_stack_switch_fim.size() > 0)
	{

		int pos_retrocorrigir = _stack_switch_fim.top();
		_stack_switch_fim.pop();
		std::list<quadrupla>::iterator it_backpatching = std::next(_cod_int.begin(), pos_retrocorrigir);
		std::string str = "XX" + std::to_string(_g_switch_idx);

		while((*it_backpatching).arg1 == str)
		{		

			(*it_backpatching).arg1 = label;

			if(_stack_switch_fim.size() == 0)
				break;

			pos_retrocorrigir = _stack_switch_fim.top();
			_stack_switch_fim.pop();
			it_backpatching = std::next(_cod_int.begin(), pos_retrocorrigir);

		}

	}
	_g_switch_idx--;
}

void Parser::print_int_code()
{
	std::list<quadrupla>::iterator it;
	int i = 0;
	std::cout << "\n\nN\tOp\tArg1\tArg2\tResultado\n";
    for(it = _cod_int.begin(); it != _cod_int.end(); it++) {
    	std::cout << "(" + std::to_string(i) + ")\t" + (*it).op + "\t" + (*it).arg1 + "\t" + (*it).arg2 + "\t" + (*it).resultado + "\n";
    	i++;
    }
    std::cout << "\n";

}

