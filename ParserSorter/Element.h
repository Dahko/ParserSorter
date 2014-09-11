#pragma once
#include <memory>

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

