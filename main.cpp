#include <iostream>
#include <fstream>
#include <set>
#include <map>
#include <utility>

using std::cout;
using std::endl;
using std::ifstream;
using std::set;
using std::map;
using std::make_pair;
using std::pair;

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

bool dfaStart(DFA& dfa, std::ifstream& inputStream)
{
    unsigned int currentState = dfa.startingState;

    while(inputStream.good())
    {
        char symbol = inputStream.get();
        pair<unsigned int, char> transition = {currentState, symbol};

        if(dfa.transitionTable.count(transition) == 0)
        {
            // Backtrack
            inputStream.putback(symbol);

            if(dfa.finalStates.count(currentState) > 0)
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        currentState = dfa.transitionTable[transition];
    }

    if(dfa.finalStates.count(currentState) > 0)
    {
        return true;
    }

    return false;
}

int main()
{
    ifstream inputStream("source.txt");

    DFA dfa;
    dfaAddTransition(dfa, 0, 'a', 1);
    dfaAddTransition(dfa, 1, 'b', 1);
    dfaAddTransition(dfa, 1, 'a', 2);
    dfaAddTransition(dfa, 2, 'a', 2);
    dfaAddTransition(dfa, 2, 'b', 1);
    dfaAddFinalState(dfa, 2);

    if(dfaStart(dfa, inputStream))
    {
        cout << "Success" << endl;
    }
    else
    {
        cout << "Failure" << endl;
    }

    inputStream.close();

    return 0;
}
