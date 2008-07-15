#ifndef icomp_CReferenceAttribute_included
#define icomp_CReferenceAttribute_included


// STL includes
#include <string>

#include "icomp/TSingleAttribute.h"


namespace icomp{


/**
	Special attribute used to store referenced component ID.
*/
class CReferenceAttribute: public TSingleAttribute<std::string>
{
public:
	typedef TSingleAttribute<std::string> BaseClass;

	CReferenceAttribute(){}
	CReferenceAttribute(const CReferenceAttribute& attribute):BaseClass(attribute){}
	explicit CReferenceAttribute(const std::string& value):BaseClass(value){}
};


} // namespace icomp


#endif // !icomp_CReferenceAttribute_included


