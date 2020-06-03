#include "Grammar.h"
#include <string>

Grammar::Grammar()
{
}

Grammar  Grammar::parseInput(string terminals,string start, string nonterm, vector<string> rules)// { XX }->a {XX} / @|a / @
{
	Grammar grammar;
	grammar.startSymbol = start;
	for (size_t i = 0; i < terminals.size(); i++)
	{
		if (terminals[i] == '}' || terminals[i] == '{' || terminals[i] == '@' ||
			(terminals[i] == char(92) && terminals[i+1]!='}' && terminals[i+1] != '{' && terminals[i+1] != '@' && terminals[i+1] != char(92)))
		{
			cerr << "Wrong terminals!" << endl;
			return grammar;
		}
	    else if (terminals[i]==char(92)) {
			++i;
			grammar.terminals.push_back(terminals[i]);

		}
		else if (terminals[i] != ' ') {
			grammar.terminals.push_back(terminals[i]);
		}
	
	}
	for (size_t i = 0; i < nonterm.size(); i++)
	{
		if (nonterm[i] == '{') //we have found a nonterminal
			
		{
			string nt;
			i++;
			while (nonterm[i] != '}')
			{
				nt += nonterm[i];
				i++;
			}
			grammar.nonterminals.push_back(nt);

		}

	}
	for (size_t i = 0; i < rules.size(); i++)
	{
		Rule ru;
		for (size_t j = 0; j < rules[i].size(); j++)
		{
			if (rules[i][j] == '{') {
				string nont;
				j++;
				while (rules[i][j]!='}')
				{
					nont += rules[i][j];
					j++;
				}
				if (ru.nonTerminal.empty()) { ru.nonTerminal = nont; }
				else { ru.destinationProduct.push_back(nont); }
			}
			else if (rules[i][j] == '-' ) {
				j++;
			}
			else if (rules[i][j] == char(92) && rules[i][j+1]!= '}' && rules[i][j + 1] != '{' && rules[i][j + 1] != '@' && rules[i][j + 1] != char(47)) {
				cerr << "Invalid input!" << endl;
				return grammar;
			}
			else if (rules[i][j] == char(92)) {
				string specialSymbol;
				specialSymbol += rules[i][j];
				j++;
				specialSymbol += rules[i][j];
				ru.destinationProduct.push_back(specialSymbol);
			}
			else if (rules[i][j] == '|') {
				grammar.rules.push_back(ru);
				ru.destinationProduct.clear();
			}
			else {
				string chartostring;
				chartostring=rules[i][j];
				ru.destinationProduct.push_back(chartostring); }
		}
		grammar.rules.push_back(ru);
	}
	return grammar;
}

Grammar::Grammar(string start, TerminalSet term, NonTerminalSet non, RulesSet rul):startSymbol(start), terminals(term), nonterminals(non), rules(rul) {  }


