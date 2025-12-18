#include <iattr/CEnumAttributeMetaInfo.h>


// ACF includes
#include <istd/CChangeNotifier.h>


namespace iattr
{


CEnumAttributeMetaInfo::CEnumAttributeMetaInfo(const QByteArray& typeId)
:	BaseClass(typeId),
	m_isAnyValueAllowed(false)
{
}


void CEnumAttributeMetaInfo::SetOtherValueAllowed(bool state)
{
	m_isAnyValueAllowed = state;
}


const QString& CEnumAttributeMetaInfo::GetUnknownDescription() const
{
	return m_unknownDescription;
}


void CEnumAttributeMetaInfo::SetUnknownDescription(const QString& description)
{
	if (description != m_unknownDescription){
		istd::CChangeNotifier notifier(this);
		Q_UNUSED(notifier);

		m_unknownDescription = description;
	}
}


bool CEnumAttributeMetaInfo::InsertOption(const QString& description, const iser::IObject* valuePtr, bool releaseFlag)
{
	Q_ASSERT(valuePtr != NULL);

	istd::CChangeNotifier notifier(this);
	Q_UNUSED(notifier);

	m_enums.push_back(EnumInfo());

	EnumInfo& info = m_enums.back();

	info.description = description;
	info.attributePtr.SetPtr(valuePtr, releaseFlag);

	return true;
}


// reimplemented (iattr::IEnumAttributeMetaInfo)

bool CEnumAttributeMetaInfo::IsAnyValueAllowed() const
{
	return m_isAnyValueAllowed;
}


int CEnumAttributeMetaInfo::GetEnumsCount() const
{
	return int(m_enums.size());
}


QString CEnumAttributeMetaInfo::GetValueDescription(const iser::IObject& value) const
{
	for (		Enums::const_iterator iter = m_enums.cbegin();
				iter != m_enums.cend();
				++iter){
		const EnumInfo& info = *iter;
		Q_ASSERT(info.attributePtr.IsValid());

		if (info.attributePtr->IsEqual(value)){
			return info.description;
		}
	}

	return m_unknownDescription;
}


const iser::IObject& CEnumAttributeMetaInfo::GetEnum(int index) const
{
	Q_ASSERT(index >= 0);

	Enums::const_iterator iter = m_enums.cbegin();
	while (index-- != 0){
		++iter;
	}

	const iser::IObject* attributePtr = (*iter).attributePtr.GetPtr();
	Q_ASSERT(attributePtr != NULL);

	return *attributePtr;
}


} // namespace iattr


