#ifndef myNamespace_CExample_included
#define myNamespace_CExample_included


// ACF includes
#include "istd/IPolymorphic.h"
#include "icomp/CComponentBase.h"


namespace myNamespace
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
class CExample: public icomp::CComponentBase, virtual public ITestInterface
{
	typedef CComponentBase BaseClass;	// OBLIGATORY definition of component base class type as 'BaseClass'

	// initialize all component attributes and references
	I_BEGIN_COMPONENT(CExample);
		I_REGISTER_INTERFACE(ITestInterface);
		I_ASSIGN(m_value, "Value", "Test of attribute", false, 0);
		I_ASSIGN_MULTI_0(m_valueList, "ValueList", "Test of attribute list", false);
		I_ASSIGN(m_cloneableRef, "Cloneable", "Test of reference", false, "Cloneable");
		I_ASSIGN_MULTI_0(m_cloneableListRef, "CloneableList", "Test of reference list", false);
		I_ASSIGN(m_cloneableFact, "CloneableFactory", "Test of factory", false, "CloneableFactory");
		I_ASSIGN_MULTI_0(m_cloneableListFact, "CloneableFactoryList", "Test of factory list", false);
	I_END_COMPONENT;

public:
	CExample();

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
	I_REF(ITestInterface, m_cloneableRef);
	/**
		Set of references to other components.
	*/
	I_MULTIREF(ITestInterface, m_cloneableListRef);
	/**
		Normal factory.
	*/
	I_FACT(ITestInterface, m_cloneableFact);
	/**
		Set of factories.
	*/
	I_MULTIFACT(ITestInterface, m_cloneableListFact);
};


} // namespace myNamespace


#endif // !myNamespace_CExample_included


