#ifndef icomp_IComponentStaticInfo_included
#define icomp_IComponentStaticInfo_included


#include <string>

#include "istd/IPolymorphic.h"
#include "istd/TIMap.h"
#include "istd/CClassInfo.h"

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

	\ingroup ComponentConcept
*/
class IComponentStaticInfo: virtual public istd::IPolymorphic
{
public:
	typedef void* (*InterfaceExtractorPtr)(IComponent* componentPtr);

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

	/**
		Describes assignment of the component to a logical category.
	*/
	enum ComponentCategoryType
	{
		/**
			Component is not assigned to any category.
		*/
		CCT_NONE = 0,

		/**
			Component serves for realization of the application logic or architecture.
		*/
		CCT_APPLICATION_LOGIC = 1,

		/**
			Component serves for realization of the multimedia services.
		*/
		CCT_MULTIMEDIA = 2,

		/**
			Component serves for realization of the data model used in the application logic.
		*/
		CCT_DATA = 4,

		/**
			Component serves for realization of the data model persistency.
		*/
		CCT_PERSISTENCY = 8,

		/**
			General graphical user interface component.
		*/
		CCT_GUI = 16,

		/**
			Data model presentation. Usually, this category is used with CCT_GUI.
		*/
		CCT_DATA_PRESENTATION = 32,

		/**
			Component serves for a general implementation of data processing.
		*/
		CCT_DATA_PROCESSING = 64,
		
		/**
			Component realized the application's entry point.
		*/
		CCT_APPLICATION = 128,
	
		/**
			Component realize a service. 
		*/
		CCT_SERVICE = 256
	};


	/**
		Map from class type to interface extractor implementation.
	*/
	typedef istd::TIMap<istd::CClassInfo, InterfaceExtractorPtr> InterfaceExtractors;
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
		Create component instance.
		\return				pointer to created component or NULL if this component cannot be created.
	 */
	virtual IComponent* CreateComponent() const = 0;

	/**
		Returns the interface ID with the index @c index.
	*/
	virtual const InterfaceExtractors& GetInterfaceExtractors() const = 0;

	/**
		Get number of attributes.
	*/
	virtual const AttributeInfos& GetAttributeInfos() const = 0;

	/**
		Get list of subcomponent ID's.
	*/
	virtual Ids GetSubcomponentIds() const = 0;

	/**
		Return number of interfaces for specific slot.
		\param	subcomponentId	ID of subcomponent.
								\sa	GetSubcomponentIds().
	*/
	virtual const IComponentStaticInfo* GetSubcomponentInfo(const std::string& subcomponentId) const = 0;

	/**
		Register interface ID for this static component info.
		This interface ID is used for static check
		if this component can be used to resolve reference dependecy of second one.
	*/
	virtual bool RegisterInterfaceExtractor(const istd::CClassInfo& interfaceId, InterfaceExtractorPtr extractorPtr) = 0;

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
	virtual const istd::CString& GetDescription() const = 0;

	/**
		Get keywords for this component.
	*/
	virtual const istd::CString& GetKeywords() const = 0;
};


}//namespace icomp


#endif // !icomp_IComponentStaticInfo_included


