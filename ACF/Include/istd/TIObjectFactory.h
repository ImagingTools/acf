#ifndef istd_TIObjectFactory_h_included
#define istd_TIObjectFactory_h_included


#include "istd/istd.h"


#include "istd/IPolymorphic.h"


namespace istd
{


/**
	Template based factory interface.
*/
template <class ObjectClass>
class TIObjectFactory: public istd::IPolymorphic
{
public:
	virtual ObjectClass* CreateObject() = 0;
};


} // namespace istd


#endif // istd_TIObjectFactory_h_included

