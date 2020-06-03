#include "User.h"
using std::string;
using std::cout;
using std::endl;
using std::cin;
void run() {
	string user_input;
	cout << "Context free grammar" << endl;
	cout << endl;
	cout << "Possible commands are:" << endl;
	cout << "0.Enter..../new grammar" << endl;
	cout << "1.<< " << endl;
	cout << "2.>>" << endl;
	cout << "3. Concat..../enter a new grammar and perform concatenation of the two grammars" << endl;
	cout << "4.Union..../enter a new grammar and perform union of the two grammars" << endl;
	cout << "5.Iter..../performs iteration over the grammar" << endl;
	cout << "6.Accept *word*..../checks if the grammar reads a certain word" << endl;
	cout << "7.Finite?..../checks if the grammar language is finite" << endl;
	cout << "8.Empty?..../checks if the grammar language is empty" << endl;
	cout << "9.Exit" << endl;

	cout << endl;
	cout << endl;
	cout << "Please,type a command here: ";
	Grammar start;
	Grammar second;
	PushdownAuto stackf;
	PushdownAuto stacks;
	do {
		cin >> user_input;
		command(user_input,start,second,stackf,stacks);
	} while (user_input != "Exit");
 }
void command(string cmd, Grammar& first, Grammar& second,PushdownAuto& stackf,PushdownAuto& stacks) {
	if (cmd=="Enter")
	{
		cin.ignore();
		string terminals;
		cout << "Enter terminals:";
		getline(cin, terminals);
		cout << endl<<"Enter nonterminals:";
		string nonterm;
		getline(cin, nonterm);
		cout << endl << "Enter rules/when finished,enter stop/:";
		string rule;
		vector<string> rules;
		do
		{
			cin >> rule;
			if(rule!="stop") rules.push_back(rule);

		} while (rule!="stop");
		cout <<endl<< "Enter a start symbol:";
		string starts;
		cin >> starts;
		first=first.parseInput(terminals, starts, nonterm,rules);
		 stackf = stackf.ConvertToPDA(first);
		cout << endl;
	
	}
	else if (cmd=="<<") {
		cout << "Type a word:";
		string word;
		cin >> word;
		stackf << word;
		cout << endl;
	}
	else if (cmd == ">>") {
		string result;
		stackf >> result;
		cout << endl;
	}
	else if (cmd == "Concat") {
		cin.ignore();
		string terminals;
		cout << "Please enter a new grammar.Enter terminals:";
		getline(cin, terminals);
		cout << endl << "Enter nonterminals:";
		string nonterm;
		getline(cin, nonterm);
		cout << endl << "Enter rules/when finished,enter stop/:";
		string rule;
		vector<string> rules;
		do
		{
			cin >> rule;
			if (rule != "stop") rules.push_back(rule);

		} while (rule != "stop");
		cout << endl << "Enter a start symbol:";
		string starts;
		cin >> starts;
		second = second.parseInput(terminals, starts, nonterm, rules);
		stacks = stacks.ConvertToPDA(second);
		PushdownAuto res = stackf.Concat(stacks);
		cout << endl;
		res.print();
	}
	else if (cmd == "Union") {
		cin.ignore();
		string terminals;
		cout << "Please enter a new grammar.Enter terminals:";
		getline(cin, terminals);
		cout << endl << "Enter nonterminals:";
		string nonterm;
		getline(cin, nonterm);
		cout << endl << "Enter rules/when finished,enter stop/:";
		string rule;
		vector<string> rules;
		do
		{
			cin >> rule;
			if (rule != "stop") rules.push_back(rule);

		} while (rule != "stop");
		cout << endl << "Enter a start symbol:";
		string starts;
		cin >> starts;
		second = second.parseInput(terminals, starts, nonterm, rules);
		stacks = stacks.ConvertToPDA(second);
		PushdownAuto res = stackf.Union(stacks);
		cout << endl;
		res.print();
	}
	else if (cmd == "Iter") {
		PushdownAuto res = stackf.Iter();
		cout << endl;
		res.print();

	}
	else if (cmd == "Accept") {
		cin.ignore();
		string word;
		cout << endl << "Please type a word:";
		cin >> word;
		if (stackf.isAccepted(word)) { cout<< "Yes"; }
		else { cout << "No"; }
		cout << endl;
		
	}
	else if (cmd == "Finite?") {
	cin.ignore();
	if (stackf.isFinite()) { cout << "Yes"; }
	else { cout << "No"; }
	cout << endl;

	}
	else if (cmd == "Empty?") {
	cin.ignore();
	if (stackf.isEmpty()) { cout << "Yes"; }
	else { cout << "No"; }
	cout << endl;

	}
	else if (cmd == "Exit") {
	return;
}
}