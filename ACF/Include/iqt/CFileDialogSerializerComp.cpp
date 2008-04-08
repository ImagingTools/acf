#include "iqt/CFileDialogSerializerComp.h"


#include <QAction>
#include <QFileDialog>


namespace iqt
{


// reimplemented (iser::IFileSerializer)

int CFileDialogSerializerComp::LoadFromFile(iser::ISerializable& data, const istd::CString& filePath) const
{
	QString openFileName = GetOpenFileName(filePath);
	if (openFileName.isEmpty()){
		return StateAborted;
	}

	iser::IFileSerializer* serializerPtr = GetSerializerFor(openFileName);
	if (serializerPtr != NULL){
		return serializerPtr->LoadFromFile(data, iqt::GetCString(openFileName));
	}

	return StateFailed;
}


int CFileDialogSerializerComp::SaveToFile(const iser::ISerializable& data, const istd::CString& filePath) const
{
	QString saveFileName = getSaveFileName(filePath);
	if (saveFileName.isEmpty()){
		return StateAborted;
	}

	iser::IFileSerializer* serializerPtr = GetSerializerFor(saveFileName);
	if (serializerPtr != NULL){
		return serializerPtr->SaveToFile(data, iqt::GetCString(saveFileName));
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
			fileFilter += iqt::GetQString(m_fileFiltersAttrPtr.GetValue(index));
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
			fileFilter += iqt::GetQString(m_fileFiltersAttrPtr.GetValue(index));
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


iser::IFileSerializer* CFileDialogSerializerComp::GetSerializerFor(const QString& filePath) const
{
	QFileInfo fileInfo(filePath);

	QString fileExtension = QString(".") + fileInfo.suffix();

	for (int index = 0; index < int(m_fileFiltersAttrPtr.GetCount()); index++){
		QString fileFilter = iqt::GetQString(m_fileFiltersAttrPtr.GetValue(index));
		if (fileFilter.contains(fileExtension)){
			if (index < m_serializersCompPtr.GetCount()){
				return m_serializersCompPtr.GetInterfacePtr(index);			
			}
		}
	}

	return NULL;
}


} // namespace iqt