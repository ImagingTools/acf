#include "iqtgui/CFileDialogLoaderComp.h"


// Qt includes
#include <QtCore/QObject>
#include <QtGui/QAction>
#include <QtGui/QFileDialog>


namespace iqtgui
{


// reimplemented (iser::IFileLoader)

bool CFileDialogLoaderComp::IsOperationSupported(
			const istd::IChangeable* dataObjectPtr,
			const QString* filePathPtr,
			int flags,
			bool beQuiet) const
{
	if (filePathPtr != NULL && !filePathPtr->isEmpty()){
		const iser::IFileLoader* loaderPtr = GetLoaderFor(*filePathPtr, -1, flags, beQuiet);
		if (loaderPtr != NULL){
			return loaderPtr->IsOperationSupported(dataObjectPtr, filePathPtr, flags, beQuiet);
		}
		return false;
	}

	if (m_loadersCompPtr.IsValid()){
		int correctedFlags = (flags | QF_FILE) & (~QF_ANONYMOUS);
		int loaderCount = m_loadersCompPtr.GetCount();
		for (int index = 0; index < loaderCount; index++){
			const iser::IFileLoader* slaveLoaderPtr = m_loadersCompPtr[index];
			if ((slaveLoaderPtr != NULL) && slaveLoaderPtr->IsOperationSupported(dataObjectPtr, NULL, correctedFlags)){
				return true;
			}
		}
	}

	if (!beQuiet){
		SendInfoMessage(MI_NONE_SUPPORTS, "None registered serializer supports specified type");
	}

	return false;
}


int CFileDialogLoaderComp::LoadFromFile(istd::IChangeable& data, const QString& filePath) const
{
	int selectionIndex = -1;
	QString openFileName = GetFileName(filePath, false, selectionIndex);
	if (openFileName.isEmpty()){
		return StateAborted;
	}

	iser::IFileLoader* loaderPtr = GetLoaderFor(openFileName, selectionIndex, QF_LOAD, false);
	if (loaderPtr != NULL){
		return loaderPtr->LoadFromFile(data, openFileName);
	}

	return StateFailed;
}


int CFileDialogLoaderComp::SaveToFile(const istd::IChangeable& data, const QString& filePath) const
{
	int selectionIndex = -1;

	QString saveFileName = GetFileName(filePath, true, selectionIndex);
	if (saveFileName.isEmpty()){
		return StateAborted;
	}

	iser::IFileLoader* loaderPtr = GetLoaderFor(saveFileName, selectionIndex, QF_SAVE, false);
	if (loaderPtr != NULL){
		return loaderPtr->SaveToFile(data, saveFileName);
	}

	return StateFailed;
}


// reimplemented (iser::IFileTypeInfo)

bool CFileDialogLoaderComp::GetFileExtensions(QStringList& result, int flags, bool doAppend) const
{
	if (!doAppend){
		result.clear();
	}

	int loadersCount = m_loadersCompPtr.GetCount();
	for (int i = 0; i < loadersCount; i++){
		const iser::IFileLoader* loaderPtr = m_loadersCompPtr[i];
		if (loaderPtr != NULL){
			loaderPtr->GetFileExtensions(result, flags, true);
		}
	}

	return false;
}


QString CFileDialogLoaderComp::GetTypeDescription(const QString* /*extensionPtr*/) const
{
	return "";
}


// reimplemented (iser::IFileLoaderInfo)

QString CFileDialogLoaderComp::GetLastFilePath(OperationType operationType, PathType pathType) const
{
	switch (operationType){
	case OT_LOAD:
		return GetPathForType(m_lastOpenInfo, pathType);

	case OT_SAVE:
		return GetPathForType(m_lastSaveInfo, pathType);

	default:
		if (!m_lastOpenInfo.filePath().isEmpty()){
			return GetPathForType(m_lastOpenInfo, pathType);
		}
		else{
			return GetPathForType(m_lastOpenInfo, pathType);
		}
	}
}


// static methods

void CFileDialogLoaderComp::AppendLoaderFilterList(const iser::IFileTypeInfo& fileTypeInfo, int flags, QString& allExt, QStringList& result)
{
	QStringList docExtensions;
	if (!fileTypeInfo.GetFileExtensions(docExtensions, flags)){
		return;
	}

	QString commonFilter;
	for (		QStringList::const_iterator extIter = docExtensions.begin();
				extIter != docExtensions.end();
				++extIter){
		const QString& extension = *extIter;

		if (!extension.isEmpty()){
			if (!commonFilter.isEmpty()){
				commonFilter += " ";
			}

			commonFilter += QString("*.") + extension;
		}
	}

	QString commonDescription = fileTypeInfo.GetTypeDescription();
	if (!commonDescription.isEmpty()){
		if (!commonFilter.isEmpty()){
			if (!allExt.isEmpty()){
				allExt += " ";
			}

			allExt += commonFilter;

			result += tr("%1 (%2)").arg(commonDescription).arg(commonFilter);
		}
	}
	else{
		for (		QStringList::const_iterator extIter = docExtensions.begin();
					extIter != docExtensions.end();
					++extIter){
			const QString& extension = *extIter;

			if (!extension.isEmpty()){
				QString description = fileTypeInfo.GetTypeDescription(&extension);

				allExt += extension;

				result += description + " (*." + extension + ")";
			}
		}
	}
}


// protected methods

QString CFileDialogLoaderComp::GetPathForType(const QFileInfo& fileInfo, PathType pathType) const
{
	switch (pathType){
	case OT_FILENAME:
		return fileInfo.fileName();

	case OT_DIR:
		return fileInfo.absolutePath();

	default:
		return fileInfo.absoluteFilePath();
	}
}


QString CFileDialogLoaderComp::GetFileName(const QString& filePath, bool isSaving, int& selectionIndex) const
{
	selectionIndex = -1;

	QString retVal = filePath;
	if (retVal.isEmpty()){
		QStringList filterList;
		QString allExt;

		int loadersCount = m_loadersCompPtr.GetCount();
		for (int i = 0; i < loadersCount; ++i){
			iser::IFileLoader* loaderPtr = m_loadersCompPtr[i];
			if (loaderPtr != NULL){
				iqtgui::CFileDialogLoaderComp::AppendLoaderFilterList(*loaderPtr, isSaving? QF_SAVE: QF_LOAD, allExt, filterList);
			}
		}

		if (filterList.size() > 1){
			filterList += tr("All known file types (%1)").arg(allExt);
		}

		QString selectedFilter;
		QString caption;

		I_ASSERT(m_useNativeAttrPtr.IsValid());

		bool useNativeDialogs = *m_useNativeAttrPtr;

		if (isSaving){
			caption = tr("Enter file name");

			if (!useNativeDialogs){
				QFileDialog dialog(
							NULL, 
							caption,
							m_lastSaveInfo.absoluteFilePath());
				dialog.setNameFilters(filterList);
				dialog.setAcceptMode(QFileDialog::AcceptSave);
				
				if (dialog.exec()){
					selectedFilter = dialog.selectedNameFilter();
					retVal = dialog.selectedFiles().join(";");
				}
			}
			else {
				retVal = QFileDialog::getSaveFileName(
							NULL,
							caption, 
							m_lastSaveInfo.absoluteFilePath(),
							filterList.join("\n"),
							&selectedFilter); 
			}
		}
		else{
			caption = tr("Select a file to open");

			if (!useNativeDialogs){
				QFileDialog dialog(
					NULL, 
					caption,
					m_lastSaveInfo.absoluteFilePath());
				dialog.setNameFilters(filterList);
				dialog.setAcceptMode(QFileDialog::AcceptOpen);

				if (dialog.exec()){
					selectedFilter = dialog.selectedNameFilter();
					retVal = dialog.selectedFiles().join(";");
				}
			}
			else {
				retVal = QFileDialog::getOpenFileName(
					NULL,
					caption,
					m_lastOpenInfo.absoluteFilePath(),
					filterList.join("\n"),
					&selectedFilter);
			}
		}

		QString enrolledFilters = filterList.join("\n");

		int selectedPos = enrolledFilters.indexOf(selectedFilter);
		enrolledFilters.truncate(selectedPos);
		selectionIndex = enrolledFilters.count('\n');
	}

	if (!retVal.isEmpty()){
		QFileInfo fileInfo(retVal);
		if (isSaving){
			m_lastSaveInfo = fileInfo;
		}
		else{
			m_lastOpenInfo = fileInfo;
		}

		if (m_statupDirectoryCompPtr.IsValid()){
			m_statupDirectoryCompPtr->SetPath(fileInfo.dir().absolutePath());
		}
	}

	return retVal;
}


iser::IFileLoader* CFileDialogLoaderComp::GetLoaderFor(const QString& filePath, int selectionIndex, int flags, bool beQuiet) const
{
	iser::IFileLoader* retVal = NULL;

	QFileInfo fileInfo(filePath);
	QString fileExtension = (QString("*.") + fileInfo.suffix()).toLower();

	int filtersSum = 0;
	int loadersCount = m_loadersCompPtr.GetCount();
	for (int i = 0; i < loadersCount; i++){
		iser::IFileLoader* loaderPtr = m_loadersCompPtr[i];
		if (loaderPtr != NULL){
			QString extensions;
			QStringList filters;
			AppendLoaderFilterList(*loaderPtr, flags, extensions, filters);
		
			if (extensions.contains(fileExtension)){
				if ((selectionIndex < 0) || ((selectionIndex >= filtersSum) && (selectionIndex < filtersSum + filters.size()))){
					return loaderPtr;
				}

				retVal = loaderPtr;
			}

			filtersSum += filters.size();
		}
	}

	if (!beQuiet){
		SendInfoMessage(MI_NONE_SUPPORTS, "None registered serializer supports specified file");
	}

	return retVal;
}


// reimplemented (icomp::CComponentBase)

void CFileDialogLoaderComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_statupDirectoryCompPtr.IsValid()){
		QString lastDirectoryPath = m_statupDirectoryCompPtr->GetPath();

		m_lastOpenInfo.setFile(lastDirectoryPath);
		m_lastSaveInfo.setFile(lastDirectoryPath);
	}
}


} // namespace iqtgui


