#ifndef ibase_INamedObject_included
#define ibase_INamedObject_included


#include "ibase/ibase.h"


#include "istd/INamed.h"

#include "ibase/IObject.h"


namespace ibase
{


/**
	Connection interface between IObject and INamed.
*/
class INamedObject: virtual public ibase::IObject, virtual public istd::INamed
{
};


} // namespace ibase


#endif // !ibase_INamedObject_included

