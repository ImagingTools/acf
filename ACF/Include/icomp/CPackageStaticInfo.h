#ifndef icomp_CPackageStaticInfo_included
#define icomp_CPackageStaticInfo_included


#include "icomp/IComponentStaticInfo.h"


namespace icomp
{


/**
	Static info for component package.
	Please note, that in composed component concept there is no distinguish between package and component.
*/
class CPackageStaticInfo: virtual public IComponentStaticInfo
{
public:
	// reimplemented (icomp::CPackageStaticInfo)
	virtual IComponent* CreateComponent(const IComponentContext* contextPtr) const;
	virtual const InterfaceExtractors& GetInterfaceExtractors() const;
	virtual const AttributeInfos& GetAttributeInfos() const;
	virtual const SubcomponentInfos& GetSubcomponentInfos() const;
	virtual bool RegisterInterfaceExtractor(const std::string& interfaceId, InterfaceExtractorPtr extractorPtr);
	virtual bool RegisterAttributeInfo(const std::string& attributeId, const IAttributeStaticInfo* attributeInfoPtr);
	virtual bool RegisterSubcomponentInfo(const std::string& subcomponentId, const IComponentStaticInfo* componentInfoPtr);

protected:
	void ResetSubcomponentList();

private:
	SubcomponentInfos m_subcomponentInfos;
};


}//namespace icomp


#endif // !icomp_CPackageStaticInfo_included


