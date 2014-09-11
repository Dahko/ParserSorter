#pragma once
#include <string>
#include <stdexcept>
#include <algorithm>
#include <vector>

typedef char TElement;

class IEquation {
public:
	virtual bool IsLeaf() const = 0;
	virtual TElement GetMinElement() const = 0; 
	virtual void Sort() = 0;
	/*virtual unsigned GetChilds();
	virtual IEquation* Getchild();*/
	virtual void Print(std::ostream &os) = 0;

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
	virtual void Sort() { return; } // always sorted
	virtual void Print( std::ostream &os ) {
		os << m_el;
	}

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
	CEquationBranch(std::vector<TEquationPtr> const& childs) : m_childs(childs) {}
	virtual bool IsLeaf() const {
		return false;
	}

	virtual TElement GetMinElement() const {
		if(m_childs.empty()) { throw std::logic_error("oops"); };
		TElement minimum = m_childs[0]->GetMinElement();
		for(unsigned i=0; i<m_childs.size(); i++) {
			TElement localMin = m_childs[i]->GetMinElement();
			if(localMin < minimum) minimum = localMin;
		}
		return minimum;
	}

	virtual void Print( std::ostream &os ) {
		os << "(";
		for(size_t i=0; i < m_childs.size(); i++) {
			if(i!=0) os<<"+";
			m_childs[i]->Print(os);
		}
		os << ")";
	}
	virtual void Sort() {
		for(unsigned i=0; i<m_childs.size(); i++) {
			m_childs[i]->Sort();
		}
		std::sort(m_childs.begin(), m_childs.end(), SortingPredicate());

	}
public:
	std::vector<TEquationPtr> m_childs;
};

