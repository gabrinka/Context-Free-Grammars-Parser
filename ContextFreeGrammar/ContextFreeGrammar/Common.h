#pragma once
#include <vector>
#include <string>
using namespace std;
struct Rule {
	string nonTerminal; //source point
	vector<string> destinationProduct;
	Rule() {};
	Rule(string non, vector<string> dest) :nonTerminal(non), destinationProduct(dest) {}

};

using terminal = char;
using nonterminal = string;
using TerminalSet = vector<char>;
using NonTerminalSet = vector<string>;
using RulesSet = vector<Rule>;

struct TransitionState {
	string CurrentStack;
	string input; //terminal
	vector <string> ResultStack;
	TransitionState() {};
	TransitionState(string CStack, string inp, vector <string> RStack) :CurrentStack(CStack), input(inp), ResultStack(RStack) {};
};

