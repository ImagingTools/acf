#include "CRegistryLoaderComp.h"


// Qt includes
#include <QString>
#include <QFileInfo>
#include <QDir>
#include <QMessageBox>

// ACF includes
#include "iqt/iqt.h"

#include "CRegistryModelComp.h"


// reimplemented (iser::IFileLoader)

int CRegistryLoaderComp::LoadFromFile(istd::IChangeable& data, const istd::CString& filePath) const
{
	int retVal = BaseClass::LoadFromFile(data, filePath);

	if (retVal == StateOk){
		CRegistryModelComp* registryModelPtr = dynamic_cast<CRegistryModelComp*>(&data);
		if (registryModelPtr != NULL){
			iser::CXmlFileReadArchive archive(GetLayoutPath(filePath));

			if (!registryModelPtr->SerializeComponentsLayout(archive)){
				QMessageBox::information(NULL, QObject::tr("Registry Loader"), QObject::tr("Layout information cannot be loaded"));
			}
		}
	}

	return retVal;
}


int CRegistryLoaderComp::SaveToFile(const istd::IChangeable& data, const istd::CString& filePath) const
{
	int retVal = BaseClass::SaveToFile(data, filePath);

	if (retVal == StateOk){
		const CRegistryModelComp* registryModelPtr = dynamic_cast<const CRegistryModelComp*>(&data);
		if (registryModelPtr != NULL){
			iser::CXmlFileWriteArchive archive(GetLayoutPath(filePath), GetVersionInfo());
			I_ASSERT(archive.IsStoring());

			if (!(const_cast<CRegistryModelComp*>(registryModelPtr))->SerializeComponentsLayout(archive)){
				QMessageBox::information(NULL, QObject::tr("Registry Loader"), QObject::tr("Layout information cannot be stored"));
			}
		}
	}

	return retVal;
}


// protected methods

istd::CString CRegistryLoaderComp::GetLayoutPath(const istd::CString& registryPath) const
{
	QFileInfo fileInfo(iqt::GetQString(registryPath));
	QString layoutPath = fileInfo.dir().absoluteFilePath(fileInfo.completeBaseName() + ".alx");

	return iqt::GetCString(layoutPath);
}


