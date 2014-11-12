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


const QString& CComponentStaticInfoBase::GetDescription() const
{
	static QString emptyString;

	return emptyString;
}


const QString& CComponentStaticInfoBase::GetKeywords() const
{
	static QString emptyString;

	return emptyString;
}


//	reimplemented (icomp::IElementStaticInfo)

IElementStaticInfo::Ids CComponentStaticInfoBase::GetMetaIds(int /*metaGroupId*/) const
{
	Ids retVal;

	return retVal;
}


const IElementStaticInfo* CComponentStaticInfoBase::GetSubelementInfo(const QByteArray& /*subcomponentId*/) const
{
	return NULL;
}


//	reimplemented (iattr::IAttributesMetaInfoProvider)

iattr::IAttributesProvider::AttributeIds CComponentStaticInfoBase::GetAttributeMetaIds() const
{
	iattr::IAttributesProvider::AttributeIds retVal;

	return retVal;
}


const iattr::IAttributeMetaInfo* CComponentStaticInfoBase::GetAttributeMetaInfo(const QByteArray& attributeId) const
{
	return NULL;
}


} // namespace icomp


