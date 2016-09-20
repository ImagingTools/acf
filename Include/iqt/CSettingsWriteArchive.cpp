#include <iqt/CSettingsWriteArchive.h>


// ACF includes
#include <iqt/iqt.h>


namespace iqt
{


// public methods

CSettingsWriteArchive::CSettingsWriteArchive(
			const QString& organizationName,
			const QString& productName,
			const QString& rootKey,
			QSettings::Scope settingsScope,
			const iser::IVersionInfo* versionInfoPtr)
:	BaseClass(versionInfoPtr),
	BaseClass2(organizationName, productName, rootKey, settingsScope)
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
	if (EnterTag(tag.GetId())){
		QString countRegistryKey = GetCurrentCountKey();

		BaseClass2::setValue(countRegistryKey, count);

		return true;
	}

	return false;
}


bool CSettingsWriteArchive::EndTag(const iser::CArchiveTag& tag)
{
	return LeaveTag(tag.GetId());
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


// protected methods

// reimplemented (iser::CTextWriteArchiveBase)

bool CSettingsWriteArchive::WriteTextNode(const QByteArray& text)
{
	QString registryKey = CreateNextValueKey();

	if (registryKey.isEmpty()){
		return false;
	}

	BaseClass2::setValue(registryKey, text);

	return true;
}


} // namespace iqt


