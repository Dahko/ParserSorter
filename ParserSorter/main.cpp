#include "Element.h"
#include "Parsing.h"

#include <string>
#include <iostream>

void TestParsing() {
	std::string s("(X + (V+M+A) + C +(M+L + (C + C) + C))");
	std::cout << "\ninput: " << s;
	TEquationPtr ptr = ParseString(s.c_str());
	ptr->Sort();
	std::cout << "\nresult: ";
	if(!ptr) { std::cout << "parsing failed\n"; return; }
	ptr->Print(std::cout);
	std::cout << "\n";
}

void TestParsingIncorrectData() {
	ParseString("(1");
	ParseString("1)");
	ParseString("(+1)");
	ParseString("(1+)");
	ParseString("(+)");
	ParseString("()");
	ParseString("(11)");
	ParseString("(())");
}

void main() {
	TestParsing();
	
	//TestClosingBracket();
	//TestParsingIncorrectData();

	system("pause");
}


