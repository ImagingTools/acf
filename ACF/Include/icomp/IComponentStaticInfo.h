#ifndef icomp_IComponentStaticInfo_included
#define icomp_IComponentStaticInfo_included


#include <string>

#include "istd/IPolymorphic.h"
#include "istd/TCascadedMap.h"

#include "icomp/IComponent.h"
#include "icomp/IAttributeStaticInfo.h"
#include "icomp/IComponentStaticInfo.h"

#include "icomp/icomp.h"


namespace icomp
{


/**
	This interface provide static information about component meta info.
	Components can implements interfaces and contain attributes and subcomponents.
	Attributes have values specified in IComponentContext object.
*/
class IComponentStaticInfo: virtual public istd::IPolymorphic
{
public:
	typedef void* (*InterfaceExtractorPtr)(IComponent* componentPtr);

	typedef istd::TCascadedMap< std::string, InterfaceExtractorPtr> InterfaceExtractors;
	typedef istd::TCascadedMap< std::string, const IAttributeStaticInfo*> AttributeInfos;
	typedef std::set< std::string > Ids;

	/**
		Create component instance.
		\param	contextPtr	pointer to context of new created component.
							This context object is same like template and is used to describe component
							its attributes and connections to other components.
							It cannot be NULL.
		\return				pointer to created component or NULL if this component cannot be created.
	 */
	virtual IComponent* CreateComponent(const IComponentContext* contextPtr) const = 0;

	/**
		Returns the interface ID with the index @c index.
	*/
	virtual const InterfaceExtractors& GetInterfaceExtractors() const = 0;

	/**
		Get number of attributes.
	*/
	virtual const AttributeInfos& GetAttributeInfos() const = 0;

	/**
		Return number of interfaces for specific slot.
	*/
	virtual const IComponentStaticInfo* GetSubcomponent(const std::string& subcomponentId) const = 0;

	/**
		Register interface ID for this static component info.
		This interface ID is used for static check
		if this component can be used to resolve reference dependecy of second one.
	*/
	virtual bool RegisterInterfaceExtractor(const std::string& interfaceId, InterfaceExtractorPtr extractorPtr) = 0;

	/**
		Register static attribute info.
		\param	attributeId			ID of attribute.
		\param	attributeInfoPtr	static attribute info object used to describe attribute type and as factory.
									It cannot be NULL.
	*/
	virtual bool RegisterAttributeInfo(const std::string& attributeId, const IAttributeStaticInfo* attributeInfoPtr) = 0;

	/**
		Register static subcomponent info.
		\param	subcomponentId		ID of this subcomponent.
		\param	componentInfoPtr	static subcomponent info object used to describe subcomponent type and as factory.
									It cannot be NULL.
	*/
	virtual bool RegisterSubcomponentInfo(const std::string& subcomponentId, const IComponentStaticInfo* componentInfoPtr) = 0;

	/**
		Get human readable description of this component.
	*/
	virtual istd::CString GetDescription() const = 0;

	/**
		Get keywords for this component.
	*/
	virtual istd::CString GetKeywords() const = 0;
};


}//namespace icomp


#endif // !icomp_IComponentStaticInfo_included


