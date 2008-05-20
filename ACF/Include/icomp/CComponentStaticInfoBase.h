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
	virtual const IComponentStaticInfo* GetSubcomponent(const std::string& subcomponentId) const;
	virtual bool RegisterSubcomponentInfo(const std::string& subcomponentId, const IComponentStaticInfo* componentInfoPtr);

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


}//namespace icomp


#endif // !icomp_CComponentStaticInfoBase_included


