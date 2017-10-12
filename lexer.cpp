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

int cont_sl=0;

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
	//inicializa la tabla de simbolos
	lex.symbolTable.emplace(make_pair("if",make_pair("IF","")));
	lex.symbolTable.emplace(make_pair("else",make_pair("ELSE","")));
	lex.symbolTable.emplace(make_pair("do",make_pair("DO","")));
	lex.symbolTable.emplace(make_pair("while",make_pair("WHILE","")));
	lex.symbolTable.emplace(make_pair("break",make_pair("BREAK","")));
	lex.symbolTable.emplace(make_pair("num",make_pair("NUM","")));
	lex.symbolTable.emplace(make_pair("bool",make_pair("BOOL","")));
	lex.symbolTable.emplace(make_pair("false",make_pair("FALSE","")));
	lex.symbolTable.emplace(make_pair("true",make_pair("TRUE","")));

	//automata de identificadores
	dfaAddTransition(lex.dfa_id,0,'l',1);
	dfaAddTransition(lex.dfa_id,1,'l',1);
	dfaAddTransition(lex.dfa_id,1,'n',1);
	dfaAddFinalState(lex.dfa_id,1);

	//automata de numeros
	dfaAddTransition(lex.dfa_num,0,'+',1);
	dfaAddTransition(lex.dfa_num,0,'-',1);
	dfaAddTransition(lex.dfa_num,0,'n',2);
	dfaAddTransition(lex.dfa_num,1,'n',2);
	dfaAddTransition(lex.dfa_num,2,'n',2);
	dfaAddTransition(lex.dfa_num,2,'.',3);
	dfaAddTransition(lex.dfa_num,2,'e',5);
	dfaAddTransition(lex.dfa_num,3,'n',4);
	dfaAddTransition(lex.dfa_num,4,'n',4);
	dfaAddTransition(lex.dfa_num,4,'e',5);
	dfaAddTransition(lex.dfa_num,5,'+',6);
	dfaAddTransition(lex.dfa_num,5,'-',6);
	dfaAddTransition(lex.dfa_num,5,'n',7);
	dfaAddTransition(lex.dfa_num,6,'n',7);
	dfaAddTransition(lex.dfa_num,7,'n',7);
	dfaAddFinalState(lex.dfa_num,2);
	dfaAddFinalState(lex.dfa_num,4);
	dfaAddFinalState(lex.dfa_num,7);

	//automata de < y <=
	dfaAddTransition(lex.dfa_menor,0,'<',1);
	dfaAddTransition(lex.dfa_menor,1,'=',2);
	dfaAddFinalState(lex.dfa_menor,1);
	dfaAddFinalState(lex.dfa_menor,2);

	//automata de > y >=
	dfaAddTransition(lex.dfa_mayor,0,'>',1);
	dfaAddTransition(lex.dfa_mayor,1,'=',2);
	dfaAddFinalState(lex.dfa_mayor,1);
	dfaAddFinalState(lex.dfa_mayor,2);

	//automata de ==
	dfaAddTransition(lex.dfa_iguali,0,'=',1);
	dfaAddTransition(lex.dfa_iguali,1,'=',2);
	dfaAddFinalState(lex.dfa_iguali,2);

	//automata de +
	dfaAddTransition(lex.dfa_suma,0,'+',1);
	dfaAddFinalState(lex.dfa_suma,1);

	//automata de -
	dfaAddTransition(lex.dfa_resta,0,'-',1);
	dfaAddFinalState(lex.dfa_resta,1);

	//automata de *
	dfaAddTransition(lex.dfa_multi,0,'*',1);
	dfaAddFinalState(lex.dfa_multi,1);

	//automata de /
	dfaAddTransition(lex.dfa_div,0,'/',1);
	dfaAddFinalState(lex.dfa_div,1);

	//automata de %
	dfaAddTransition(lex.dfa_mod,0,'%',1);
	dfaAddFinalState(lex.dfa_mod,1);

	//automata de &&
	dfaAddTransition(lex.dfa_and,0,'&',1);	
	dfaAddTransition(lex.dfa_and,1,'&',2);
	dfaAddFinalState(lex.dfa_and,2);

	//automata de ||
	dfaAddTransition(lex.dfa_or,0,'|',1);	
	dfaAddTransition(lex.dfa_or,1,'|',2);
	dfaAddFinalState(lex.dfa_or,2);

	//automata de =
	dfaAddTransition(lex.dfa_asi,0,'=',1);
	dfaAddFinalState(lex.dfa_asi,1);

	//automata de (
	dfaAddTransition(lex.dfa_parizq,0,'(',1);
	dfaAddFinalState(lex.dfa_parizq,1);

	//automata de )
	dfaAddTransition(lex.dfa_parder,0,')',1);
	dfaAddFinalState(lex.dfa_parder,1);

	//automata de [
	dfaAddTransition(lex.dfa_corizq,0,'[',1);
	dfaAddFinalState(lex.dfa_corizq,1);

	//automata de ]
	dfaAddTransition(lex.dfa_corder,0,']',1);
	dfaAddFinalState(lex.dfa_corder,1);

	//automata de {
	dfaAddTransition(lex.dfa_llaveizq,0,'{',1);
	dfaAddFinalState(lex.dfa_llaveizq,1);

	//automata de }
	dfaAddTransition(lex.dfa_llaveder,0,'}',1);
	dfaAddFinalState(lex.dfa_llaveder,1);

	//automata de ;
	dfaAddTransition(lex.dfa_puntocoma,0,';',1);
	dfaAddFinalState(lex.dfa_puntocoma,1);

	//automata de espacio
	dfaAddTransition(lex.dfa_space,0,' ',1);
	dfaAddFinalState(lex.dfa_space,1);

	//automata de salto de linea 
	dfaAddTransition(lex.dfa_saltol,0,'\n',1);
	dfaAddFinalState(lex.dfa_saltol,1);

	//automata de tabulador
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
    return "";
}

void addSymbolTable(Lexer& lexer,Token token)
{
	if(lexer.symbolTable.count(token.nombre)==0)
	{
		lexer.symbolTable.emplace(make_pair(token.nombre,make_pair(token.nombre,token.atributo)));
	}
}


Token getNextToken(Lexer& lexer,std::ifstream& inputStream)
{
	string cad;
	Token token;
	while(inputStream.good())
	{
		//ejecuta el DFA de identificadores
		cad=dfaStart(lexer.dfa_id,inputStream);
		if(cad!="No aceptado")
		{
			token.nombre="ID";
			token.atributo=cad;
			addSymbolTable(lexer,token);
			return token;
		}
		//ejecuta el DFA de numeros
		cad=dfaStart(lexer.dfa_num,inputStream);
		if(cad!="No aceptado")
		{
			token.nombre="ConsNum";
			token.atributo=cad;
			addSymbolTable(lexer,token);
			return token;
		}
		//ejecuta el DFA de < y <=
		cad=dfaStart(lexer.dfa_menor,inputStream);
		if(cad!="No aceptado")
		{
			token.nombre="OpeRel";
			token.atributo=cad;
			return token;
		}
		//ejecuta el DFA de > y >=
		cad=dfaStart(lexer.dfa_mayor,inputStream);
		if(cad!="No aceptado")
		{
			token.nombre="OpeRel";
			token.atributo=cad;
			return token;
		}
		//ejecuta el DFA de ==
		cad=dfaStart(lexer.dfa_iguali,inputStream);
		if(cad!="No aceptado")
		{
			token.nombre="OpeRel";
			token.atributo=cad;
			return token;
		}
		//ejecuta el DFA de +
		cad=dfaStart(lexer.dfa_suma,inputStream);
		if(cad!="No aceptado")
		{
			token.nombre="OpeAri";
			token.atributo=cad;
			return token;
		}
		//ejecuta el DFA de -
		cad=dfaStart(lexer.dfa_resta,inputStream);
		if(cad!="No aceptado")
		{
			token.nombre="OpeAri";
			token.atributo=cad;
			return token;
		}
		//ejecuta el DFA de *
		cad=dfaStart(lexer.dfa_multi,inputStream);
		if(cad!="No aceptado")
		{
			token.nombre="OpeAri";
			token.atributo=cad;
			return token;
		}
		//ejecuta el DFA de /
		cad=dfaStart(lexer.dfa_div,inputStream);
		if(cad!="No aceptado")
		{
			token.nombre="OpeAri";
			token.atributo=cad;
			return token;
		}
		//ejecuta el DFA de %
		cad=dfaStart(lexer.dfa_mod,inputStream);
		if(cad!="No aceptado")
		{
			token.nombre="OpeAri";
			token.atributo=cad;
			return token;
		}
		//ejecuta el DFA de &&
		cad=dfaStart(lexer.dfa_and,inputStream);
		if(cad!="No aceptado")
		{
			token.nombre="OpeLog";
			token.atributo=cad;
			return token;
		}
		//ejecuta el DFA de ||
		cad=dfaStart(lexer.dfa_or,inputStream);
		if(cad!="No aceptado")
		{
			token.nombre="OpeLog";
			token.atributo=cad;
			return token;
		}
		//ejecuta el DFA de =
		cad=dfaStart(lexer.dfa_asi,inputStream);
		if(cad!="No aceptado")
		{
			token.nombre="OpeAsig";
			token.atributo=cad;
			return token;
		}
		//ejecuta el DFA de (
		cad=dfaStart(lexer.dfa_parizq,inputStream);
		if(cad!="No aceptado")
		{
			token.nombre="OpeAgru";
			token.atributo=cad;
			return token;
		}
		//ejecuta el DFA de )
		cad=dfaStart(lexer.dfa_parder,inputStream);
		if(cad!="No aceptado")
		{
			token.nombre="OpeAgru";
			token.atributo=cad;
			return token;
		}
		//ejecuta el DFA de [
		cad=dfaStart(lexer.dfa_corizq,inputStream);
		if(cad!="No aceptado")
		{
			token.nombre="OpeAgru";
			token.atributo=cad;
			return token;
		}
		//ejecuta el DFA de ]
		cad=dfaStart(lexer.dfa_corder,inputStream);
		if(cad!="No aceptado")
		{
			token.nombre="OpeAgru";
			token.atributo=cad;
			return token;
		}
		//ejecuta el DFA de {
		cad=dfaStart(lexer.dfa_llaveizq,inputStream);
		if(cad!="No aceptado")
		{
			token.nombre="OpeAgru";
			token.atributo=cad;
			return token;
		}
		//ejecuta el DFA de }
		cad=dfaStart(lexer.dfa_llaveder,inputStream);
		if(cad!="No aceptado")
		{
			token.nombre="OpeAgru";
			token.atributo=cad;
			return token;
		}
		//ejecuta el DFA de ;
		cad=dfaStart(lexer.dfa_puntocoma,inputStream);
		if(cad!="No aceptado")
		{
			token.nombre="CaraPunt";
			token.atributo=cad;
			return token;
		}
		//ejecuta el DFA de space
		cad=dfaStart(lexer.dfa_space,inputStream);
		if(cad!="No aceptado")
		{
			token.nombre="Space";
			token.atributo=cad;
			return token;
		}
		//ejecuta el DFA de \n
		cad=dfaStart(lexer.dfa_saltol,inputStream);
		if(cad!="No aceptado")
		{
			token.nombre="SalLin";
			token.atributo=cad;
			cont_sl=cont_sl+1;
			return token;
		}
		//ejecuta el DFA de \t
		cad=dfaStart(lexer.dfa_tab,inputStream);
		if(cad!="No aceptado")
		{
			token.nombre="Tab";
			token.atributo=cad;
			return token;
		}
		//Verifica si hubo un error
		if(cad=="No aceptado")
		{
			cout<< "Error en la linea" << " "<<cont_sl <<endl;
			exit(1);
		}
		//Verifica si se termino el archivo
		if(cad=="")
		{
			token.nombre="EOF";
			token.atributo="";
			return token;
		}
	}
}


int main()
{
	Lexer lexer;
	Token tok;
	inicializaLexer(lexer);
	ifstream inputStream("programa.txt");
	tok=getNextToken(lexer,inputStream);
	cout<< tok.nombre << " " << tok.atributo <<endl;
	while(inputStream.good() && tok.nombre!="EOF")
	{
		tok=getNextToken(lexer,inputStream);
		cout<< tok.nombre << " " << tok.atributo <<endl;
	}

	/*cout<<"***************************************************" <<endl;
	for(auto& i:lexer.symbolTable)
	{
		cout << i.first << "," << i.second.first << "," <<i.second.second  << endl;
	}*/
	inputStream.close();
return 0;
}
