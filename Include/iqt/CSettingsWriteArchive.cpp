#include "iqt/CSettingsWriteArchive.h"


// ACF includes
#include "iqt/iqt.h"


namespace iqt
{


// public methods

CSettingsWriteArchive::CSettingsWriteArchive(
			const QString& organizationName, 
			const QString& productName,
			const QString& rootKey,
			const iser::IVersionInfo* versionInfoPtr)
:	BaseClass(versionInfoPtr),
	BaseClass2(organizationName, productName, rootKey)
{
	SerializeAcfHeader();
}


// reimplemented (iser::IArchive)

bool CSettingsWriteArchive::BeginTag(const iser::CArchiveTag& tag)
{
	return EnterTag(tag.GetId());
}


bool CSettingsWriteArchive::BeginMultiTag(const iser::CArchiveTag& tag, const iser::CArchiveTag& /*subTag*/, int& count)
{
	QString countRegistryKey = GetCurrentCountKey();

	BaseClass2::setValue(countRegistryKey, count);

	return EnterTag(tag.GetId());
}


bool CSettingsWriteArchive::EndTag(const iser::CArchiveTag& tag)
{
	return LeaveTag(tag.GetId());
}


bool CSettingsWriteArchive::Process(QByteArray& value)
{
	QString registryKey = CreateNextValueKey();

	if (registryKey.isEmpty()){
		return false;
	}

	BaseClass2::setValue(registryKey, QString(value));

	return true;
}


bool CSettingsWriteArchive::Process(QString& value)
{
	QString registryKey = CreateNextValueKey();

	if (registryKey.isEmpty()){
		return false;
	}

	BaseClass2::setValue(registryKey, value);

	return true;
}


} // namespace iqt


