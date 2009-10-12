#ifndef icomp_CFactoryAttribute_included
#define icomp_CFactoryAttribute_included


// STL includes
#include <string>

#include "icomp/TAttribute.h"


namespace icomp{


/**
	Special attribute used to store referenced component ID.
*/
class CFactoryAttribute: public TAttribute<std::string>
{
public:
	typedef TAttribute<std::string> BaseClass;

	CFactoryAttribute(){}
	CFactoryAttribute(const CFactoryAttribute& attribute):BaseClass(attribute){}
	explicit CFactoryAttribute(const std::string& value):BaseClass(value){}
};


} // namespace icomp


#endif // !icomp_CFactoryAttribute_included


