#ifndef iattr_IAttributeMetaInfo_included
#define iattr_IAttributeMetaInfo_included


// ACF includes
#include "iser/IObject.h"


namespace iattr
{


/**
	Meta information describing single attribute.
*/
class IAttributeMetaInfo: virtual public istd::IChangeable
{
public:
	/**
		Flags signalizing set of attribute properties.
	*/
	enum AttributeFlags
	{
		/**
			Active if user declared this attribute as obligatory.
		*/
		AF_OBLIGATORY = 1 << 0,
		/**
			Active if this attribute can be unset (null).
			Please note, that sometimes obligatory attributes can be nullable - for example for simple types default value can be taken.
		*/
		AF_NULLABLE	= 1 << 1,
		/**
			Attribute is single type.
		*/
		AF_SINGLE = 1 << 2,
		/**
			Attribute is multiple type.
		*/
		AF_MULTIPLE = 1 << 3,
		/**
			Attribute is simple value type.
		*/
		AF_VALUE = 1 << 4
	};

	/**
		Get description of this attribute.
	 */
	virtual QString GetAttributeDescription() const = 0;

	/**
		Get default attribute value.
		\return	pointer to default attribute value, if set. Elsewhere it returns NULL.
	*/
	virtual const iser::IObject* GetAttributeDefaultValue() const = 0;

	/**
		Return attribute type ID.
		The attribute type ID identifies unique this attribute in factory.
		\sa	istd::TIFactory::CreateInstance.
	*/
	virtual QByteArray GetAttributeTypeId() const = 0;

	/**
		Get flags of this attribute.
	 */
	virtual int GetAttributeFlags() const = 0;
};


} // namespace iattr


#endif // !iattr_IAttributeMetaInfo_included


