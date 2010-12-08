#ifndef icomp_IComponentStaticInfo_included
#define icomp_IComponentStaticInfo_included


#include <string>

#include "istd/IPolymorphic.h"
#include "istd/TIMap.h"
#include "istd/CString.h"

#include "icomp/icomp.h"


namespace icomp
{


class IAttributeStaticInfo;
class IComponent;


/**
	This interface provide static information about component meta info.
	Components can implements interfaces and contain attributes and subcomponents.
	Attributes have values specified in IComponentContext object.

	\ingroup ComponentConcept
*/
class IComponentStaticInfo: virtual public istd::IPolymorphic
{
public:
	/**
		Specify type of the component realization
	*/
	enum ComponentType
	{
		/**
			Undefined.
		*/
		CT_NONE,

		/**
			Component is realized as a class and managed by the ACF registry package (arp).
		*/
		CT_REAL,

		/**
			Component is the composition of real or other composite components.
		*/
		CT_COMPOSITE
	};

	enum MetaGroupId
	{
		/**
			ID of meta group storing list of supported interfaces.
		*/
		MGI_INTERFACES = 0,
		/**
			ID of meta group storing list of attributes.
		*/
		MGI_ATTRIBUTES,
		/**
			ID of group for subcomponents.
		*/
		MGI_SUBCOMPONENTS,
		/**
			ID of group for embedded types.
		*/
		MGI_EMBEDDED_COMPONENTS
	};
	/**
		Map from attribute name string to attribute static info object.
	*/
	typedef istd::TIMap<std::string, const IAttributeStaticInfo*> AttributeInfos;
	typedef std::set<std::string> Ids;

	/**
		Get information about component type.
	*/
	virtual int GetComponentType() const = 0;

	/**
		Get set of attributes.
	*/
	virtual const IAttributeStaticInfo* GetAttributeInfo(const std::string& attributeId) const = 0;

	/**
		Return number of interfaces for specific slot.
		\param	subcomponentId	ID of subcomponent.
								\sa	GetMetaIds() and MGI_SUBCOMPONENTS.
	*/
	virtual const IComponentStaticInfo* GetSubcomponentInfo(const std::string& subcomponentId) const = 0;

	/**
		Return number of interfaces for specific slot.
		\param	subcomponentId	ID of subcomponent.
								\sa	GetMetaIds() and MGI_EMBEDDED_COMPONENTS.
	*/
	virtual const IComponentStaticInfo* GetEmbeddedComponentInfo(const std::string& embeddedId) const = 0;

	/**
		Get list of meta IDs associated with some meta key.
		\sa MetaGroupId
	*/
	virtual Ids GetMetaIds(int metaGroupId) const = 0;

	/**
		Get human readable description of this component.
	*/
	virtual const istd::CString& GetDescription() const = 0;

	/**
		Get keywords for this component.
	*/
	virtual const istd::CString& GetKeywords() const = 0;
};


}//namespace icomp


#endif // !icomp_IComponentStaticInfo_included


