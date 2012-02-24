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
	virtual const QString& GetDescription() const;
	virtual const QString& GetKeywords() const;

	//	reimplemented (icomp::IElementStaticInfo)
	virtual const IElementStaticInfo* GetSubelementInfo(const std::string& subcomponentId) const;
	virtual Ids GetMetaIds(int metaGroupId) const;
};


// inline methods

// reimplemented (icomp::IComponentStaticInfo)

inline const QString& CComponentStaticInfoBase::GetDescription() const
{
	static QString emptyString;

	return emptyString;
}


inline const QString& CComponentStaticInfoBase::GetKeywords() const
{
	static QString emptyString;

	return emptyString;
}


} // namespace icomp


#endif // !icomp_CComponentStaticInfoBase_included


