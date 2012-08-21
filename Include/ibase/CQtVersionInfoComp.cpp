#include "ibase/CQtVersionInfoComp.h"


namespace ibase
{


// reimplemented (iser::IVersionInfo)

iser::IVersionInfo::VersionIds CQtVersionInfoComp::GetVersionIds() const
{
	IVersionInfo::VersionIds ids = BaseClass::GetVersionIds();

		ids << QVI_COMPILED << QVI_RUNTIME;

	return ids;
}


bool CQtVersionInfoComp::GetVersionNumber(int versionId, quint32& result) const
{
	switch (versionId){
		case QVI_COMPILED:
			result = QT_VERSION;
			return true;

		case QVI_RUNTIME:
			QString qtVersionString = qVersion();
			int major = qtVersionString.mid(0, 1).toInt();
			int minor = qtVersionString.mid(2, 1).toInt();
			int fix = qtVersionString.mid(4, 1).toInt();
			
			result = (major << 16) | (minor << 8) | fix;

			return true;
	}

	return BaseClass::GetVersionNumber(versionId, result);
}


QString CQtVersionInfoComp::GetVersionIdDescription(int versionId) const
{
	switch (versionId){
		case QVI_COMPILED:
			return "Qt Compiled Version";

		case QVI_RUNTIME:
			return "Qt Runtime Version";
	}

	return BaseClass::GetVersionIdDescription(versionId);
}


QString CQtVersionInfoComp::GetEncodedVersionName(int versionId, quint32 versionNumber) const
{
	switch (versionId){
		case QVI_COMPILED:
			return QT_VERSION_STR;

		case QVI_RUNTIME:
			return qVersion();
	}

	return BaseClass::GetEncodedVersionName(versionId, versionNumber);
}


} // namespace ibase
