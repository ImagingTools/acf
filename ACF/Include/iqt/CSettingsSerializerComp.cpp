#include "iqt/CSettingsSerializerComp.h"


#include "iqt/CSettingsReadArchive.h"
#include "iqt/CSettingsWriteArchive.h"


namespace iqt
{


// reimplemented (iser::IFileLoader)

bool CSettingsSerializerComp::IsOperationSupported(
			const istd::IChangeable* dataObjectPtr,
			const istd::CString* /*filePathPtr*/,
			bool /*forLoading*/,
			bool /*forSaving*/) const
{
	if (dataObjectPtr != NULL){
		return dynamic_cast<const iser::ISerializable*>(dataObjectPtr) != NULL;
	}

	return true;
}


int CSettingsSerializerComp::LoadFromFile(istd::IChangeable& data, const istd::CString& /*filePath*/) const
{
	iser::ISerializable* serializeblePtr = dynamic_cast<iser::ISerializable*>(&data);
	if (m_applicationInfoCompPtr.IsValid() && (serializeblePtr != NULL)){ 
		istd::CString applicationName = m_applicationInfoCompPtr->GetApplicationName();
		istd::CString companyName = m_applicationInfoCompPtr->GetCompanyName();

		CSettingsReadArchive archive(iqt::GetQString(companyName), iqt::GetQString(applicationName));

		if (serializeblePtr->Serialize(archive)){
			return StateOk;
		}
	}

	return StateFailed;
}


int CSettingsSerializerComp::SaveToFile(const istd::IChangeable& data, const istd::CString& /*filePath*/) const
{
	iser::ISerializable* serializeblePtr = dynamic_cast<iser::ISerializable*>(const_cast<istd::IChangeable*>(&data));
	if (m_applicationInfoCompPtr.IsValid() && (serializeblePtr != NULL)){ 
		istd::CString applicationName = m_applicationInfoCompPtr->GetApplicationName();
		istd::CString companyName = m_applicationInfoCompPtr->GetCompanyName();

		CSettingsWriteArchive archive(iqt::GetQString(companyName), iqt::GetQString(applicationName));

		if (serializeblePtr->Serialize(archive)){
			return StateOk;
		}
	}

	return StateFailed;
}


const istd::CString& CSettingsSerializerComp::GetLastLoadFileName() const
{
	static istd::CString emptyName;

	return emptyName;
}


const istd::CString& CSettingsSerializerComp::GetLastSaveFileName() const
{
	return CSettingsSerializerComp::GetLastLoadFileName();
}
	  

} // namespace iqt


