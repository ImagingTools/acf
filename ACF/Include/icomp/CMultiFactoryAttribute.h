#ifndef icomp_CMultiFactoryAttribute_included
#define icomp_CMultiFactoryAttribute_included


// STL includes
#include <vector>

#include "icomp/TMultiAttribute.h"


namespace icomp
{


/**
	Special attribute used to store list of referenced component ID's.
*/
class CMultiFactoryAttribute: public TMultiAttribute<std::string>
{
public:
	typedef TMultiAttribute<std::string> BaseClass;

	CMultiFactoryAttribute(){}
	CMultiFactoryAttribute(const CMultiFactoryAttribute& attribute):BaseClass(attribute){}
	CMultiFactoryAttribute(int elementsCount, std::string* valuesPtr):BaseClass(elementsCount, valuesPtr){}
};


} // namespace icomp


#endif // icomp_CMultiFactoryAttribute_included


