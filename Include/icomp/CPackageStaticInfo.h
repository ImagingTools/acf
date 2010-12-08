#ifndef icomp_CPackageStaticInfo_included
#define icomp_CPackageStaticInfo_included


#include "iser/IArchive.h"

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
				const istd::CString& description,
				const istd::CString& keywords);

	void Reset();

	virtual bool SerializeMeta(iser::IArchive& archive);

	/**
		Register embedded component static info.
		\param	embeddedId			ID of this embedded component.
		\param	componentInfoPtr	embedded component static info object used to describe its type.
									It cannot be NULL.
	*/
	virtual void RegisterEmbeddedComponentInfo(const std::string& embeddedId, const IComponentStaticInfo* componentInfoPtr);

	// reimplemented (icomp::IComponentStaticInfo)
	virtual const IComponentStaticInfo* GetEmbeddedComponentInfo(const std::string& embeddedId) const;
	virtual Ids GetMetaIds(int metaGroupId) const;
	virtual const istd::CString& GetDescription() const;
	virtual const istd::CString& GetKeywords() const;

private:
	typedef std::map<std::string, const IComponentStaticInfo*> EmbeddedComponentInfos;
	EmbeddedComponentInfos m_embeddedComponentInfos;

	istd::CString m_description;
	istd::CString m_keywords;
};


}//namespace icomp


#endif // !icomp_CPackageStaticInfo_included


