#ifndef icomp_CBaseComponentStaticInfo_included
#define icomp_CBaseComponentStaticInfo_included


#include "icomp/CComponentStaticInfoBase.h"


namespace icomp
{


/**
	Standard implementation of static info for base component classes.
	The main difference to 'normal' component static info is, that instances of such components cannot be created.
*/
class CBaseComponentStaticInfo: virtual public CComponentStaticInfoBase
{
public:
	typedef CComponentStaticInfoBase BaseClass;

	CBaseComponentStaticInfo(const IComponentStaticInfo* baseComponentPtr = NULL);

	//	reimplemented (icomp::IComponentStaticInfo)
	virtual IComponent* CreateComponent() const;
	virtual const InterfaceExtractors& GetInterfaceExtractors() const;
	virtual const AttributeInfos& GetAttributeInfos() const;
	virtual Ids GetSubcomponentIds() const;
	virtual const IComponentStaticInfo* GetSubcomponentInfo(const std::string& subcomponentId) const;
	virtual bool RegisterInterfaceExtractor(const std::string& interfaceId, InterfaceExtractorPtr extractorPtr);
	virtual bool RegisterAttributeInfo(const std::string& attributeId, const IAttributeStaticInfo* attributeInfoPtr);

private:
	const IComponentStaticInfo* m_baseComponentPtr;

	InterfaceExtractors m_interfaceExtractors;
	AttributeInfos m_attributeInfos;
};


}//namespace icomp


#endif // !icomp_CBaseComponentStaticInfo_included


