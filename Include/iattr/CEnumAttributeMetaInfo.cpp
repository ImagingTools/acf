#include "iattr/CEnumAttributeMetaInfo.h"


namespace iattr
{


CEnumAttributeMetaInfo::CEnumAttributeMetaInfo()
:	m_isAnyValueAllowed(false)
{
}


void CEnumAttributeMetaInfo::SetOtherValueAllowed(bool state)
{
	m_isAnyValueAllowed = state;
}


bool CEnumAttributeMetaInfo::InsertOption(const QString& description, const iser::IObject* valuePtr, bool releaseFlag)
{
	Q_ASSERT(valuePtr != NULL);

	m_enums.append(EnumInfo());

	EnumInfo& info = m_enums.back();

	info.description = description;
	info.attributePtr.SetPtr(valuePtr, releaseFlag);

	return true;
}


// reimplemented (iattr::IPropertyEnumConstraints)

bool CEnumAttributeMetaInfo::IsAnyValueAllowed() const
{
	return m_isAnyValueAllowed;
}


int CEnumAttributeMetaInfo::GetEnumsCount() const
{
	return m_enums.count();
}


QString CEnumAttributeMetaInfo::GetEnumDescription(int index) const
{
	return m_enums[index].description;
}


const iser::IObject& CEnumAttributeMetaInfo::GetEnum(int index) const
{
	const iser::IObject* attributePtr = m_enums[index].attributePtr.GetPtr();
	Q_ASSERT(attributePtr != NULL);

	return *attributePtr;
}


} // namespace iattr


