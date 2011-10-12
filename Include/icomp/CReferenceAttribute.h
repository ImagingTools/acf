#ifndef icomp_CReferenceAttribute_included
#define icomp_CReferenceAttribute_included


// STL includes
#include <string>

#include "icomp/TAttribute.h"


namespace icomp{


/**
	Special attribute used to store referenced component ID.
*/
class CReferenceAttribute: public CStdStringAttribute
{
public:
	typedef CStdStringAttribute BaseClass;

	enum DefaultAttributeFlags
	{
		DAF_OBLIGATORY = IAttributeStaticInfo::AF_OBLIGATORY | IAttributeStaticInfo::AF_REFERENCE,
		DAF_OPTIONAL = IAttributeStaticInfo::AF_NULLABLE | IAttributeStaticInfo::AF_REFERENCE
	};

	CReferenceAttribute(){}
	CReferenceAttribute(const CReferenceAttribute& attribute):BaseClass(attribute){}
	explicit CReferenceAttribute(const std::string& value):BaseClass(value){}

	// reimplemented (iser::IObject)
	const std::string& GetFactoryId() const;

	// static methods
	static const std::string& GetTypeName();

private:
	static const std::string s_typeName;
};


} // namespace icomp


#endif // !icomp_CReferenceAttribute_included


