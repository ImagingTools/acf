#include "ibase/CQtVersionInfoComp.h"


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
	int major = qtVersionString.mid(0, 1).toInt();
	int minor = qtVersionString.mid(2, 1).toInt();
	int fix = qtVersionString.mid(4, 1).toInt();
			
	return (major << 16) | (minor << 8) | fix;
}


} // namespace ibase
