#include "iqt/CSettingsSerializerComp.h"


// ACF includes
#include "istd/CSystem.h"
#include "iqt/CSettingsReadArchive.h"
#include "iqt/CSettingsWriteArchive.h"


namespace iqt
{


// reimplemented (ifile::IFilePersistence)

bool CSettingsSerializerComp::IsOperationSupported(
			const istd::IChangeable* dataObjectPtr,
			const QString* /*filePathPtr*/,
			int flags,
			bool beQuiet) const
{
	if ((dataObjectPtr != NULL) && (dynamic_cast<const iser::ISerializable*>(dataObjectPtr) == NULL)){
		if (!beQuiet){
			SendInfoMessage(MI_BAD_OBJECT_TYPE, "Object is not serializable");
		}

		return false;
	}

	if ((flags & (QF_LOAD | QF_SAVE)) == 0){
		return false;
	}

	return ((flags & QF_ANONYMOUS) != 0);
}


int CSettingsSerializerComp::LoadFromFile(
			istd::IChangeable& data,
			const QString& /*filePath*/,
			ibase::IProgressManager* /*progressManagerPtr*/) const
{
	if (IsOperationSupported(&data, NULL, QF_LOAD | QF_ANONYMOUS, false)){
		iser::ISerializable* serializeblePtr = dynamic_cast<iser::ISerializable*>(&data);
		if (m_applicationInfoCompPtr.IsValid() && (serializeblePtr != NULL)){
			QString applicationName = m_applicationInfoCompPtr->GetApplicationAttribute(ibase::IApplicationInfo::AA_APPLICATION_NAME, false);
			QString companyName = m_applicationInfoCompPtr->GetApplicationAttribute(ibase::IApplicationInfo::AA_COMPANY_NAME, false);

			QSettings::Scope scope = m_scopeAttrPtr.IsValid() ? QSettings::Scope(*m_scopeAttrPtr) : QSettings::UserScope;

			CSettingsReadArchive archive(
						companyName,
						applicationName,
						istd::CSystem::GetEnrolledPath(*m_rootKeyAttrPtr, false),
						scope);

			if (serializeblePtr->Serialize(archive)){
				return OS_OK;
			}
			else{
				SendInfoMessage(MI_CANNOT_LOAD, "Cannot serialize object from file");
			}
		}
		else{
			SendInfoMessage(MI_CANNOT_LOAD, "No application info found, it is needed to save to settings");
		}
	}

	return OS_FAILED;
}


int CSettingsSerializerComp::SaveToFile(
			const istd::IChangeable& data,
			const QString& /*filePath*/,
			ibase::IProgressManager* /*progressManagerPtr*/) const
{
	if (IsOperationSupported(&data, NULL, QF_SAVE | QF_ANONYMOUS, false)){
		iser::ISerializable* serializeblePtr = dynamic_cast<iser::ISerializable*>(const_cast<istd::IChangeable*>(&data));
		if (m_applicationInfoCompPtr.IsValid() && (serializeblePtr != NULL)){
			QString companyName = m_applicationInfoCompPtr->GetApplicationAttribute(ibase::IApplicationInfo::AA_COMPANY_NAME, false);
			QString applicationName = m_applicationInfoCompPtr->GetApplicationAttribute(ibase::IApplicationInfo::AA_APPLICATION_NAME, false);

			QSettings::Scope scope = m_scopeAttrPtr.IsValid() ? QSettings::Scope(*m_scopeAttrPtr) : QSettings::UserScope;

			CSettingsWriteArchive archive(
						companyName,
						applicationName,
						istd::CSystem::GetEnrolledPath(*m_rootKeyAttrPtr, false),
						scope,
						&m_applicationInfoCompPtr->GetVersionInfo());

			if (serializeblePtr->Serialize(archive)){
				return OS_OK;
			}
			else{
				SendInfoMessage(MI_CANNOT_SAVE, "Cannot serialize object to file");
			}
		}
		else{
			SendInfoMessage(MI_CANNOT_LOAD, "No application info found, it is needed to save to settings");
		}
	}

	return OS_FAILED;
}


// reimplemented (ifile::IFileTypeInfo)

bool CSettingsSerializerComp::GetFileExtensions(QStringList& /*result*/, int /*flags*/, bool /*doAppend*/) const
{
	return false;
}


QString CSettingsSerializerComp::GetTypeDescription(const QString* /*extensionPtr*/) const
{
	return "";
}


} // namespace iqt


