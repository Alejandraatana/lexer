#include <iostream>
#include <fstream>
#include <set>
#include <map>
#include <utility>
#include <unordered_map>
#include <cstring>

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

//Estructura del lexer
struct Lexer {
    unordered_map<string,pair<string,string>> Tabla;
    DFA identificadores;
    DFA constantesNum;
    DFA operadorRel;
    DFA operadorArit;
    DFA operadorLog;
    DFA operadorAsig;
    DFA SimboloAgrup;
    DFA PuntoComa;
    DFA caracterBlanco;
    
};

//Inicializo los lexer
void inicializaLexer(Lexer& lexema){
    
    //Tabla de simbolos
    lexema.Tabla.emplace(make_pair("if",make_pair("IF","")));
    lexema.Tabla.emplace(make_pair("else",make_pair("ELSE","")));
    lexema.Tabla.emplace(make_pair("do",make_pair("DO","")));
    lexema.Tabla.emplace(make_pair("while",make_pair("WHILE","")));
    lexema.Tabla.emplace(make_pair("break",make_pair("BREAK","")));
    lexema.Tabla.emplace(make_pair("num",make_pair("NUM","")));
    lexema.Tabla.emplace(make_pair("bool",make_pair("BOOL","")));
    lexema.Tabla.emplace(make_pair("false",make_pair("FALSE","")));
    lexema.Tabla.emplace(make_pair("true",make_pair("TRUE","")));
    
    //Automata de indetificadores
    dfaAddTransition(lexema.identificadores,0,'c',1);
    dfaAddTransition(lexema.identificadores,0,'e',1);
    dfaAddTransition(lexema.identificadores,1,'c',1);
    dfaAddTransition(lexema.identificadores,1,'e',1);
    dfaAddTransition(lexema.identificadores,1,'d',1);
    dfaAddFinalState(lexema.identificadores,1);
    
    //Automata Constantes numericas
    //dfaAddTransition(lexema.constantesNum,0,'+',1);
    dfaAddTransition(lexema.constantesNum,0,'-',1);
    dfaAddTransition(lexema.constantesNum,0,'d',2);
    dfaAddTransition(lexema.constantesNum,1,'d',2);
    dfaAddTransition(lexema.constantesNum,2,'d',2);
    dfaAddTransition(lexema.constantesNum,2,'.',3);
    dfaAddTransition(lexema.constantesNum,2,'e',5);
    dfaAddTransition(lexema.constantesNum,3,'d',4);
    dfaAddTransition(lexema.constantesNum,4,'d',4);
    dfaAddTransition(lexema.constantesNum,4,'e',5);
    dfaAddTransition(lexema.constantesNum,5,'+',6);
    dfaAddTransition(lexema.constantesNum,5,'-',6);
    dfaAddTransition(lexema.constantesNum,5,'d',7);
    dfaAddTransition(lexema.constantesNum,6,'d',7);
    dfaAddTransition(lexema.constantesNum,7,'d',7);
    dfaAddFinalState(lexema.constantesNum,2);
    dfaAddFinalState(lexema.constantesNum,4);
    dfaAddFinalState(lexema.constantesNum,7);
    
    //Automata Operadores Relacionales
    dfaAddTransition(lexema.operadorRel,0,'>',1);
    dfaAddTransition(lexema.operadorRel,0,'<',1);
    dfaAddTransition(lexema.operadorRel,0,'=',3);
    dfaAddTransition(lexema.operadorRel,1,'=',2);
    dfaAddTransition(lexema.operadorRel,3,'=',2);
    dfaAddFinalState(lexema.operadorRel,1);
    dfaAddFinalState(lexema.operadorRel,2);
    
    //Automata Operadores Aritmeticos
    dfaAddTransition(lexema.operadorArit,0,'+',1);
    dfaAddTransition(lexema.operadorArit,0,'-',1);
    dfaAddTransition(lexema.operadorArit,0,'*',1);
    dfaAddTransition(lexema.operadorArit,0,'/',1);
    dfaAddTransition(lexema.operadorArit,0,'%',1);
    dfaAddFinalState(lexema.operadorArit,1);
    
    //Automata Operadores logico
    dfaAddTransition(lexema.operadorLog,0,'&',1);
    dfaAddTransition(lexema.operadorLog,0,'|',2);
    dfaAddTransition(lexema.operadorLog,1,'&',3);
    dfaAddTransition(lexema.operadorLog,2,'|',3);
    dfaAddFinalState(lexema.operadorLog,3);
    
    //Automata Operador de Asignacion
    dfaAddTransition(lexema.operadorAsig,0,'=',1);
    dfaAddFinalState(lexema.operadorAsig,1);
    
    //Automata de Simbolos de agrupacion
    dfaAddTransition(lexema.SimboloAgrup,0,'(',1);
    dfaAddTransition(lexema.SimboloAgrup,0,')',1);
    dfaAddTransition(lexema.SimboloAgrup,0,'[',1);
    dfaAddTransition(lexema.SimboloAgrup,0,']',1);
    dfaAddTransition(lexema.SimboloAgrup,0,'{',1);
    dfaAddTransition(lexema.SimboloAgrup,0,'}',1);
    dfaAddFinalState(lexema.SimboloAgrup,1);
    
    //Automata de punto y coma
    dfaAddTransition(lexema.PuntoComa,0,';',1);
    dfaAddFinalState(lexema.PuntoComa,1);
    
    //Automata caracteres en blanco
    dfaAddTransition(lexema.caracterBlanco,0,' ',1);
    dfaAddTransition(lexema.caracterBlanco,0,'\n',1);
    dfaAddTransition(lexema.caracterBlanco,0,'\t',1);
    dfaAddFinalState(lexema.caracterBlanco,1);

}

string dfaStart(DFA& dfa, std::ifstream& inputStream)
{
    unsigned int currentState = dfa.startingState;
    string cadena;

    while(inputStream.good())
    {
        char symbol = inputStream.get();
        char auxiliar=symbol;
        
        if(isalpha(symbol) || symbol=='_'){
            if(symbol != 'e'){
                symbol = 'c';
            }
        }
        if(isdigit(symbol)){
            symbol = 'd';
        }
        
        pair<unsigned int, char> transition = {currentState, symbol};

        if(dfa.transitionTable.count(transition) == 0)
        {
            // Backtrack
            inputStream.putback(auxiliar);

            if(dfa.finalStates.count(currentState) > 0)
            {
                return cadena;
            }
            else
            {
                
                if(currentState > 0){
                  inputStream.putback(cadena.back());
                }
                return "nulo";
            }
        }
        else{
            cadena=cadena+auxiliar;
        }

        currentState = dfa.transitionTable[transition];
    }
    return "";
}

struct Token{
    string nombre;
    string atributo;
    
};

void agregarSimbolo(Lexer& lexer, Token token){
    if(lexer.Tabla.count(token.nombre) == 0){
        lexer.Tabla.emplace(make_pair(token.nombre,make_pair(token.nombre,token.atributo)));
    }
}

string palabraRes(Lexer& lex, string cad){
    
    if(lex.Tabla.count(cad)>0){
        return "Si esta";
    }
    return"";
}

Token getNextToken(Lexer& lexer, std::ifstream& inputStream, int linea){
    string cadena;
    string esp = " ";
    Token token;
    
    while(inputStream.good()){
        
        cadena = dfaStart(lexer.identificadores, inputStream);
        string reservada = palabraRes(lexer, cadena);
        if(cadena != "nulo"){
            if(reservada == "Si esta"){
                token.nombre=cadena;
                token.atributo="";
                return token;
            }
            
            else{
                token.nombre= "ID" +esp+cadena;
                token.atributo= cadena;
                agregarSimbolo(lexer, token);
                return token;
            }
        }
        
        //ejecuta DFA constantes numericas
        cadena = dfaStart(lexer.constantesNum, inputStream);
        if(cadena != "nulo"){
            token.nombre= "CTENum"+esp+cadena;
            token.atributo= cadena;
            agregarSimbolo(lexer, token);//no entiendo
            return token;
        }
        
        //ejecuta DFA operadores relacionales
        cadena = dfaStart(lexer.operadorRel, inputStream);
        if(cadena != "nulo"){
            token.nombre= "OPRel";
            token.atributo= cadena;
            return token;
        }
        
        //ejecuta DFA operadores Aritmeticos
        cadena = dfaStart(lexer.operadorArit, inputStream);
        if(cadena != "nulo"){
            token.nombre= "OPArit";
            token.atributo= cadena;
            return token;
        }
        
        //ejecuta DFA operadores logicos
        cadena = dfaStart(lexer.operadorLog, inputStream);
        if(cadena != "nulo"){
            token.nombre= "OPLog";
            token.atributo= cadena;
            return token;
        }
        
        //ejecuta DFA operadores asignacion
        cadena = dfaStart(lexer.operadorAsig, inputStream);
        if(cadena != "nulo"){
            token.nombre= "OPAsig";
            token.atributo= cadena;
            return token;
        }
        
        //ejecuta DFAsimbolos agrupacion
        cadena = dfaStart(lexer.SimboloAgrup, inputStream);
        if(cadena != "nulo"){
            token.nombre= "SimAgrup";
            token.atributo= cadena;
            return token;
        }
        
        //ejecuta DFA punto y coma
        cadena = dfaStart(lexer.PuntoComa, inputStream);
        if(cadena != "nulo"){
            token.nombre= "PuntoComa";
            token.atributo= cadena;
            return token;
        }
        
        //ejecuta DFA caracteres blanco
        cadena = dfaStart(lexer.caracterBlanco, inputStream);
        if(cadena != "nulo"){
            token.nombre= "CarBlan";
            token.atributo= cadena;
            return token;
        }
        
        //Verifica si hubo error
        if(cadena == "nulo"){
            cout <<"Error linea: " <<linea <<endl;
            exit(1);
        }
        
        //Fin de archivo
        if(cadena == ""){
            token.nombre= "EOF";
            token.atributo="";
            return token;
        }
    }
    
}

int main()
{
    Lexer lex;
    Token token;
    int linea=1;
    inicializaLexer(lex);
    ifstream inputStream("source.txt");
    token = getNextToken(lex, inputStream, linea);
    
    cout <<"TOKENS" <<endl;
    cout <<token.nombre <<" " <<token.atributo <<"\n" <<endl;
    while(inputStream.good() && token.nombre != "EOF"){
        token = getNextToken(lex, inputStream, linea);
        char symbol= inputStream.get();
        if(symbol == '\n'){
           linea++;
           }
        inputStream.putback(symbol);
        cout <<token.nombre <<" " <<token.atributo <<endl;
    }
    
    cout <<"\nTABLA DE SIMBOLOS" <<endl;
    for(auto& i:lex.Tabla){
        cout <<i.first << ", " <<i.second.first <<", " <<i.second.second <<endl;
    }
    
    inputStream.close();
    
return 0;
}
