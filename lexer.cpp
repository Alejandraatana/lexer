#include <iostream>
#include <fstream>
#include <set>
#include <map>
#include <utility>
#include <unordered_map>
#include <cstring>
#include <string>
#include <cctype>

using namespace std;
using std::cout;
using std::endl;
using std::ifstream;
using std::set;
using std::map;
using std::make_pair;
using std::pair;
using std::unordered_map;

struct DFA
{
    unsigned int startingState = {0};
    set<unsigned int> finalStates;
    map<pair<unsigned int, char>, unsigned int> transitionTable;
};

void dfaAddTransition(DFA& dfa, unsigned int state, char symbol, unsigned int nextState)
{
    dfa.transitionTable.emplace(make_pair(pair<unsigned int, char>(state, symbol), nextState));
}

void dfaAddFinalState(DFA& dfa, unsigned int state)
{
    dfa.finalStates.emplace(state);
}


struct Lexer
{
	unordered_map<string,pair<string,string>> symbolTable;
	DFA dfa_id;
	DFA dfa_num;
	DFA dfa_menor;
	DFA dfa_mayor;
	DFA dfa_iguali;
	DFA dfa_suma;
	DFA dfa_resta;
	DFA dfa_multi;
	DFA dfa_div;
	DFA dfa_mod;
	DFA dfa_and;
	DFA dfa_or;
	DFA dfa_asi;
	DFA dfa_parizq;
	DFA dfa_parder;
	DFA dfa_corizq;
	DFA dfa_corder;
	DFA dfa_llaveizq;
	DFA dfa_llaveder;
	DFA dfa_puntocoma;
	DFA dfa_space;
	DFA dfa_saltol;
	DFA dfa_tab;
};

void inicializaLexer(Lexer& lex)
{
	lex.symbolTable.emplace(make_pair("if",make_pair("IF","")));
	lex.symbolTable.emplace(make_pair("else",make_pair("ELSE","")));
	lex.symbolTable.emplace(make_pair("do",make_pair("DO","")));
	lex.symbolTable.emplace(make_pair("while",make_pair("WHILE","")));
	lex.symbolTable.emplace(make_pair("break",make_pair("BREAK","")));
	lex.symbolTable.emplace(make_pair("num",make_pair("NUM","")));
	lex.symbolTable.emplace(make_pair("bool",make_pair("BOOL","")));
	lex.symbolTable.emplace(make_pair("false",make_pair("FALSE","")));
	lex.symbolTable.emplace(make_pair("true",make_pair("TRUE","")));

	dfaAddTransition(lex.dfa_id,0,'l',1);
	dfaAddTransition(lex.dfa_id,1,'l',1);
	dfaAddTransition(lex.dfa_id,1,'n',1);
	dfaAddFinalState(lex.dfa_id,1);

	dfaAddTransition(lex.dfa_puntocoma,0,';',1);
	dfaAddFinalState(lex.dfa_puntocoma,1);

	dfaAddTransition(lex.dfa_space,0,' ',1);
	dfaAddFinalState(lex.dfa_space,1);

	dfaAddTransition(lex.dfa_saltol,0,'\n',1);
	dfaAddFinalState(lex.dfa_saltol,1);

	dfaAddTransition(lex.dfa_tab,0,'\t',1);
	dfaAddFinalState(lex.dfa_tab,1);
}

struct Token
{
	string nombre;
	string atributo;
};


string dfaStart(DFA& dfa, std::ifstream& inputStream)
{
	unsigned int currentState = dfa.startingState;
	string word;

    while(inputStream.good())
    {
        char symbol = inputStream.get();
	char symbolAux=symbol;

	if(isalpha(symbol)|| symbol=='_')
	{
		symbol='l';
	}
	if(isdigit(symbol))
	{
		symbol='n';
	}

	 pair<unsigned int, char> transition = {currentState, symbol};

        if(dfa.transitionTable.count(transition) == 0)
        {
            // Backtrack
	    inputStream.putback(symbolAux);
            if(dfa.finalStates.count(currentState) > 0)
            {
	       return word;
	    }
	    else
	    {
		    return "No aceptado";
	    }
        }
	else
	{
		word=word+symbolAux;
	}
        currentState = dfa.transitionTable[transition];
    }
}

void addSymbolTable()
{

}


Token getNextToken(Lexer& lexer,std::ifstream& inputStream)
{
	string cad;
	int cont_sl=0;
	Token token;
	while(inputStream.good())
	{
		cad=dfaStart(lexer.dfa_id,inputStream);
		if(cad!="No aceptado")
		{
			token.nombre="ID";
			token.atributo=cad;
			return token;
		}
		cad=dfaStart(lexer.dfa_puntocoma,inputStream);
		if(cad!="No aceptado")
		{
			token.nombre="PC";
			token.atributo=cad;
			return token;
		}
		cad=dfaStart(lexer.dfa_saltol,inputStream);
		if(cad!="No aceptado")
		{
			token.nombre="SL";
			token.atributo=cad;
			return token;
		}
		if(cad=="No aceptado")
		{
			cout<< "Error en la linea n" <<endl;
			exit(1);
		}
	}

}


int main()
{
	Lexer lexer;
	Token tok;
	inicializaLexer(lexer);
	ifstream inputStream("programa.txt");
	while(inputStream.good())
	{
		cout<< "a" <<endl;
		/*tok=getNextToken(lexer,inputStream);
		cout<< tok.nombre << " " << tok.atributo <<endl;
		tok=getNextToken(lexer,inputStream);
		cout<< tok.nombre << " " << tok.atributo <<endl;
		tok=getNextToken(lexer,inputStream);
		cout<< tok.nombre << " " << tok.atributo <<endl;*/
	}

	/*for(auto& i:lexer.symbolTable)
	{
		cout << i.first << "," << i.second.first << "," <<i.second.second  << endl;
	}*/

	inputStream.close();
return 0;
}
