#include "icomp/CComponentStaticInfoBase.h"


namespace icomp
{


// reimplemented (icomp::IComponentStaticInfo)

int CComponentStaticInfoBase::GetComponentType() const
{
	return CT_NONE;
}


const IAttributeStaticInfo* CComponentStaticInfoBase::GetAttributeInfo(const std::string& /*attributeId*/) const
{
	return NULL;
}


const IComponentStaticInfo* CComponentStaticInfoBase::GetEmbeddedComponentInfo(const std::string& /*embeddedId*/) const
{
	return NULL;
}


//	reimplemented (icomp::IElementStaticInfo)

const IElementStaticInfo* CComponentStaticInfoBase::GetSubelementInfo(const std::string& /*subcomponentId*/) const
{
	return NULL;
}


IElementStaticInfo::Ids CComponentStaticInfoBase::GetMetaIds(int /*metaGroupId*/) const
{
	Ids retVal;

	return retVal;
}


} // namespace icomp


