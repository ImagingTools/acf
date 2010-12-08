#ifndef icomp_CComponentStaticInfoBase_included
#define icomp_CComponentStaticInfoBase_included


// STL incldues
#include <map>
#include <string>

#include "icomp/IComponentStaticInfo.h"


namespace icomp
{


class CComponentStaticInfoBase: virtual public IComponentStaticInfo
{
public:
	// reimplemented (icomp::IComponentStaticInfo)
	virtual int GetComponentType() const;
	virtual const IAttributeStaticInfo* GetAttributeInfo(const std::string& attributeId) const;
	virtual const IComponentStaticInfo* GetSubcomponentInfo(const std::string& subcomponentId) const;
	virtual const IComponentStaticInfo* GetEmbeddedComponentInfo(const std::string& embeddedId) const;
	virtual Ids GetMetaIds(int metaGroupId) const;
	virtual const istd::CString& GetDescription() const;
	virtual const istd::CString& GetKeywords() const;
};


// inline methods

inline const istd::CString& CComponentStaticInfoBase::GetDescription() const
{
	return istd::CString::GetEmpty();
}


inline const istd::CString& CComponentStaticInfoBase::GetKeywords() const
{
	return istd::CString::GetEmpty();
}


}//namespace icomp


#endif // !icomp_CComponentStaticInfoBase_included


