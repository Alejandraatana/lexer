#include<iostream>
#include<ctype.h>
#include <fstream>
#include<unordered_map>

using namespace std;
using std::ifstream;
using std::unordered_map;
using std::make_pair;
using std::pair;

int main()
{
	unordered_map<string,pair<string,unsigned int>> hash;
	unordered_map<string,pair<string,unsigned int>>::iterator iter;
	hash.emplace(make_pair("else",make_pair("PR",6)));
	iter=hash.end();
	cout << hash.size() << endl; 

    /*ifstream inputStream("source.txt");
    string cad; 
    cad.clear();
    while(inputStream.good())
    {
        char symbol = inputStream.get();
	if(symbol!=EOF && symbol !='\n')
	{
	cad= cad + symbol;
	}
    }
    inputStream.close();
    cout << cad <<endl;*/

		/*for(auto& i:lexer.symbolTable)
	{
		cout << i.first << "," << i.second.first << "," <<i.second.second  << endl;
	}*/

}
