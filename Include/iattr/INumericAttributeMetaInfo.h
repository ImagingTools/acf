#ifndef iattr_INumericAttributeMetaInfo_included
#define iattr_INumericAttributeMetaInfo_included


#include <iattr/IAttributeMetaInfo.h>


namespace iattr
{


/**
	Represents constraints of properties allowing values from enumerated set.
*/
class INumericAttributeMetaInfo: virtual public IAttributeMetaInfo
{
public:
	/**
		Get minimal allowed value for some attribute.
	*/
	virtual bool GetMinimalValue(iser::IObject& result) const = 0;
	/**
		Get maximal allowed value for some attribute.
	*/
	virtual bool GetMaximalValue(iser::IObject& result) const = 0;
};


} // namespace iattr


#endif // !iattr_INumericAttributeMetaInfo_included


