#pragma once


// ACF includes
#include <icomp/IRealComponentStaticInfo.h>
#include <icomp/CComponentStaticInfoBase.h>


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

	static istd::CClassInfo s_compInterfaceType;

	explicit CBaseComponentStaticInfo(const IRealComponentStaticInfo* baseComponentPtr = NULL);

	/**
		Register interface ID for this static component info.
		This interface ID is used for static check
		if this component can be used to resolve reference dependecy of second one.
	*/
	virtual void RegisterInterfaceExtractor(const QByteArray& interfaceName, InterfaceExtractorPtr extractorPtr);
	/**
		Register static attribute info.
		\param	attributeId			ID of attribute.
		\param	attributeInfoPtr	static attribute info object used to describe attribute type and as factory.
									It cannot be NULL.
	*/
	virtual void RegisterAttributeInfo(const QByteArray& attributeId, const IAttributeStaticInfo* attributeInfoPtr);
	/**
		Register static subcomponent info.
		\param	subcomponentId		ID of this subcomponent.
		\param	componentInfoPtr	static subcomponent info object used to describe subcomponent type and as factory.
									It cannot be NULL.
	*/
	virtual void RegisterSubelementInfo(const QByteArray& subcomponentId, const IElementStaticInfo* staticInfoPtr);

	//	reimplemented (icomp::IRealComponentStaticInfo)
	virtual IComponentUniquePtr CreateComponent() const override;

	//	reimplemented (icomp::IComponentInterfaceExtractor)
	virtual void* GetComponentInterface(
				const istd::CClassInfo& interfaceType,
				IComponent& component,
				const QByteArray& subId) const override;

	//	reimplemented (icomp::IComponentStaticInfo)
	virtual const IAttributeStaticInfo* GetAttributeInfo(const QByteArray& attributeId) const override;

	//	reimplemented (icomp::IElementStaticInfo)
	virtual Ids GetMetaIds(int metaGroupId) const override;
	virtual const IElementStaticInfo* GetSubelementInfo(const QByteArray& subcomponentId) const override;

	//	reimplemented (iattr::IAttributesMetaInfoProvider)
	virtual iattr::IAttributesProvider::AttributeIds GetAttributeMetaIds() const override;
	virtual const iattr::IAttributeMetaInfo* GetAttributeMetaInfo(const QByteArray& attributeId) const override;

private:
	const IRealComponentStaticInfo* m_baseComponentPtr;

	typedef QMap<QByteArray, InterfaceExtractorPtr> InterfaceExtractors;
	InterfaceExtractors m_interfaceExtractors;

	typedef QMap<QByteArray, const IElementStaticInfo*> SubelementInfos;
	SubelementInfos m_subelementInfos;

	typedef QMap<QByteArray, const IAttributeStaticInfo*> AttributeInfos;
	AttributeInfos m_attributeInfos;
};


} // namespace icomp


