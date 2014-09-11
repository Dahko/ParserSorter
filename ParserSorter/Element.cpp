#include "Element.h"
#include <iostream>

void TestClosingBracket() {
	std::string s("(123+dfsd +(1+(2+1) )+1) + 200 + (1+2)");
	TStrIter itClosing = GetClosingBracket(s.begin()+1, s.end());
	printf((itClosing - s.begin()) ? "ok" : "oops");
	std::string s2(s.begin(), itClosing+1);
	printf(("\n"+s2).c_str());
	printf("\n");
}

void TestParsing() {
	std::string s("(X + (V+M+A) + C +(M+L + (C + C) + C))");
	std::cout << "\ninput: " << s;
	TEquationPtr ptr = ParseString(s);
	std::cout << "\nresult: ";
	if(!ptr) { std::cout << "parsing failed\n"; return; }
	ptr->Print(std::cout);
	std::cout << "\n";
}

void main() {
	TestClosingBracket();
	TestParsing();

	system("pause");
}

bool AllowedAsChar( char c ) {
	return c!='+'
		&& c!='('
		&& c!=')'
		&& c!=' ';
}

TStrIter GetClosingBracket( TStrIter itBegin, TStrIter itEnd ) {
	for(TStrIter i=itBegin; i != itEnd; i++) {
		// stop when found closing bracket
		if(*i == ')') {
			return i;
		}
		// skip subexpressions
		else if(*i == '(') {
			i = GetClosingBracket(i+1, itEnd); 
		}
	}
	throw EParseException("No closing bracket");
}

TEquationPtr ReadEquation( TStrIter &itBegin, TStrIter itEnd ) {
	if(itBegin == itEnd) throw EParseException("empty equation");
	// Single char
	if(AllowedAsChar(*itBegin)) {
		CEquationLeaf* result = new CEquationLeaf(*itBegin);
		itBegin++;
		return TEquationPtr(result);
	}
	// Compound expression in ()
	if(*itBegin == '(') {
		// Find substring in braces
		TStrIter itEqEnd = GetClosingBracket(itBegin+1, itEnd);
		// Get all items in braces
		std::vector<TEquationPtr> sum = ParseSum(itBegin+1, itEqEnd);
		if(sum.empty()) throw EParseException("Empty bracket contents");
		CEquationBranch* result = new CEquationBranch(sum);
		itBegin = itEqEnd+1;
		return TEquationPtr(result);
	}
	throw EParseException("unexpected character");
}

std::vector<TEquationPtr> ParseSum( TStrIter itBegin, TStrIter itEnd ) {
	std::vector<TEquationPtr> result;
	if(itBegin == itEnd) return result;
	while(itBegin != itEnd) {
		// Read equation
		result.push_back(ReadEquation(itBegin, itEnd)); // itBegin changes here
		// Success if string ended
		if(itBegin ==itEnd) return result;
		// Otherwise skip '+'
		if(*itBegin == '+') itBegin++;
		else throw EParseException("Expected plus sign, not found");
	}
	// Make sure string doesn't end with a plus
	throw EParseException("Expected equation after plus sign");
}

void RemoveSpaces( std::string &s ) {
	s.erase(std::remove(s.begin(), s.end(),' '), s.end());
}

TEquationPtr ParseString( std::string str ) {
	RemoveSpaces(str);
	try {
		TStrIter itBegin = str.begin();
		return ReadEquation(itBegin, str.end());
	} catch(EParseException e) {
		printf("error: %s", e.what());
		return TEquationPtr();
	}
}
