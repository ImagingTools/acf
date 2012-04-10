#include "icomp/CComponentStaticInfoBase.h"


namespace icomp
{


// reimplemented (icomp::IComponentStaticInfo)

int CComponentStaticInfoBase::GetComponentType() const
{
	return CT_NONE;
}


const IAttributeStaticInfo* CComponentStaticInfoBase::GetAttributeInfo(const QByteArray& /*attributeId*/) const
{
	return NULL;
}


const IComponentStaticInfo* CComponentStaticInfoBase::GetEmbeddedComponentInfo(const QByteArray& /*embeddedId*/) const
{
	return NULL;
}


//	reimplemented (icomp::IElementStaticInfo)

const IElementStaticInfo* CComponentStaticInfoBase::GetSubelementInfo(const QByteArray& /*subcomponentId*/) const
{
	return NULL;
}


IElementStaticInfo::Ids CComponentStaticInfoBase::GetMetaIds(int /*metaGroupId*/) const
{
	Ids retVal;

	return retVal;
}


} // namespace icomp


