#include "Parsing.h"
#include "ElementImpl.h"

#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <string>

//////////////////////////////////////////////////////////////////////////

typedef std::string::iterator TStrIter;

// Exception can be thrown inside internal parser functions
class EParseException : public std::logic_error {
public:
	EParseException(std::string s) : std::logic_error(s) {}
};

// Factory function, which creates an equation by its string
TEquationPtr ReadEquation(TStrIter &itBegin, TStrIter itEnd);

// Tells if character c is allowed as an equation leaf
bool AllowedAsChar(char c);

// Given a substring starting _after_ opening bracket
// returns the position of matching closing bracket
TStrIter GetClosingBracket(TStrIter itBegin, TStrIter itEnd); 

// Parses sum of equations, each item being an IEquation descendant
std::vector<TEquationPtr> ParseSum(TStrIter itBegin, TStrIter itEnd);

// Remove spaces in a string
void RemoveSpaces(std::string &s);

//////////////////////////////////////////////////////////////////////////

TEquationPtr ParseString( const char* cstr ) {
	std::string str(cstr);
	RemoveSpaces(str);
	try {
		TStrIter itBegin = str.begin();
		TEquationPtr result = ReadEquation(itBegin, str.end());
		if(itBegin != str.end()) throw EParseException("Training characters after equation");
		return result;
	} catch(EParseException e) {
		std::cout << "in string: \"" << str << "\" ";
		std::cout << "error: " << e.what();
		std::cout << "\n";
		return TEquationPtr();
	}
}

void RemoveSpaces( std::string &s ) {
	s.erase(std::remove(s.begin(), s.end(),' '), s.end());
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
	throw EParseException("Unexpected character");
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

//////////////////////////////////////////////////////////////////////////

void TestClosingBracket() {
	std::string s("(123+dfsd +(1+(2+1) )+1) + 200 + (1+2)");
	TStrIter itClosing = GetClosingBracket(s.begin()+1, s.end());
	std::cout << ((itClosing - s.begin()) ? "ok" : "oops") << "\n";
	std::string s2(s.begin(), itClosing+1);
	std::cout << s2 << "\n";
}