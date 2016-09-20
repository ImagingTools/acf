#include <ifile/CComposedFileMetaInfoProviderComp.h>


// Qt includes
#include <QtCore/QFileInfo>

// ACF includes
#include <ifile/IFileTypeInfo.h>


namespace ifile
{


// public methods

// reimplemented (ifile::IFileMetaInfoProvider)

ifile::IFileMetaInfoProvider::MetaInfoPtr CComposedFileMetaInfoProviderComp::GetFileMetaInfo(const QString& filePath) const
{
	QString fileExtension = QFileInfo(filePath).suffix();

	for (int i = 0; i < m_slaveInfoProvidersCompPtr.GetCount(); ++i){
		ifile::IFileMetaInfoProvider* providerPtr = m_slaveInfoProvidersCompPtr[i];

		ifile::IFileTypeInfo* fileTypeInfoPtr = dynamic_cast<ifile::IFileTypeInfo*>(providerPtr);
		if (fileTypeInfoPtr != NULL){
			QStringList supportedExtensions;
			if (fileTypeInfoPtr->GetFileExtensions(supportedExtensions)){
				if (!supportedExtensions.contains(fileExtension, Qt::CaseInsensitive)){
					continue;
				}
			}
		}

		if (providerPtr != NULL){
			MetaInfoPtr retVal = providerPtr->GetFileMetaInfo(filePath);
			if (retVal.IsValid()){
				return retVal;
			}
		}
	}

	return MetaInfoPtr();
}


} // namespace ifile


