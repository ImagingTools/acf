#include "icmpstr/CRegistryLoaderComp.h"


// Qt includes
#include <QString>
#include <QFileInfo>
#include <QDir>
#include <QMessageBox>


// ACF includes
#include "istd/TChangeNotifier.h"

#include "icmpstr/CVisualRegistryComp.h"


namespace icmpstr
{


// reimplemented (iser::IFileLoader)

int CRegistryLoaderComp::LoadFromFile(istd::IChangeable& data, const istd::CString& filePath) const
{
	istd::CChangeNotifier notifier(&data);

	if (!IsOperationSupported(&data, &filePath, QF_NO_SAVING, false)){
		return StateFailed;
	}

	CVisualRegistryComp* geometricalRegistryPtr = dynamic_cast<CVisualRegistryComp*>(&data);
	if (geometricalRegistryPtr != NULL){
		ReadArchiveEx registryArchive(filePath, this);
		I_ASSERT(!registryArchive.IsStoring());

		if (!geometricalRegistryPtr->SerializeRegistry(registryArchive)){
			OnReadError(registryArchive, data, filePath);

			return StateFailed;
		}

		ReadArchiveEx layoutArchive(GetLayoutPath(filePath), this);
		I_ASSERT(!layoutArchive.IsStoring());

		if (!geometricalRegistryPtr->SerializeComponentsLayout(layoutArchive)){
			SendInfoMessage(
						MI_CANNOT_READ_LAYOUT,
						iqt::GetCString(tr("Layout information cannot be loaded (%1)").
									arg(iqt::GetQString(filePath))));
		}

		return StateOk;
	}
	else{
		return BaseClass::LoadFromFile(data, filePath);
	}

	return StateFailed;
}


int CRegistryLoaderComp::SaveToFile(const istd::IChangeable& data, const istd::CString& filePath) const
{
	if (!IsOperationSupported(&data, &filePath, QF_NO_LOADING, false)){
		return StateFailed;
	}

	const CVisualRegistryComp* geometricalRegistryPtr = dynamic_cast<const CVisualRegistryComp*>(&data);
	if (geometricalRegistryPtr != NULL){
		WriteArchiveEx registryArchive(filePath, GetVersionInfo(), this);
		I_ASSERT(registryArchive.IsStoring());

		if (!const_cast<CVisualRegistryComp*>(geometricalRegistryPtr)->SerializeRegistry(registryArchive)){
			SendErrorMessage(
						MI_LOAD_ERROR,
						iqt::GetCString(tr("Cannot store to file %1").arg(iqt::GetQString(filePath))));

			return StateFailed;
		}

		WriteArchiveEx layoutArchive(GetLayoutPath(filePath), GetVersionInfo(), this);
		I_ASSERT(layoutArchive.IsStoring());

		if (!const_cast<CVisualRegistryComp*>(geometricalRegistryPtr)->SerializeComponentsLayout(layoutArchive)){
			SendInfoMessage(
						MI_CANNOT_READ_LAYOUT,
						iqt::GetCString(tr("Layout information cannot be stored (%1)").
									arg(iqt::GetQString(filePath))));
		}

		return StateOk;
	}

	return StateFailed;
}


// reimplemented (iser::IFileTypeInfo)

bool CRegistryLoaderComp::GetFileExtensions(istd::CStringList& result, int flags, bool doAppend) const
{
	if (!BaseClass::GetFileExtensions(result, flags, doAppend)){
		if (!doAppend){
			result.clear();
		}

		result.push_back("arx");
	}

	return true;
}


istd::CString CRegistryLoaderComp::GetTypeDescription(const istd::CString* extensionPtr) const
{
	if ((extensionPtr == NULL) || extensionPtr->IsEqualNoCase("arx")){
		return iqt::GetCString(tr("ACF registry file"));
	}

	return "";
}


// protected methods

istd::CString CRegistryLoaderComp::GetLayoutPath(const istd::CString& registryPath) const
{
	QFileInfo fileInfo(iqt::GetQString(registryPath));
	QString layoutPath = fileInfo.dir().absoluteFilePath(fileInfo.completeBaseName() + ".alx");

	return iqt::GetCString(layoutPath);
}


// reimplemented (ibase::TFileSerializerComp)

void CRegistryLoaderComp::OnReadError(
			const iser::CXmlFileReadArchive& archive,
			const istd::IChangeable& /*data*/,
			const istd::CString& filePath) const
{
	int lastReadLine = archive.GetLastReadLine();

	QString message = tr("%1(%2) : Cannot load file").arg(iqt::GetQString(filePath)).arg(lastReadLine);

	SendErrorMessage(MI_CANNOT_LOAD, iqt::GetCString(message));
}


} // namespace icmpstr


