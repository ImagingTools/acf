#ifndef istdc_TSingleFactoryComp_included
#define istdc_TSingleFactoryComp_included


#include "istd/TSingleFactory.h"

#include "icomp/CComponentBase.h"


namespace istdc
{


template <class InterfaceType, class ImplementationClass>
class TSingleFactoryComp:	public icomp::CComponentBase, 
							public istd::TSingleFactory<InterfaceType, ImplementationClass>
{
public:
	typedef istd::TIFactory<InterfaceType> BaseInterface;
	typedef icomp::CComponentBase BaseClass;
	typedef istd::TSingleFactory<InterfaceType, ImplementationClass> BaseClass2;
	typedef TSingleFactoryComp<InterfaceType, ImplementationClass> ComponentClass;

	I_BEGIN_COMPONENT(ComponentClass)
		I_REGISTER_INTERFACE(BaseInterface)
		I_ASSIGN(m_keyAttrPtr, "FactoryKey", "Factory key", false, "")
	I_END_COMPONENT

	TSingleFactoryComp();

	// reimplemented (istd::TIFactory)
	KeyList GetFactoryKeys() const;
	InterfaceType* CreateInstance(const std::string& keyId) const;

private:
	I_ATTR(istd::CString, m_keyAttrPtr);
};


template <class InterfaceType, class ImplementationClass>
TSingleFactoryComp<InterfaceType, ImplementationClass>::TSingleFactoryComp()
	:BaseClass2(std::string())
{
}


template <class InterfaceType, class ImplementationClass>
typename istd::TIFactory<InterfaceType>::KeyList TSingleFactoryComp<InterfaceType, ImplementationClass>::GetFactoryKeys() const
{
	KeyList retVal;

	if (m_keyAttrPtr.IsValid()){
		retVal.push_back(m_keyAttrPtr->GetValue().ToString());
	}

	return retVal;
}


template <class InterfaceType, class ImplementationClass>
InterfaceType* TSingleFactoryComp<InterfaceType, ImplementationClass>::CreateInstance(const std::string& keyId) const
{
	if (keyId.empty() || (m_keyAttrPtr.IsValid() && keyId == m_keyAttrPtr->GetValue().ToString())){
		return new ImplementationClass;
	}
	
	return NULL;
}


} // namespace istdc


#endif // !istdc_TSingleFactoryComp_included
