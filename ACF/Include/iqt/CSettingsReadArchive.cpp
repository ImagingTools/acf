#include "iqt/CSettingsReadArchive.h"

#include <QStringList>


#include "iqt/iqt.h"


namespace iqt
{


// public methods

CSettingsReadArchive::CSettingsReadArchive(	const QString& organizationName,
											const QString& applicationName)
	:BaseClass2(organizationName, applicationName)
{
}


// reimplemented (iser::IArchive)

bool CSettingsReadArchive::BeginTag(const iser::CArchiveTag& tag)
{
	m_openTagsList.push_back(TagInfo(tag.GetId(), 0));

	return true;
}


bool CSettingsReadArchive::BeginMultiTag(const iser::CArchiveTag& tag, const iser::CArchiveTag& /*subTag*/, int& count)
{
	m_openTagsList.push_back(TagInfo(tag.GetId(), count));

	QString registryKey = CreateKey(false);

	count = BaseClass2::value(registryKey).toInt();

	m_openTagsList.back().count = count;

	return true;
}


bool CSettingsReadArchive::EndTag(const iser::CArchiveTag& /*tag*/)
{
	I_ASSERT(!m_openTagsList.empty());

	TagInfo& currentTag = m_openTagsList.back();
	
	if (currentTag.count == 0){		
		m_openTagsList.pop_back();
	}
	
	return true;
}


bool CSettingsReadArchive::Process(std::string& value)
{
	if (m_openTagsList.empty()){
		return false;
	}

	QString registryKey = CreateKey();

	value = BaseClass2::value(registryKey).toString().toStdString();

	return true;
}


bool CSettingsReadArchive::Process(istd::CString& value)
{
	std::string str(value.ToString());

	if (Process(str)){
		value = str;

		return true;
	}

	return false;
}


} // namespace iqt

