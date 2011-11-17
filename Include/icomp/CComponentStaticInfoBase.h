#ifndef icomp_CComponentStaticInfoBase_included
#define icomp_CComponentStaticInfoBase_included


// STL incldues
#include <map>
#include <string>


// ACF includes
#include "icomp/IComponentStaticInfo.h"


namespace icomp
{


class CComponentStaticInfoBase: virtual public IComponentStaticInfo
{
public:
	// reimplemented (icomp::IComponentStaticInfo)
	virtual int GetComponentType() const;
	virtual const IAttributeStaticInfo* GetAttributeInfo(const std::string& attributeId) const;
	virtual const IComponentStaticInfo* GetEmbeddedComponentInfo(const std::string& embeddedId) const;
	virtual const istd::CString& GetDescription() const;
	virtual const istd::CString& GetKeywords() const;

	//	reimplemented (icomp::IElementStaticInfo)
	virtual const IElementStaticInfo* GetSubelementInfo(const std::string& subcomponentId) const;
	virtual Ids GetMetaIds(int metaGroupId) const;
};


// inline methods

// reimplemented (icomp::IComponentStaticInfo)

inline const istd::CString& CComponentStaticInfoBase::GetDescription() const
{
	return istd::CString::GetEmpty();
}


inline const istd::CString& CComponentStaticInfoBase::GetKeywords() const
{
	return istd::CString::GetEmpty();
}


} // namespace icomp


#endif // !icomp_CComponentStaticInfoBase_included


