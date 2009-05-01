#include "icmpstr/CRegistryLoaderComp.h"


// Qt includes
#include <QString>
#include <QFileInfo>
#include <QDir>
#include <QMessageBox>


#include "istd/TChangeNotifier.h"

#include "icmpstr/CRegistryModelComp.h"


namespace icmpstr
{


// reimplemented (iser::IFileLoader)

int CRegistryLoaderComp::LoadFromFile(istd::IChangeable& data, const istd::CString& filePath) const
{
	istd::CChangeNotifier notifier(&data);

	if (!IsOperationSupported(&data, &filePath, QF_NO_SAVING, false)){
		return StateFailed;
	}

	CRegistryModelComp* registryModelPtr = dynamic_cast<CRegistryModelComp*>(&data);
	if (registryModelPtr != NULL){
		iser::CXmlFileReadArchive registryArchive(filePath);
		I_ASSERT(!registryArchive.IsStoring());

		if (!registryModelPtr->SerializeRegistry(registryArchive)){
			OnReadError(registryArchive, data, filePath);

			SendErrorMessage(
						MI_LOAD_ERROR,
						iqt::GetCString(QObject::tr("Cannot load file %1").arg(iqt::GetQString(filePath))));

			return StateFailed;
		}

		iser::CXmlFileReadArchive layoutArchive(GetLayoutPath(filePath));
		I_ASSERT(!layoutArchive.IsStoring());

		if (!registryModelPtr->SerializeComponentsLayout(layoutArchive)){
			SendInfoMessage(
						MI_CANNOT_READ_LAYOUT,
						iqt::GetCString(QObject::tr("Layout information cannot be loaded (%1)").
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
	if (!IsOperationSupported(&data, &filePath, QF_NO_SAVING, false)){
		return StateFailed;
	}

	const CRegistryModelComp* registryModelPtr = dynamic_cast<const CRegistryModelComp*>(&data);
	if (registryModelPtr != NULL){
		iser::CXmlFileWriteArchive registryArchive(filePath);
		I_ASSERT(registryArchive.IsStoring());

		if (!const_cast<CRegistryModelComp*>(registryModelPtr)->SerializeRegistry(registryArchive)){
			SendErrorMessage(
						MI_LOAD_ERROR,
						iqt::GetCString(QObject::tr("Cannot store to file %1").arg(iqt::GetQString(filePath))));

			return StateFailed;
		}

		iser::CXmlFileWriteArchive layoutArchive(GetLayoutPath(filePath));
		I_ASSERT(layoutArchive.IsStoring());

		if (!const_cast<CRegistryModelComp*>(registryModelPtr)->SerializeComponentsLayout(layoutArchive)){
			SendInfoMessage(
						MI_CANNOT_READ_LAYOUT,
						iqt::GetCString(QObject::tr("Layout information cannot be stored (%1)").
									arg(iqt::GetQString(filePath))));
		}

		return StateOk;
	}

	return StateFailed;
}


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
		return iqt::GetCString(QObject::tr("ACF registry file"));
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

	QString message = QObject::tr("Cannot load object from file %1 (Line: %2)").arg(iqt::GetQString(filePath)).arg(lastReadLine);

	SendInfoMessage(MI_CANNOT_LOAD, iqt::GetCString(message));
}


} // namespace icmpstr


