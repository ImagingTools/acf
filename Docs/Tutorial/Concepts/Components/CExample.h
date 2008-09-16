#ifndef CExample_included
#define CExample_included


#include "istd/IPolymorphic.h"

#include "icomp/CComponentBase.h"


namespace itut
{


/**
	This is example interface only.
	Normally it shouldn't be defined in class file.
*/
class ITestInterface: public istd::IPolymorphic
{
public:
	virtual int GetValue() const = 0;
};


/**
	Demonstrate using of all elements of component concept.
*/
class CExample: public icomp::CComponentBase, public ITestInterface
{
	typedef CComponentBase BaseClass;

	// initialize all component attributes and references
	I_BEGIN_COMPONENT(CExample)
		I_ASSIGN(m_value, "Value", "Test of attribute", false, 0)
		I_ASSIGN_MULTI_0(m_valueList, "ValueList", "Test of attribute list", false)
		I_ASSIGN(m_cloneableRef, "Cloneable", "Test of reference", false, "Cloneable")
		I_ASSIGN_MULTI_0(m_cloneableListRef, "CloneableList", "Test of reference list", false)
		I_ASSIGN(m_cloneableFact, "CloneableFactory", "Test of factory", false, "CloneableFactory")
		I_ASSIGN_MULTI_0(m_cloneableListFact, "CloneableFactoryList", "Test of factory list", false)
	I_END_COMPONENT

public:
	CExample(const icomp::IComponentContext* contextPtr);

private:
	/**
		Normal attribute.
	*/
	I_ATTR(int, m_value);
	/**
		Set of attributes.
	*/
	I_MULTIATTR(int, m_valueList);
	/**
		Normal reference to other component.
	*/
	I_REF(IClonable, m_cloneableRef);
	/**
		Set of references to other components.
	*/
	I_MULTIREF(IClonable, m_cloneableListRef);
	/**
		Normal factory.
	*/
	I_FACT(IClonable, m_cloneableFact);
	/**
		Set of factories.
	*/
	I_MULTIFACT(IClonable, m_cloneableListFact);
};


} // namespace itut


#indif // !CExample_included


