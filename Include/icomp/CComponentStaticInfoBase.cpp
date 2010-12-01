#include "icomp/CComponentStaticInfoBase.h"


namespace icomp
{


void CComponentStaticInfoBase::Reset()
{
	m_subcomponentInfos.ResetLocal();
}


bool CComponentStaticInfoBase::RegisterSubcomponentInfo(const std::string& subcomponentId, const IComponentStaticInfo* componentInfoPtr)
{
	return m_subcomponentInfos.InsertLocal(subcomponentId, componentInfoPtr);
}


// reimplemented (icomp::IComponentStaticInfo)

int CComponentStaticInfoBase::GetComponentType() const
{
	return CT_NONE;
}


IComponent* CComponentStaticInfoBase::CreateComponent() const
{
	I_CRITICAL();	// trying to create abstract base component. Check if I_BEGIN_COMPONENT in constructed component wasn't forgotten

	return NULL;
}


IComponentStaticInfo::Ids CComponentStaticInfoBase::GetMetaIds(int /*metaGroupId*/) const
{
	return Ids();
}


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


// protected methods

void CComponentStaticInfoBase::ResetSubcomponentList()
{
	m_subcomponentInfos.ResetLocal();
}


}//namespace icomp


