#pragma once
#include "PushdownAuto.h"
#include <vector>
#include <iostream>
#include <string>
#include "Common.h"
using namespace std;





class Grammar {
private:
	friend class PushdownAuto;

	string startSymbol;
	TerminalSet terminals;
	NonTerminalSet nonterminals;
	RulesSet rules; 
public:
	Grammar();
	Grammar parseInput(string terminals,string start,string nonterm,vector<string> rules);
	Grammar(string start, TerminalSet term, NonTerminalSet non, RulesSet rul);
	

	
};
