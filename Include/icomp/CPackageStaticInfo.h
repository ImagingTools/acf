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
	CPackageStaticInfo();
	CPackageStaticInfo(
				const std::string& logicalName,
				const istd::CString& description,
				const istd::CString& keywords);

	// reimplemented (icomp::IComponentStaticInfo)
	virtual const InterfaceExtractors& GetInterfaceExtractors() const;
	virtual const AttributeInfos& GetAttributeInfos() const;
	virtual bool RegisterInterfaceExtractor(const istd::CClassInfo& interfaceId, InterfaceExtractorPtr extractorPtr);
	virtual bool RegisterAttributeInfo(const std::string& attributeId, const IAttributeStaticInfo* attributeInfoPtr);
	virtual const istd::CString& GetDescription() const;
	virtual const istd::CString& GetKeywords() const;

private:
	istd::CString m_description;
	istd::CString m_keywords;
};


}//namespace icomp


#endif // !icomp_CPackageStaticInfo_included


