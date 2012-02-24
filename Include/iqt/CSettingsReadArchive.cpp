#include "iqt/CSettingsReadArchive.h"


// Qt includes
#include <QStringList>


// ACF includes
#include "iqt/iqt.h"


namespace iqt
{


// public methods

CSettingsReadArchive::CSettingsReadArchive(
			const QString& organizationName,
			const QString& applicationName,
			const QString& rootKey)
:	BaseClass2(organizationName, applicationName, rootKey)
{
	SerializeAcfHeader();
}


// reimplemented (iser::IArchive)

bool CSettingsReadArchive::BeginTag(const iser::CArchiveTag& tag)
{
	return EnterTag(tag.GetId());
}


bool CSettingsReadArchive::BeginMultiTag(const iser::CArchiveTag& tag, const iser::CArchiveTag& /*subTag*/, int& count)
{
	QString countRegistryKey = GetCurrentCountKey();

	bool isOk = true;
	count = BaseClass2::value(countRegistryKey).toInt(&isOk);

	if (!isOk){
		return false;
	}

	return EnterTag(tag.GetId());
}


bool CSettingsReadArchive::EndTag(const iser::CArchiveTag& tag)
{
	return LeaveTag(tag.GetId());
}


bool CSettingsReadArchive::Process(std::string& value)
{
	QString registryKey = CreateNextValueKey();

	if (registryKey.isEmpty()){
		return false;
	}

	value = BaseClass2::value(registryKey).toString().toStdString();

	return true;
}


bool CSettingsReadArchive::Process(QString& value)
{
	QString registryKey = CreateNextValueKey();

	if (registryKey.isEmpty()){
		return false;
	}

	value = BaseClass2::value(registryKey).toString();

	return true;
}


} // namespace iqt

