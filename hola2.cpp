#include<iostream>
#include<ctype.h>
#include <fstream>

using namespace std;
using std::ifstream;

int main()
{
    ifstream inputStream("source.txt"); 
    while(inputStream.good())
    {
        char symbol = inputStream.get();
   	if(isdigit(symbol))
    		{
        		cout<<"It's a digit" <<endl;
    		}
    	else
	{
   		if(isalpha(symbol))
    			{
        			cout<<"It's a leter" <<endl;  
    			}
		else
		{
   			if(isspace(symbol))
    			{
				if(symbol=='\n')
				{
					cout << "It's a jum" << endl;
				}
				else
				{
        			cout<<"It's a space" <<endl;
				}	
    			}
			else
    			{
				if(symbol==EOF)
				{
					cout << "end" << endl;
				}
				else
				{
        				cout<<"It's a other" <<endl;
				}	
    			}
		}
	}
    }
    inputStream.close();
}
