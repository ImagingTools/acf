#ifndef ibase_INamedObject_included
#define ibase_INamedObject_included


#include "ibase/ibase.h"


#include "istd/INamed.h"

#include "ibase/IObject.h"


namespace ibase
{


class INamedObject: virtual public IObject, virtual public INamed
{
};


} // namespace ibase


#endif // !ibase_INamedObject_included

