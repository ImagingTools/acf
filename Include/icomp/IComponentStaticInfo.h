#ifndef icomp_IComponentStaticInfo_included
#define icomp_IComponentStaticInfo_included


// ACF includes
#include "icomp/IElementStaticInfo.h"


namespace icomp
{


class IAttributeStaticInfo;


/**
	This interface provide static information about component meta info.
	Components can implements interfaces and contain attributes and subcomponents.
	Attributes have values specified in IComponentContext object.

	\ingroup ComponentConcept
*/
class IComponentStaticInfo: virtual public IElementStaticInfo
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
			ID of meta group storing list of attributes.
		*/
		MGI_ATTRIBUTES = IElementStaticInfo::MGI_LAST + 1,
		/**
			ID of group for embedded types.
		*/
		MGI_EMBEDDED_COMPONENTS
	};

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
								\sa	GetMetaIds() and MGI_EMBEDDED_COMPONENTS.
	*/
	virtual const IComponentStaticInfo* GetEmbeddedComponentInfo(const std::string& embeddedId) const = 0;

	/**
		Get human readable description of this component.
	*/
	virtual const istd::CString& GetDescription() const = 0;

	/**
		Get keywords for this component.
	*/
	virtual const istd::CString& GetKeywords() const = 0;
};


} // namespace icomp


#endif // !icomp_IComponentStaticInfo_included


