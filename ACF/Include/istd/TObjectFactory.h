#ifndef istd_TObjectFactory_h_included
#define istd_TObjectFactory_h_included


#include "istd/istd.h"


#include "istd/IObjectFactory.h"


namespace istd
{


/**
	Standard template implementation of the IObjectFactory
*/
template <class InterfaceClass, class ImplementationClass>
class TObjectFactory: public istd::IObjectFactory<InterfaceClass>
{
public:
	typedef istd::IObjectFactory<InterfaceClass> BaseInterface;

	// reimplemented (istd::IObjectFactory)
 	virtual InterfaceClass* CreateObject();
};


template <class InterfaceClass, class ImplementationClass>
InterfaceClass* TObjectFactory<InterfaceClass, ImplementationClass>::createObject()
{
	return new ImplementationClass;
}


} // namespace istd


#endif // !istd_TObjectFactory_h_included

