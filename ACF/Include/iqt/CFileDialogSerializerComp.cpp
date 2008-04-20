#include "iqt/CFileDialogSerializerComp.h"


#include <QAction>
#include <QFileDialog>


namespace iqt
{


// reimplemented (istd::IFileLoader)

bool CFileDialogSerializerComp::IsObjectSupported(const istd::IChangeable& dataObject) const
{
	if (m_serializersCompPtr.IsValid()){
		int loaderCount = m_serializersCompPtr.GetCount();
		for (int index = 0; index < loaderCount; index++){
			istd::IFileLoader* slaveLoaderPtr = m_serializersCompPtr[index];
			if (slaveLoaderPtr != NULL && slaveLoaderPtr->IsObjectSupported(dataObject)){
				return true;
			}
		}
	}

	return false;
}


bool CFileDialogSerializerComp::IsFileSupported(const istd::CString& filePath) const
{
	const istd::IFileLoader* loaderPtr = GetLoaderFor(iqt::GetQString(filePath));
	if (loaderPtr != NULL){
		return loaderPtr->IsFileSupported(filePath);
	}

	return false;
}


int CFileDialogSerializerComp::LoadFromFile(istd::IChangeable& data, const istd::CString& filePath) const
{
	QString openFileName = GetOpenFileName(filePath);
	if (openFileName.isEmpty()){
		return StateAborted;
	}

	istd::IFileLoader* loaderPtr = GetLoaderFor(openFileName);
	if (loaderPtr != NULL){
		return loaderPtr->LoadFromFile(data, iqt::GetCString(openFileName));
	}

	return StateFailed;
}


int CFileDialogSerializerComp::SaveToFile(const istd::IChangeable& data, const istd::CString& filePath) const
{
	QString saveFileName = GetSaveFileName(filePath);
	if (saveFileName.isEmpty()){
		return StateAborted;
	}

	istd::IFileLoader* loaderPtr = GetLoaderFor(saveFileName);
	if (loaderPtr != NULL){
		return loaderPtr->SaveToFile(data, iqt::GetCString(saveFileName));
	}

	return StateFailed;
}


const istd::CString& CFileDialogSerializerComp::GetLastSaveFileName() const
{
	return m_lastSaveFileName;
}


const istd::CString& CFileDialogSerializerComp::GetLastOpenFileName() const
{
	return m_lastOpenFileName;
}


// protected methods

QString CFileDialogSerializerComp::GetOpenFileName(const istd::CString& filePath) const
{
	if (!m_fileFiltersAttrPtr.IsValid()){
		return QString();
	}

	QString openFileName = iqt::GetQString(filePath);
	if (openFileName.isEmpty()){
		QString fileFilter;

		for (int index = 0; index < int(m_fileFiltersAttrPtr.GetCount()); index++){
			fileFilter += iqt::GetQString(m_fileFiltersAttrPtr[index]);
			if (index < int(m_fileFiltersAttrPtr.GetCount())-1){
				fileFilter += "\n";
			}
		}	

		openFileName = QFileDialog::getOpenFileName(NULL, QObject::tr("Select a file to open"), m_lastOpenPath, fileFilter); 
	}

	m_lastOpenFileName = iqt::GetCString(openFileName);

	QFileInfo fileInfo(openFileName);
	m_lastOpenPath = fileInfo.absolutePath();

	return openFileName;
}


QString CFileDialogSerializerComp::GetSaveFileName(const istd::CString& filePath) const
{
	QString saveFileName = iqt::GetQString(filePath);
	if (saveFileName.isEmpty()){
		QString fileFilter;

		for (int index = 0; index < int(m_fileFiltersAttrPtr.GetCount()); index++){
			fileFilter += iqt::GetQString(m_fileFiltersAttrPtr[index]);
			if (index < int(m_fileFiltersAttrPtr.GetCount())-1){
				fileFilter += "\n";
			}
		}	

		saveFileName = QFileDialog::getSaveFileName(NULL,
													QObject::tr("Enter file name"), 
													m_lastSavePath,
													fileFilter); 
	}

	m_lastSaveFileName = iqt::GetCString(saveFileName);

	QFileInfo fileInfo(saveFileName);
	m_lastSavePath = fileInfo.absolutePath();

	return saveFileName;
}


istd::IFileLoader* CFileDialogSerializerComp::GetLoaderFor(const QString& filePath) const
{
	QFileInfo fileInfo(filePath);

	QString fileExtension = QString(".") + fileInfo.suffix();

	int filtersCount = istd::Min(m_fileFiltersAttrPtr.GetCount(), m_serializersCompPtr.GetCount());
	for (int index = 0; index < filtersCount; index++){
		QString fileFilter = iqt::GetQString(m_fileFiltersAttrPtr[index]);
		if (fileFilter.contains(fileExtension)){
			return m_serializersCompPtr[index];
		}
	}

	return NULL;
}


} // namespace iqt