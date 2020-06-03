#include "PushdownAuto.h"
#include <algorithm>
#include <stack>
static string output;
static vector<string> usedNonterminals;

bool stringOccursInVector(const vector<string>& vec, const string& str) {
	bool isFound = false;
	for (auto vit = vec.cbegin(); !isFound && vit != vec.cend(); vit++) {
		isFound = (*vit) == str;
	}
	return isFound;
}

bool PushdownAuto::checkIfEpsilonT(TransitionState state)
{
	return (state.ResultStack[0] == "@");
}

bool PushdownAuto::checkIfCorrectTerminals(PushdownAuto other)
{
	bool correctTerminals = other.InputSymbolsSet.size() == this->InputSymbolsSet.size();
	for (auto i = this->InputSymbolsSet.cbegin(); correctTerminals && i != this->InputSymbolsSet.cend(); i++) {
		for (auto j = other.InputSymbolsSet.cbegin(); !correctTerminals && j != other.InputSymbolsSet.cend(); j++) {
			correctTerminals = (*i == *j);
		}
	}
	return correctTerminals;
}

bool PushdownAuto::checkIfNonterminalIsIn(string nonterminal)
{
	bool result = false;
	for (auto i =usedNonterminals.cbegin();i!=usedNonterminals.cend() && !result;i++)
	{
		if (nonterminal == *i)
		{
			result = true;
			
		}
	}
	return result;
}

vector<TransitionState> PushdownAuto::findApSt(nonterminal start)
{
	try {
		vector<TransitionState> result;
		for (size_t i = 0; i < this->states.size(); i++)
		{
			if (this->states[i].CurrentStack == start) {
				result.push_back(this->states[i]);
			}
		}
		return result;
	}
	catch (...) {
		cerr << "the language is polurazreshim :) \n";

	}
}

void PushdownAuto::RemoveRecursiveStates(vector<TransitionState>& states)
{
	
	bool flag = true;
	for (int i=0;i<states.size() && i>=0;i++ )
	{
		flag = true;
		for (int j = 0; j < states[i].ResultStack.size() && (flag); j++)
		{
			if (states[i].CurrentStack==states[i].ResultStack[j])
			{
				vector<TransitionState>::iterator tempIt = states.begin() + i;
				states.erase(tempIt);
				if (i == 0) break;
				else i--;
				flag = false;
				
				
			}

		}
	}
}

void PushdownAuto::RemoveEpsilonStates(vector<TransitionState>& states)
{
	for (size_t i = 0; i < states.size(); i++)
	{
		
			if (states[i].ResultStack[0]=="@")
			{
				vector<TransitionState>::iterator tempIt = states.begin() + i;
				states.erase(tempIt);
				i--;
			}

		
	}
}

bool PushdownAuto::isTerminal(string t)
{
	for (size_t i = 0; i < this->InputSymbolsSet.size(); i++)
	{
		if (t[0] == this->InputSymbolsSet[i]) return true;
		if (t.size() != 1 && t[1] == this->InputSymbolsSet[i]) return true;
	}
	return false;
}

string PushdownAuto::ApplyTransitionRule(TransitionState const & state)
{
	string product;
	for (size_t i = 0; i < state.ResultStack.size(); i++)
	{
		if (!isTerminal(state.ResultStack[i]) && state.ResultStack[i] != "@") {
			product += '{';
			product += state.ResultStack[i];
			product += '}';
		}
		else {
			product += state.ResultStack[i];
		}
	}
	return product;
}

PushdownAuto PushdownAuto::ConvertToPDA(Grammar const& grammar)
{
	PushdownAuto Automata;
	Automata.StartStackSymbol = grammar.startSymbol;
	Automata.InputSymbolsSet = grammar.terminals;
	Automata.Alphabet.first = grammar.terminals;
	Automata.Alphabet.second = grammar.nonterminals;

	TransitionState default_transition;
	default_transition.CurrentStack = "@";
	default_transition.input = '@';
	default_transition.ResultStack.push_back("@");
	Automata.states.push_back(default_transition);



	for (size_t i = 0; i < grammar.terminals.size(); i++) //(q,a,a)=(q,@);
	{
		TransitionState transition;
		string charinput;
		charinput = grammar.terminals[i];
		transition.CurrentStack = charinput;
		if (grammar.terminals[i] == char(92) || grammar.terminals[i] == '{' || grammar.terminals[i] == '}' || grammar.terminals[i] == '@')
		{
			transition.CurrentStack.erase();
			transition.CurrentStack+=char(92);
			transition.CurrentStack += grammar.terminals[i];

			transition.input += char(92);
			transition.input += grammar.terminals[i];
			
		}
		else {
			transition.input = grammar.terminals[i];
		}
		transition.ResultStack.push_back("@");
		Automata.states.push_back(transition);


	}
	for (size_t i = 0; i < grammar.rules.size(); i++)

	{
		TransitionState RuleTransformation;
		RuleTransformation.CurrentStack = grammar.rules[i].nonTerminal;
		RuleTransformation.input = '@';

		for (size_t j = 0; j < grammar.rules[i].destinationProduct.size(); j++)
		{

			RuleTransformation.ResultStack.push_back(grammar.rules[i].destinationProduct[j]);

		}
		Automata.states.push_back(RuleTransformation);

	}
	
	return Automata;

}

string PushdownAuto::operator<<(string const & input) // automata<<"AAbcXX"
{
	::output.clear();
 int n = this->states.size();
	for (size_t i = 0; i < input.size(); i++)
	{
		if (input[i] == '{') {
			nonterminal found;
			i++;
			while (input[i]!='}')
			{
				found += input[i];
				i++;
			}
			vector<TransitionState> ApplicableStates = findApSt(found);
			RemoveRecursiveStates(ApplicableStates);
			RemoveEpsilonStates(ApplicableStates);
			if (ApplicableStates.empty())
			{
				cerr << "Only nonrecursive transitions are allowed!" << endl;
				return input;
			}
			else
			{
				int random = rand() % ApplicableStates.size();
				::output += ApplyTransitionRule(ApplicableStates[random]);
			}
			
		}
		else {
			::output += input[i];
		}
	}
	return ::output;
}

string & PushdownAuto::operator>>(string & output)
{
	output = ::output;
	if (output.empty())
	{
		cerr << "No string saved" << endl;
	}
	cout << output;
	return output;
}

bool PushdownAuto::isAccepted(string word)
{
	try {
		stack<string> pdastack;
		pdastack.push(this->StartStackSymbol);
		vector<TransitionState> apllicablestates = this->findApSt(pdastack.top());
		int num=0;
		bool found = false;
		parseState(found,num,word, word, pdastack, apllicablestates);
		/*for (int i = 0; i < pdastack.size(); i++)
		{
			cout << pdastack.top() << endl;
			pdastack.pop();
		}*/
		//cout << word;
		if (word.empty() || (found)) return true;

		
		return false;
	}
	catch (...) {
		cerr << "the language is polurazreshim :) \n";
		return 0;
	}
}

void PushdownAuto::parseState(bool& found,int term_num,string& word, string old_word, stack<string> pdastack, vector<TransitionState> apllicablestates)
{
	if (found) { return; }
	bool flag = true;
	size_t p = 0;
	for (p; p < apllicablestates.size(); p++)
	{
		flag = true;
		if (found) { return; }
		else if (pdastack.empty() && !word.empty()) { return; }
		else if (apllicablestates[p].ResultStack[0] == "@" && !isTerminal(pdastack.top()) && word[0]!='@') {
			stack<string> old_pdastack = pdastack;
			int old_termnum = term_num;
			pdastack.pop();
			if (pdastack.empty() && word.empty()) found = true;
			else if (pdastack.empty()) { pdastack = old_pdastack; }
			else {
				apllicablestates = findApSt(pdastack.top());
				parseState(found, term_num, word, old_word, pdastack, apllicablestates);
			}
			if (found) { return; }
			if (!pdastack.empty()) {
				pdastack = old_pdastack; word = old_word; term_num = old_termnum; apllicablestates = findApSt(pdastack.top());
			}
		}
		else if ((apllicablestates[p].ResultStack[0] == "@") && word[0] == '@')
		{
			pdastack.pop();
			found=true;
			word.erase(0, 1);
			return;

		}
		else if(!isTerminal(pdastack.top())) {
			if (term_num > word.size()) { return; }
			stack<string> old_pdastack = pdastack;
			pdastack.pop();
			old_word = word;
			string letter;
			letter = word[0];
			int old_termnum = term_num;
			if (letter[0] == char(92)) letter += word[1];
			auto i = apllicablestates[p].ResultStack.cend();
			for (auto ri = apllicablestates[p].ResultStack.rbegin(); ri != apllicablestates[p].ResultStack.rend(); ++ri)
			{
				if (isTerminal(*ri)) term_num++;
				pdastack.push(*ri);
				if (word.find(pdastack.top()) == string::npos && isTerminal(pdastack.top())) {
					pdastack = old_pdastack;
					if(term_num!=0)term_num  --;
					flag = false;
					break;
					

				}
			}
			if (flag) {
				apllicablestates = findApSt(pdastack.top());
				parseState(found,term_num, word, old_word, pdastack, apllicablestates);
			}
			if (found) { return; }
			if (!pdastack.empty()) { pdastack = old_pdastack; term_num = old_termnum; word = old_word; apllicablestates = findApSt(pdastack.top());
			}
			
		}
		else if (isTerminal(pdastack.top())) {
			if (term_num > word.size()) { return; }
			stack<string> old_pdastack = pdastack;
			old_word = word;
			string letter;
			int old_termnum = term_num;
			letter = word[0];
			if (letter[0] == char(92)) letter += word[1];
			if (pdastack.top() != letter) { return; }
			pdastack.pop();
			term_num--;
			word.erase(0, 1);
			if(letter.size()==2) word.erase(0, 1);
			if (pdastack.empty() && word.empty()) { found = true; return; }
			if (pdastack.empty()) return;
			apllicablestates = findApSt(pdastack.top());
			parseState(found,term_num,word, old_word, pdastack, apllicablestates);
			if (found) { return; }
			if (!pdastack.empty() && !word.empty()) { pdastack = old_pdastack; term_num = old_termnum; apllicablestates = findApSt(pdastack.top());
			}
			


		}
	}
}
	


PushdownAuto& PushdownAuto::Union(PushdownAuto other) 
{
	::usedNonterminals.clear();
	for (size_t i = 0; i <this->getNonterminals().size(); i++)
	{
		::usedNonterminals.push_back(this->getNonterminals()[i]);
	}
	PushdownAuto* OtherCopy = new PushdownAuto(other); // we are going to parse the copy of the other automata change it if needed and then execute union o the two automatas
	//first we check if the terminals sets are the same
	PushdownAuto* UnionResult = new PushdownAuto(*this);
	if (!checkIfCorrectTerminals(*OtherCopy))
	{
		cerr << "Terminal sets are not the same,cannot perform union!" << endl;
		return *UnionResult;
	}
	//next,we check if any on the nonterminals/currentstack has repeated names,if so,we change them,we must remember the old names so that we can later on change the rules/transition states with the new names
	//in order to keep the rules we should keep the old names of the nonterminals
	map<string, string> OldNewNonterminals; //(new name is the key, old name is the value)
	//iterating over the second pda nonterminals
	int index = usedNonterminals.size();

	vector<nonterminal> a = (*OtherCopy).getNonterminals();
	auto b = a.begin();
	for (auto i = a.begin(); i!=a.end(); i++)
	{
		if (checkIfNonterminalIsIn(*i))
		{
			
			string new_nonterm_name = generateNonterminalName(*i,index);
			::usedNonterminals.push_back(new_nonterm_name);
			if ((*i) == (*OtherCopy).StartStackSymbol) (*OtherCopy).StartStackSymbol = new_nonterm_name;
			OldNewNonterminals.emplace(new_nonterm_name,*i);
		
			*i = new_nonterm_name;
			UnionResult->Alphabet.second.push_back(new_nonterm_name);
			changeTransitionStates(*OtherCopy, new_nonterm_name, OldNewNonterminals.at(new_nonterm_name));
			
		}
		else { UnionResult->Alphabet.second.push_back(*i); }
		
	}
	UnionResult->StartStackSymbol = generateNonterminalName(this->StartStackSymbol, usedNonterminals.size());
	UnionResult->Alphabet.second.push_back(UnionResult->StartStackSymbol);
	TransitionState NewStartTransition;
	NewStartTransition.CurrentStack = UnionResult->StartStackSymbol;
	NewStartTransition.input = '@';
	NewStartTransition.ResultStack.push_back(OtherCopy->StartStackSymbol);

	TransitionState NewStartTransition2;
	NewStartTransition2.CurrentStack = UnionResult->StartStackSymbol;
	NewStartTransition2.input = '@';
	NewStartTransition2.ResultStack.push_back(this->StartStackSymbol);

	UnionResult->states.emplace(UnionResult->states.begin(), NewStartTransition);
	UnionResult->states.emplace(UnionResult->states.begin()+1, NewStartTransition2);

	for (size_t i = OtherCopy->InputSymbolsSet.size()+1; i < OtherCopy->states.size(); i++)//skipping adding the (q,a,a)=(q,@) rules and (q,@,@)=(q,@)
	{
		UnionResult->states.push_back(OtherCopy->states[i]);
	}

	return *UnionResult;

}

PushdownAuto& PushdownAuto::Concat(PushdownAuto other)
{
	::usedNonterminals.clear();
	if (::usedNonterminals.empty())
	{
		for (size_t i = 0; i < this->getNonterminals().size(); i++)
		{
			::usedNonterminals.push_back(this->getNonterminals()[i]);
		}
	}
	PushdownAuto* OtherCopy = new PushdownAuto(other); // we are going to parse the copy of the other automata change it if needed and then execute concatenation o the two automatas
	//first we check if the terminals sets are the same
	PushdownAuto* UnionResult = new PushdownAuto(*this);
	if (!checkIfCorrectTerminals(*OtherCopy))
	{
		cerr << "Terminal sets are not the same,cannot perform concatenation!" << endl;
		return *UnionResult;
	}
	//next,we check if any on the nonterminals/currentstack has repeated names,if so,we change them,we must remember the old names so that we can later on change the rules/transition states with the new names
	//in order to keep the rules we should keep the old names of the nonterminals
	map<string, string> OldNewNonterminals; //(new name is the key, old name is the value)
	//iterating over the second pda nonterminals
	int index = usedNonterminals.size();

	vector<nonterminal> a = (*OtherCopy).getNonterminals();
	for (auto i = a.begin(); i != a.end(); i++)
	{
		if (checkIfNonterminalIsIn(*i))
		{

			string new_nonterm_name = generateNonterminalName(*i, index);
			::usedNonterminals.push_back(new_nonterm_name);
			if ((*i) == (*OtherCopy).StartStackSymbol) (*OtherCopy).StartStackSymbol = new_nonterm_name;
			OldNewNonterminals.emplace(new_nonterm_name, *i);

			*i = new_nonterm_name;
			UnionResult->Alphabet.second.push_back(new_nonterm_name);
			changeTransitionStates(*OtherCopy, new_nonterm_name, OldNewNonterminals.at(new_nonterm_name));

		}
		else { UnionResult->Alphabet.second.push_back(*i); }

	}
	UnionResult->StartStackSymbol = generateNonterminalName(this->StartStackSymbol, usedNonterminals.size());//TODO NEW FUNCTION
	UnionResult->Alphabet.second.push_back(UnionResult->StartStackSymbol);
	TransitionState NewStartTransition;
	NewStartTransition.CurrentStack = UnionResult->StartStackSymbol;
	NewStartTransition.input = '@';
	NewStartTransition.ResultStack.push_back(this->StartStackSymbol);
	NewStartTransition.ResultStack.push_back(OtherCopy->StartStackSymbol);

	UnionResult->states.emplace(UnionResult->states.begin(), NewStartTransition);

	for (size_t i = OtherCopy->InputSymbolsSet.size()+1; i < OtherCopy->states.size(); i++)//skipping adding the (q,a,a)=(q,@) rules and (q,@,@)=(q,@)
	{
		UnionResult->states.push_back(OtherCopy->states[i]);
	}
	return *UnionResult;

}
/*Basically returns the same grammar with new start nonterminal and new rule.*/
PushdownAuto & PushdownAuto::Iter()
{
	::usedNonterminals.clear();
	PushdownAuto* IterAutomata = new PushdownAuto(*this);
	string newStart = generateNonterminalName(this->StartStackSymbol,usedNonterminals.size());
	IterAutomata->StartStackSymbol = newStart;
	IterAutomata->Alphabet.second.push_back(newStart);
	TransitionState t1 = { newStart,"@",{"@"} };
	TransitionState t2 = { newStart,"@", { newStart,this->StartStackSymbol} };
	IterAutomata->states.push_back(t1);
	IterAutomata->states.push_back(t2);
	return *IterAutomata;

}

//if there is a cycle,the language is infinite.
bool PushdownAuto::isFinite()

{
	vector<TransitionState> statesCopy = this->states; //these transition states will be apllied to a stack
	statesCopy.erase(statesCopy.begin(), statesCopy.begin() + 1 + this->InputSymbolsSet.size());
	NonTerminalSet visited;
	string start = this->StartStackSymbol;
	vector<TransitionState> applSt = this->findApSt(start);
	for (size_t i = 0; i < applSt.size(); i++)
	{
		if (isCyclic(applSt[i], statesCopy, visited)) return false;
	}
	return true;
}
/*We proceed by solving a more general problem: whether from a nonterminal
a string of terminals is derivable.Algorithm proceeds bottom up by marking terminals, then all nonterminals
with rules whose rhs consists only of marked symbols*/
bool PushdownAuto::isEmpty() const 
{
	vector<string> marked;
	vector<TransitionState> statesCopy = this->states; //these transition states will be apllied to a stack
	statesCopy.erase(statesCopy.begin(), statesCopy.begin() + 1 + this->InputSymbolsSet.size());
	for (auto term: this->InputSymbolsSet)
	{
		string temp;
		if (term == '}' || term== '{' || term== '@' ||
			term == char(92))
		{
			temp += char(92);
			temp += term;
		}else {
			temp = term;
		}
		marked.push_back(temp);
		marked.push_back("@");
	}
	bool added = false;
	do {
		added = false;
		for (auto& rule : statesCopy) {
			bool addToMarked = true;
			auto rpit = rule.ResultStack.cbegin();
			while (addToMarked && rpit != rule.ResultStack.cend()) { //if whole product is made up from elements of marked -> add NT
				addToMarked = stringOccursInVector(marked, (*rpit));
				rpit++;
			}
			if (addToMarked && !stringOccursInVector(marked, rule.CurrentStack)) {
				marked.push_back(rule.CurrentStack);
				added = true;
			}
		}
	} while (added); //repeat untill there are NTs to add to marked
	return !stringOccursInVector(marked, StartStackSymbol);
}

bool PushdownAuto::isCyclic(TransitionState st, vector<TransitionState> statesCopy, NonTerminalSet visited)
{
	visited.push_back(st.CurrentStack);

	for (size_t i = 0; i < st.ResultStack.size(); i++)
	{
		if (!this->isTerminal(st.ResultStack[i]))
		{
			for (size_t j = 0; j < visited.size(); j++)
			{
				if (st.ResultStack[i] == visited[j])
				{
					return true;
				}
			}
			for (size_t n = 0; n < findApSt(st.ResultStack[i]).size(); n++)
			{
				if (isCyclic(findApSt(st.ResultStack[i])[0], statesCopy, visited)) return true;
			}
		}
	}
	return false;
}


PushdownAuto::PushdownAuto()
{
}


PushdownAuto::~PushdownAuto()
{
}

string PushdownAuto::generateNonterminalName(nonterminal nt, int index)
{
	string new_names;
	for (size_t i = 0; i < nt.size(); i++)
	{
		new_names += char(int(nt[i]));
	}
	new_names += to_string(index);
	::usedNonterminals.push_back(new_names);
	return new_names;

}

void PushdownAuto::changeTransitionStates(PushdownAuto & Other, string new_name, string old_name)
{
	for  (int i=0;i<Other.states.size();i++)
	{
		if (Other.states[i].CurrentStack==old_name)
		{
			Other.states[i].CurrentStack = new_name;
		}

		for (size_t j = 0; j < Other.states[i].ResultStack.size(); j++)
		{
			if (Other.states[i].ResultStack[j] == old_name) {
				Other.states[i].ResultStack[j] = new_name;
}
		}

	}
}
void PushdownAuto::print() {
	cout << "Terminals are:";
	for (size_t i = 0; i < this->InputSymbolsSet.size(); i++)
	{
		if (InputSymbolsSet[i] == '}' || InputSymbolsSet[i] == '{' || InputSymbolsSet[i] == '@' || InputSymbolsSet[i] == char(92)) {
			cout << char(92) << InputSymbolsSet[i] << " ";
} else {
			cout<< InputSymbolsSet[i] << " ";
		}
	}
	cout << endl << "Start symbol is:{" << this->StartStackSymbol<<'}';
	cout << endl << "Nonterminals are:";
	for (size_t i = 0; i < this->getNonterminals().size(); i++)
	{
		cout << '{' << this->getNonterminals()[i] << '}' << " ";
	}
	cout << endl << "The rules are:";
	for (size_t i = 0; i < this->getNonterminals().size(); i++)
	{
		cout << '{' << this->getNonterminals()[i] << '}' << "->";
		for (size_t j = 0; j < this->findApSt(this->getNonterminals()[i]).size(); j++)
		{
			for (size_t n = 0; n < this->findApSt(this->getNonterminals()[i])[j].ResultStack.size(); n++)
			{
				if(!this->isTerminal(this->findApSt(this->getNonterminals()[i])[j].ResultStack[n]) && (this->findApSt(this->getNonterminals()[i])[j].ResultStack[n])!="@"){
					cout << '{' << this->findApSt(this->getNonterminals()[i])[j].ResultStack[n] << '}';
				}
				else {
					cout <<  this->findApSt(this->getNonterminals()[i])[j].ResultStack[n];

				} 
			}if(j!= this->findApSt(this->getNonterminals()[i]).size() - 1)cout << '|';
		}
		cout << endl;
	}
}