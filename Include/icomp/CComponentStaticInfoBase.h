#ifndef icomp_CComponentStaticInfoBase_included
#define icomp_CComponentStaticInfoBase_included


#include "icomp/IComponentStaticInfo.h"


namespace icomp
{


class CComponentStaticInfoBase: virtual public IComponentStaticInfo
{
public:
	typedef istd::TCascadedMap< std::string, const IComponentStaticInfo*> SubcomponentInfos;

	const SubcomponentInfos& GetSubcomponentInfos() const;

	// reimplemented (icomp::IComponentStaticInfo)
	virtual Ids GetSubcomponentIds() const;
	virtual const IComponentStaticInfo* GetSubcomponentInfo(const std::string& subcomponentId) const;
	virtual bool RegisterSubcomponentInfo(const std::string& subcomponentId, const IComponentStaticInfo* componentInfoPtr);
	virtual istd::CString GetDescription() const;
	virtual istd::CString GetKeywords() const;

protected:
	void ResetSubcomponentList();

private:
	SubcomponentInfos m_subcomponentInfos;
};


// inline methods

inline const CComponentStaticInfoBase::SubcomponentInfos& CComponentStaticInfoBase::GetSubcomponentInfos() const
{
	return m_subcomponentInfos;
}


inline istd::CString CComponentStaticInfoBase::GetDescription() const
{
	return "";
}


inline istd::CString CComponentStaticInfoBase::GetKeywords() const
{
	return "";
}


}//namespace icomp


#endif // !icomp_CComponentStaticInfoBase_included


