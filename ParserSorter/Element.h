#pragma once
#include <string>
#include <stdexcept>
#include <algorithm>
#include <vector>

typedef char TElement;

class IEquation {
public:
	virtual bool IsLeaf() const;
	virtual TElement GetMinElement() const; 

	/*virtual unsigned GetChilds();
	virtual IEquation* Getchild();*/
	virtual void Print(std::ostream &os);

	virtual ~IEquation() {};
};

typedef std::tr1::shared_ptr<IEquation> TEquationPtr; 

class CEquationLeaf : public IEquation {
public:
	CEquationLeaf(TElement el) : m_el(el) {}
	virtual bool IsLeaf() const {
		return true;
	}
	virtual TElement GetMinElement() const {
		return m_el;
	}
	virtual void Print( std::ostream &os ) {
		os << m_el;
	}

private:
	TElement m_el;
};

class CEquationBranch : public IEquation {
public:
	CEquationBranch(std::vector<TEquationPtr> const& childs) : m_childs(childs) {}
	virtual bool IsLeaf() const {
		return false;
	}

	virtual TElement GetMinElement() const {
		std::vector<TEquationPtr>::const_iterator it = std::min_element(m_childs.begin(), m_childs.end());
		if(it == m_childs.end()) { throw std::logic_error("oops"); }
		return (*it)->GetMinElement();
	}

	virtual void Print( std::ostream &os ) {
		os << "(";
		for(size_t i=0; i < m_childs.size(); i++) {
			m_childs[i]->Print(os);
			os<<"+";
		}
		os << ")";
	}
public:
	std::vector<TEquationPtr> m_childs;
};

bool AllowedAsChar(char c) {
	return c!='+'
		&& c!='('
		&& c!=')'
		&& c!=' ';
}

typedef std::string::iterator TStrIter;
class EParseException : public std::logic_error {
public:
	EParseException(std::string s) : std::logic_error(s) {}
};

// given a substring starting _after_ opening bracket
// returns the position of matching closing bracket
TStrIter GetClosingBracket(TStrIter itBegin, TStrIter itEnd) {
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

// Factory function, which creates an equation by its string
TEquationPtr ReadEquation(TStrIter &itBegin, TStrIter itEnd) {
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

// Parses sum of equations, each item being an IEquation descendant
std::vector<TEquationPtr> ParseSum(TStrIter itBegin, TStrIter itEnd) {
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

void RemoveSpaces(std::string &s) {
	s.erase(std::remove(s.begin(), s.end(),' '), s.end());
}

TEquationPtr ParseString(std::string str) {
	RemoveSpaces(str);
	try {
		std::vector<TEquationPtr> result = ParseSum(str.begin(), str.end());
		return new CEquationBranch(result);
	} catch(EParseException e) {
		printf("error: %s", e.what());
		return NULL;
	}
};