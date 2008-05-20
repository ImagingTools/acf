#ifndef icomp_CPackageStaticInfo_included
#define icomp_CPackageStaticInfo_included


#include "icomp/CComponentStaticInfoBase.h"


namespace icomp
{


/**
	Static info for component package.
	Please note, that in composed component concept there is no distinguish between package and component.
*/
class CPackageStaticInfo: virtual public CComponentStaticInfoBase
{
public:
	// reimplemented (icomp::IComponentStaticInfo)
	virtual IComponent* CreateComponent(const IComponentContext* contextPtr) const;
	virtual const InterfaceExtractors& GetInterfaceExtractors() const;
	virtual const AttributeInfos& GetAttributeInfos() const;
	virtual bool RegisterInterfaceExtractor(const std::string& interfaceId, InterfaceExtractorPtr extractorPtr);
	virtual bool RegisterAttributeInfo(const std::string& attributeId, const IAttributeStaticInfo* attributeInfoPtr);
};


}//namespace icomp


#endif // !icomp_CPackageStaticInfo_included


