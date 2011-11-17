#ifndef icomp_IAttributeStaticInfo_included
#define icomp_IAttributeStaticInfo_included


// STL includes
#include <string>


// ACF includes
#include "istd/IPolymorphic.h"

#include "iser/IObject.h"

#include "icomp/IComponentStaticInfo.h"


namespace icomp
{


class IAttributeStaticInfo: virtual public istd::IPolymorphic
{
public:
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
			Attribute is reference to another component.
		*/
		AF_REFERENCE = 1 << 2,
		/**
			Attribute is factory of another component.
		*/
		AF_FACTORY = 1 << 2
	};

	/**
		Get description of this attribute.
	 */
	virtual const std::string& GetAttributeDescription() const = 0;

	/**
		Get default attribute value.
		\return	pointer to default attribute value, if set. Elsewhere it returns NULL.
	*/
	virtual const iser::IObject* GetAttributeDefaultValue() const = 0;

	/**
		Return attribute type description.
	 */
	virtual std::string GetAttributeTypeName() const = 0;

	/**
		Get related meta information.
		This information in normally used by references and factories to provide information about related interfaces.
	*/
	virtual IElementStaticInfo::Ids GetRelatedMetaIds(int metaGroupId, int flags, int flagsMask) const = 0;

	/**
		Get flags of this attribute.
	 */
	virtual int GetAttributeFlags() const = 0;
};


} // namespace icomp


#endif // !icomp_IAttributeStaticInfo_included


