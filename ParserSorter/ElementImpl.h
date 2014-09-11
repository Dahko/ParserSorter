#pragma once
#include "Element.h"
#include <vector>
#include <iosfwd>

class CEquationLeaf : public IEquation {
public:
	CEquationLeaf(TElement el);
	virtual bool IsLeaf() const;
	virtual TElement GetMinElement() const;
	virtual void Sort(); 
	virtual void Print( std::ostream &os );
private:
	TElement m_el;
};

class SortingPredicate {
public:
	bool operator()(TEquationPtr lhs, TEquationPtr rhs) {
		// Null checks. Maybe need to do something to guarantee this beforehead
		if(!lhs) return true;
		if(!rhs) return false;

		TElement lMin = lhs->GetMinElement();
		TElement rMin = rhs->GetMinElement();
		if(lMin < rMin) return true;
		if(lMin == rMin && lhs->IsLeaf() && !rhs->IsLeaf()) return true;
		return false;
	}
};

class CEquationBranch : public IEquation {
public:
	CEquationBranch(std::vector<TEquationPtr> const& childs);
	virtual bool IsLeaf() const;
	virtual TElement GetMinElement() const;
	virtual void Print( std::ostream &os );
	virtual void Sort();
public:
	std::vector<TEquationPtr> m_childs;
};

