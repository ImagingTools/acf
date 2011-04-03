#include "iqt/CSettingsSerializerComp.h"


// ACF includes
#include "iqt/CSettingsReadArchive.h"
#include "iqt/CSettingsWriteArchive.h"


namespace iqt
{


// reimplemented (iser::IFileLoader)

bool CSettingsSerializerComp::IsOperationSupported(
			const istd::IChangeable* dataObjectPtr,
			const istd::CString* /*filePathPtr*/,
			int flags,
			bool beQuiet) const
{
	if ((dataObjectPtr != NULL) && (dynamic_cast<const iser::ISerializable*>(dataObjectPtr) == NULL)){
		if (!beQuiet){
			SendInfoMessage(MI_BAD_OBJECT_TYPE, "Object is not serializable");
		}

		return false;
	}

	return ((flags & QF_NAMED_ONLY) == 0);
}


int CSettingsSerializerComp::LoadFromFile(istd::IChangeable& data, const istd::CString& /*filePath*/) const
{
	if (IsOperationSupported(&data, NULL, QF_NO_SAVING, false)){
		iser::ISerializable* serializeblePtr = dynamic_cast<iser::ISerializable*>(&data);
		if (m_applicationInfoCompPtr.IsValid() && (serializeblePtr != NULL)){ 
			istd::CString applicationName = m_applicationInfoCompPtr->GetApplicationAttribute(ibase::IApplicationInfo::AA_APPLICATION_NAME);
			istd::CString companyName = m_applicationInfoCompPtr->GetApplicationAttribute(ibase::IApplicationInfo::AA_COMPANY_NAME);

			CSettingsReadArchive archive(iqt::GetQString(companyName), iqt::GetQString(applicationName));

			std::string rootKey;
			if (m_rootKeyAttrPtr.IsValid()){
				rootKey = (*m_rootKeyAttrPtr).ToString();
			}

			iser::CArchiveTag rootArchiveTag(rootKey, "Root key");

			bool retVal = true;

			if (!rootKey.empty()){
				retVal = retVal && archive.BeginTag(rootArchiveTag);
			}

			retVal = retVal && serializeblePtr->Serialize(archive);

			if (!rootKey.empty()){
				retVal = retVal && archive.EndTag(rootArchiveTag);
			}

			if (retVal){
				return StateOk;
			}
			else{
				SendInfoMessage(MI_CANNOT_LOAD, "Cannot serialize object from file");
			}
		}
		else{
			SendInfoMessage(MI_CANNOT_LOAD, "No application info needed to load from settings");
		}
	}

	return StateFailed;
}


int CSettingsSerializerComp::SaveToFile(const istd::IChangeable& data, const istd::CString& /*filePath*/) const
{
	if (IsOperationSupported(&data, NULL, QF_NO_LOADING, false)){
		iser::ISerializable* serializeblePtr = dynamic_cast<iser::ISerializable*>(const_cast<istd::IChangeable*>(&data));
		if (m_applicationInfoCompPtr.IsValid() && (serializeblePtr != NULL)){ 
			istd::CString applicationName = m_applicationInfoCompPtr->GetApplicationAttribute(ibase::IApplicationInfo::AA_APPLICATION_NAME);
			istd::CString companyName = m_applicationInfoCompPtr->GetApplicationAttribute(ibase::IApplicationInfo::AA_COMPANY_NAME);

			CSettingsWriteArchive archive(iqt::GetQString(companyName), iqt::GetQString(applicationName));
		
			std::string rootKey;
			if (m_rootKeyAttrPtr.IsValid()){
				rootKey = (*m_rootKeyAttrPtr).ToString();
			}

			iser::CArchiveTag rootArchiveTag(rootKey, "Root key");

			bool retVal = true;

			if (!rootKey.empty()){
				retVal = retVal && archive.BeginTag(rootArchiveTag);
			}

			retVal = retVal && serializeblePtr->Serialize(archive);

			if (!rootKey.empty()){
				retVal = retVal && archive.EndTag(rootArchiveTag);
			}

			if (retVal){
				return StateOk;
			}
			else{
				SendInfoMessage(MI_CANNOT_SAVE, "Cannot serialize object to file");
			}
		}
		else{
			SendInfoMessage(MI_CANNOT_LOAD, "No application info needed to save to settings");
		}
	}

	return StateFailed;
}


// reimplemented (iser::IFileTypeInfo)

bool CSettingsSerializerComp::GetFileExtensions(istd::CStringList& /*result*/, int /*flags*/, bool /*doAppend*/) const
{
	return false;
}


istd::CString CSettingsSerializerComp::GetTypeDescription(const istd::CString* /*extensionPtr*/) const
{
	return "";
}


} // namespace iqt


