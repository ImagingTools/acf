#include <iqt/CSettingsReadArchive.h>


// Qt includes
#include <QtCore/QStringList>



namespace iqt
{


// public methods

CSettingsReadArchive::CSettingsReadArchive(
			const QString& organizationName,
			const QString& applicationName,
			const QString& rootKey,
			QSettings::Scope settingsScope)
:	BaseClass2(organizationName, applicationName, rootKey, settingsScope)
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
	if (EnterTag(tag.GetId())){
		QString countRegistryKey = GetCurrentCountKey();

		bool isOk = true;
		count = BaseClass2::value(countRegistryKey).toInt(&isOk);

		if (isOk){
			return true;
		}
	}

	return false;
}


bool CSettingsReadArchive::EndTag(const iser::CArchiveTag& tag)
{
	return LeaveTag(tag.GetId());
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


// protected methods

bool CSettingsReadArchive::ReadStringNode(QString& text)
{
	QString registryKey = CreateNextValueKey();

	if (registryKey.isEmpty()){
		return false;
	}

	text = BaseClass2::value(registryKey).toString();

	return true;
}


// reimplemented (iser::CTextReadArchiveBase)

bool CSettingsReadArchive::ReadTextNode(QByteArray& text)
{
	QString stringText;

	if (ReadStringNode(stringText)){
		text = stringText.toLocal8Bit();

		return true;
	}

	return false;
}


} // namespace iqt

