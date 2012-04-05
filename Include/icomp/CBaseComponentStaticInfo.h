#ifndef icomp_CBaseComponentStaticInfo_included
#define icomp_CBaseComponentStaticInfo_included


#include "icomp/IRealComponentStaticInfo.h"
#include "icomp/CComponentStaticInfoBase.h"


namespace icomp
{


/**
	Standard implementation of static info for base component classes.
	The main difference to 'normal' component static info is, that instances of such components cannot be created.
*/
class CBaseComponentStaticInfo:
			public CComponentStaticInfoBase,
			virtual public IRealComponentStaticInfo
{
public:
	typedef CComponentStaticInfoBase BaseClass;

	typedef void* (*InterfaceExtractorPtr)(IComponent& component);

	CBaseComponentStaticInfo(const IRealComponentStaticInfo* baseComponentPtr = NULL);

	/**
		Register interface ID for this static component info.
		This interface ID is used for static check
		if this component can be used to resolve reference dependecy of second one.
	*/
	virtual void RegisterInterfaceExtractor(const std::string& interfaceName, InterfaceExtractorPtr extractorPtr);
	/**
		Register static attribute info.
		\param	attributeId			ID of attribute.
		\param	attributeInfoPtr	static attribute info object used to describe attribute type and as factory.
									It cannot be NULL.
	*/
	virtual void RegisterAttributeInfo(const std::string& attributeId, const IAttributeStaticInfo* attributeInfoPtr);
	/**
		Register static subcomponent info.
		\param	subcomponentId		ID of this subcomponent.
		\param	componentInfoPtr	static subcomponent info object used to describe subcomponent type and as factory.
									It cannot be NULL.
	*/
	virtual void RegisterSubelementInfo(const std::string& subcomponentId, const IElementStaticInfo* staticInfoPtr);

	//	reimplemented (icomp::IRealComponentStaticInfo)
	virtual IComponent* CreateComponent() const;

	//	reimplemented (icomp::IComponentInterfaceExtractor)
	virtual void* GetComponentInterface(
				const istd::CClassInfo& interfaceType,
				IComponent& component,
				const std::string& subId) const;

	//	reimplemented (icomp::IComponentStaticInfo)
	virtual const IAttributeStaticInfo* GetAttributeInfo(const std::string& attributeId) const;

	//	reimplemented (icomp::IElementStaticInfo)
	virtual const IElementStaticInfo* GetSubelementInfo(const std::string& subcomponentId) const;
	virtual Ids GetMetaIds(int metaGroupId) const;

private:
	const IRealComponentStaticInfo* m_baseComponentPtr;

	typedef QMap<std::string, InterfaceExtractorPtr> InterfaceExtractors;
	InterfaceExtractors m_interfaceExtractors;

	typedef QMap<std::string, const IElementStaticInfo*> SubelementInfos;
	SubelementInfos m_subelementInfos;

	typedef QMap<std::string, const IAttributeStaticInfo*> AttributeInfos;
	AttributeInfos m_attributeInfos;
};


} // namespace icomp


#endif // !icomp_CBaseComponentStaticInfo_included


