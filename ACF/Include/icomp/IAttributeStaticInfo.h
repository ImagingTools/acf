#ifndef icomp_IAttributeStaticInfo_included
#define icomp_IAttributeStaticInfo_included


#include <string>

#include "istd/IPolymorphic.h"

#include "iser/ISerializable.h"

#include "icomp/icomp.h"


namespace icomp
{


class IAttributeStaticInfo: virtual public istd::IPolymorphic
{
public:
	/**
		Create instance of this attribute.
	 */
	virtual iser::ISerializable* CreateAttribute() const = 0;

	/**
		Get description of this attribute.
	 */
	virtual const std::string& GetAttributeDescription() const = 0;

	/**
		Get default attribute value.
		\return	pointer to default attribute value, if set. Elsewhere it returns NULL.
	*/
	virtual const iser::ISerializable* GetAttributeDefaultValue() const = 0;

	/**
		Return attribute type description.
	 */
	virtual const type_info& GetAttributeType() const = 0;

	/**
		Get related interface type.
		If there is no interface typeid(void) will be returned.
		This type in normally used by references and factories.
	*/
	virtual const type_info& GetRelatedInterfaceType() const = 0;

	/**
		Check if this attribute is obligatory.
		If attribute is obligatory, its value shouldn't be NULL.
		Please note, that this flag is only suggestion for registry designer.
	 */
	virtual bool IsObligatory() const = 0;
};


}//namespace icomp


#endif // !icomp_IAttributeStaticInfo_included


