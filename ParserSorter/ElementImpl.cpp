#include "ElementImpl.h"
#include <algorithm>
#include <ostream>


bool CEquationLeaf::IsLeaf() const {
	return true;
}

TElement CEquationLeaf::GetMinElement() const {
	return m_el;
}

void CEquationLeaf::Sort() {
	return; // always sorted;
}

void CEquationLeaf::Print( std::ostream &os ) {
	os << m_el;
}

CEquationLeaf::CEquationLeaf( TElement el ) : m_el(el) {

}

//////////////////////////////////////////////////////////////////////////

CEquationBranch::CEquationBranch( std::vector<TEquationPtr> const& childs ) : m_childs(childs) {

}

bool CEquationBranch::IsLeaf() const {
	return false;
}

TElement CEquationBranch::GetMinElement() const {
	if(m_childs.empty()) { throw std::logic_error("oops"); };
	TElement minimum = m_childs[0]->GetMinElement();
	for(unsigned i=0; i<m_childs.size(); i++) {
		TElement localMin = m_childs[i]->GetMinElement();
		if(localMin < minimum) minimum = localMin;
	}
	return minimum;
}

void CEquationBranch::Print( std::ostream &os ) {
	os << "(";
	for(size_t i=0; i < m_childs.size(); i++) {
		if(i!=0) os<<"+";
		m_childs[i]->Print(os);
	}
	os << ")";
}

void CEquationBranch::Sort() {
	for(unsigned i=0; i<m_childs.size(); i++) {
		m_childs[i]->Sort();
	}
	std::sort(m_childs.begin(), m_childs.end(), SortingPredicate());
}
