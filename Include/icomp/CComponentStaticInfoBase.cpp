#include "icomp/CComponentStaticInfoBase.h"


namespace icomp
{


// reimplemented (icomp::IPackageStaticInfo)

IComponentStaticInfo::Ids CComponentStaticInfoBase::GetSubcomponentIds() const
{
	Ids retVal;

	m_subcomponentInfos.GetKeys(retVal);

	return retVal;
}


const IComponentStaticInfo* CComponentStaticInfoBase::GetSubcomponentInfo(const std::string& subcomponentId) const
{
	const SubcomponentInfos::ValueType* retVal = m_subcomponentInfos.FindElement(subcomponentId);
	if (retVal != NULL){
		return *retVal;
	}
	else{
		return NULL;
	}
}


bool CComponentStaticInfoBase::RegisterSubcomponentInfo(const std::string& subcomponentId, const IComponentStaticInfo* componentInfoPtr)
{
	return m_subcomponentInfos.InsertLocal(subcomponentId, componentInfoPtr);
}


// protected methods

void CComponentStaticInfoBase::ResetSubcomponentList()
{
	m_subcomponentInfos.ResetLocal();
}


}//namespace icomp


