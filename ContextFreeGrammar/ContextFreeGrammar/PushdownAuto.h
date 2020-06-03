#pragma once
#include <iostream>
#include <iterator>
#include "Grammar.h"
#include "Common.h"
#include <string>
#include <stack>
#include <tuple>
#include <vector>
#include <map>
using namespace std;
class Grammar;

class PushdownAuto
{

	string StartState="q"; 
	vector<char> InputSymbolsSet;
	pair<TerminalSet, NonTerminalSet> Alphabet;
	string StartStackSymbol; //the start nonterminal
	vector<TransitionState> states; //these transition states will be apllied to a stack
	bool checkIfEpsilonT(TransitionState state);
	bool checkIfCorrectTerminals(PushdownAuto other);
	bool checkIfNonterminalIsIn(string nonterminal);
	vector<TransitionState> findApSt(nonterminal start);
	void RemoveRecursiveStates(vector<TransitionState>& states);
	void RemoveEpsilonStates(vector<TransitionState>& states);
	bool isTerminal(string t);




public:
	PushdownAuto ConvertToPDA(Grammar const& grammar);
	string ApplyTransitionRule(TransitionState const& state);
	string operator<<(string const& input);//takes a string and applies transition function to the nonterminals
	string& operator>>(string& output);//returns the string with applied rules from operator <<
	bool isAccepted(string word);
	void parseState(bool& found,int term_num,string& word, string old_word ,stack<string> pdastack, vector<TransitionState> apllicablestates);

	//void parseStates(string& word, string old_word, stack<string>& pdastack, vector<TransitionState>& apllicablestates);
	PushdownAuto& Union(PushdownAuto other);
	PushdownAuto& Concat(PushdownAuto other);
	PushdownAuto& Iter();
	void print();
	bool isFinite();
	bool isEmpty() const;
	bool isCyclic(TransitionState st, vector<TransitionState> statesCopy, NonTerminalSet visited);
	PushdownAuto();
	~PushdownAuto();

	NonTerminalSet getNonterminals() {
		return this->Alphabet.second;
	}
	string generateNonterminalName(nonterminal nt,int index);

	void changeTransitionStates(PushdownAuto& Other,string new_name, string old_name);
	
};

