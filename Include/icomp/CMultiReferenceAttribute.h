#ifndef icomp_CMultiReferenceAttribute_included
#define icomp_CMultiReferenceAttribute_included


// STL includes
#include <string>

#include "icomp/TMultiAttribute.h"


namespace icomp
{


/**
	Special attribute used to store list of referenced component ID's.
*/
class CMultiReferenceAttribute: public CMultiStdStringAttribute
{
public:
	typedef CMultiStdStringAttribute BaseClass;

	enum DefaultAttributeFlags
	{
		DAF_OBLIGATORY = IAttributeStaticInfo::AF_OBLIGATORY | IAttributeStaticInfo::AF_REFERENCE,
		DAF_OPTIONAL = IAttributeStaticInfo::AF_NULLABLE | IAttributeStaticInfo::AF_REFERENCE
	};

	CMultiReferenceAttribute(){}
	CMultiReferenceAttribute(const CMultiReferenceAttribute& attribute):BaseClass(attribute){}
	CMultiReferenceAttribute(int elementsCount, std::string* valuesPtr):BaseClass(elementsCount, valuesPtr){}

	// reimplemented (iser::IObject)
	const std::string& GetFactoryId() const;

	// static methods
	static const std::string& GetTypeName();

private:
	static const std::string s_typeName;
};


} // namespace icomp


#endif // icomp_CMultiReferenceAttribute_included


