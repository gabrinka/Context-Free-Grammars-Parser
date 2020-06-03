#include <iostream>
#include <string>
#include <vector>
#include "Grammar.h"
#include "User.h"
#include "PushdownAuto.h"
using namespace std;
int main() {
	//run();
	

	/*string terminals = R"(a b \@)";
	string nonterm = "{SS} {X}";
	vector<string> rules = { R"({SS}->aa{SS}|b{SS}|{X}|@)", R"({X}->bb{X}|@)" };
	Grammar o=o.parseInput(terminals,"SS",nonterm,rules);
	PushdownAuto m = m.ConvertToPDA(o);
	cout << m.isFinite();
	cout <<"prazen li e"<< m.isEmpty();
	//cout << "priema li"<<m.isAccepted("@")<<endl;
	cout << "priema li" << m.isAccepted("aabbb") << endl;
	//cout << "priema li" << m.isAccepted("abaa") << endl;
	//cout << "priema li" << m.isAccepted("aabaab") << endl;

	

	/*string terminals1 = "a b \\@ ";
	string nonterm1 = "{SS}";
	vector<string> rules1 = { "{SS}->{SS}" };
	Grammar o1 = o1.parseInput(terminals1,"SS", nonterm1, rules1);
	PushdownAuto m1= m1.ConvertToPDA(o1);
	m1 << ("abb");
	string output;
	m1 >> output;
	cout << m1.isEmpty();*/

	
	
	
	system ("pause");
	return 0;
}