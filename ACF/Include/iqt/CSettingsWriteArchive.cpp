#include "iqt/CSettingsWriteArchive.h"


#include "iqt/iqt.h"


namespace iqt
{


// public methods

CSettingsWriteArchive::CSettingsWriteArchive(const QString& organizationName, 
											 const QString& productName,
											 const iser::IVersionInfo* versionInfoPtr)
	:BaseClass(versionInfoPtr),
	BaseClass2(organizationName, productName)
{
}


// reimplemented (iser::IArchive)

bool CSettingsWriteArchive::BeginTag(const iser::CArchiveTag& tag)
{
	m_openTagsList.push_back(TagInfo(tag.GetId(), 0));

	return true;
}


bool CSettingsWriteArchive::BeginMultiTag(const iser::CArchiveTag& tag, const iser::CArchiveTag& /*subTag*/, int& count)
{
	m_openTagsList.push_back(TagInfo(tag.GetId(), count));

	QString registryKey = CreateKey(false);

	BaseClass2::setValue(registryKey, count);

	return true;
}


bool CSettingsWriteArchive::EndTag(const iser::CArchiveTag& /*tag*/)
{
	I_ASSERT(!m_openTagsList.empty());

	TagInfo& currentTag = m_openTagsList.back();
	if (currentTag.count == 0){		
		m_openTagsList.pop_back();
	}

	return true;
}


bool CSettingsWriteArchive::Process(std::string& value)
{
	if (m_openTagsList.empty()){
		return false;
	}

	QString registryKey = CreateKey();

	BaseClass2::setValue(CreateKey(), QString(value.c_str()));

	return true;
}


bool CSettingsWriteArchive::Process(istd::CString& value)
{
	std::string str(value.ToString());

	return Process(str);
}


} // namespace iqt

