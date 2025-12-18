#include <ibase/CQtVersionInfoComp.h>


namespace ibase
{


// reimplemented (iser::IVersionInfo)

iser::IVersionInfo::VersionIds CQtVersionInfoComp::GetVersionIds() const
{
	IVersionInfo::VersionIds ids = BaseClass::GetVersionIds();

	ids << VI_QT_VERSION;

	return ids;
}


bool CQtVersionInfoComp::GetVersionNumber(int versionId, quint32& result) const
{
	switch (versionId){
		case VI_QT_VERSION:
			result = QT_VERSION;
			return true;
	}

	return BaseClass::GetVersionNumber(versionId, result);
}


QString CQtVersionInfoComp::GetVersionIdDescription(int versionId) const
{
	switch (versionId){
		case VI_QT_VERSION:
			return "Qt Framework";
	}

	return BaseClass::GetVersionIdDescription(versionId);
}


QString CQtVersionInfoComp::GetEncodedVersionName(int versionId, quint32 versionNumber) const
{
	switch (versionId){
		case VI_QT_VERSION:
			return QT_VERSION_STR;
	}

	return BaseClass::GetEncodedVersionName(versionId, versionNumber);
}


// protected methods

// reimplemented (icomp::CComponentBase)

void CQtVersionInfoComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (QT_VERSION != GetRuntimeVersion()){
		SendWarningMessage(0, QObject::tr("The runtime version of Qt framework doesn't match the version used by creation of ACF"));
	}
}



// private static methods

quint32 CQtVersionInfoComp::GetRuntimeVersion()
{
	QString qtVersionString = qVersion();
	
	int majorVersionSeparatorIndex = qtVersionString.indexOf(".");
	int minorVersionSeparatorIndex = qtVersionString.indexOf(".", majorVersionSeparatorIndex + 1);

	QString majorVersionString = qtVersionString.mid(0, majorVersionSeparatorIndex);
	QString minorVersionString = qtVersionString.mid(majorVersionSeparatorIndex + 1, minorVersionSeparatorIndex - (majorVersionString.length() + 1));
	QString fixVersionString = qtVersionString.right(qtVersionString.length() - minorVersionSeparatorIndex - 1);

	int major = majorVersionString.toInt();
	int minor = minorVersionString.toInt();
	int fix = fixVersionString.toInt();

	return (major << 16) | (minor << 8) | fix;
}


} // namespace ibase


