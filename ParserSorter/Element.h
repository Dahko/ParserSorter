#pragma once

typedef char TElement;

class IEquation {
public:
	virtual IsLeaf() const;
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
	virtual IsLeaf() const {
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
	virtual IsLeaf() const {
		return false;
	}

	virtual TElement GetMinElement() const {
		std::vector<TEquationPtr>::iterator it = std::min_element(m_childs.begin(), m_childs.end());
		if(it == m_childs.end()) { throw std::logic_error("oops"); }
		return it->GetMinElement();
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