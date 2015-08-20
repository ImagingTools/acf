#include "ifilegui/CFileDialogLoaderComp.h"


// Qt includes
#include <QtCore/QObject>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QAction>
#include <QtWidgets/QFileDialog>
#else
#include <QtGui/QAction>
#include <QtGui/QFileDialog>
#endif


namespace ifilegui
{


// reimplemented (ifile::IFilePersistence)

bool CFileDialogLoaderComp::IsOperationSupported(
			const istd::IChangeable* dataObjectPtr,
			const QString* filePathPtr,
			int flags,
			bool beQuiet) const
{
	if (filePathPtr != NULL && !filePathPtr->isEmpty()){
		const ifile::IFilePersistence* loaderPtr = GetLoaderFor(dataObjectPtr, *filePathPtr, -1, flags, beQuiet);
		if (loaderPtr != NULL){
			return loaderPtr->IsOperationSupported(dataObjectPtr, filePathPtr, flags, beQuiet);
		}
		return false;
	}

	if (m_loadersCompPtr.IsValid()){
		int correctedFlags = (flags | QF_FILE) & (~QF_ANONYMOUS);
		int loaderCount = m_loadersCompPtr.GetCount();
		for (int index = 0; index < loaderCount; index++){
			const ifile::IFilePersistence* slaveLoaderPtr = m_loadersCompPtr[index];
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


int CFileDialogLoaderComp::LoadFromFile(
			istd::IChangeable& data,
			const QString& filePath,
			ibase::IProgressManager* /*progressManagerPtr*/) const
{
	int selectionIndex = -1;
	QString openFileName = GetFileName(data, filePath, false, selectionIndex);
	if (openFileName.isEmpty()){
		return OS_CANCELED;
	}

	ifile::IFilePersistence* loaderPtr = GetLoaderFor(&data, openFileName, selectionIndex, QF_LOAD, false);
	if (loaderPtr != NULL){
		return loaderPtr->LoadFromFile(data, openFileName);
	}

	return OS_FAILED;
}


int CFileDialogLoaderComp::SaveToFile(
			const istd::IChangeable& data,
			const QString& filePath,
			ibase::IProgressManager* /*progressManagerPtr*/) const
{
	int selectionIndex = -1;

	QString saveFileName = GetFileName(data, filePath, true, selectionIndex);
	if (saveFileName.isEmpty()){
		return OS_CANCELED;
	}

	ifile::IFilePersistence* loaderPtr = GetLoaderFor(&data, saveFileName, selectionIndex, QF_SAVE, false);
	if (loaderPtr != NULL){
		return loaderPtr->SaveToFile(data, saveFileName);
	}

	return OS_FAILED;
}


// reimplemented (ifile::IFileTypeInfo)

bool CFileDialogLoaderComp::GetFileExtensions(QStringList& result, const istd::IChangeable* dataObjectPtr, int flags, bool doAppend) const
{
	if (!doAppend){
		result.clear();
	}

	int loadersCount = m_loadersCompPtr.GetCount();
	for (int i = 0; i < loadersCount; i++){
		const ifile::IFilePersistence* loaderPtr = m_loadersCompPtr[i];
		if (loaderPtr != NULL){
			loaderPtr->GetFileExtensions(result, dataObjectPtr, flags, true);
		}
	}

	return false;
}


QString CFileDialogLoaderComp::GetTypeDescription(const QString* /*extensionPtr*/) const
{
	return "";
}


// reimplemented (ifile::IFilePersistenceInfo)

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

void CFileDialogLoaderComp::AppendLoaderFilterList(const ifile::IFileTypeInfo& fileTypeInfo, const istd::IChangeable* dataObjectPtr, int flags, QStringList& allExt, QStringList& result)
{
	QStringList docExtensions;
	if (!fileTypeInfo.GetFileExtensions(docExtensions, dataObjectPtr, flags)){
		return;
	}

	QString commonDescription = fileTypeInfo.GetTypeDescription();
	if (!commonDescription.isEmpty() && ((flags & QF_SAVE) == 0)){
		if (!docExtensions.isEmpty()){
			allExt += docExtensions;

			result += tr("%1 (%2)").arg(commonDescription).arg("*." + docExtensions.join(" *."));
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


QString CFileDialogLoaderComp::GetFileName(const istd::IChangeable& data, const QString& filePath, bool isSaving, int& selectionIndex) const
{
	selectionIndex = -1;

	QString retVal = filePath;
	if (retVal.isEmpty()){
		QStringList filterList;
		QStringList allExt;

		int loadersCount = m_loadersCompPtr.GetCount();
		for (int i = 0; i < loadersCount; ++i){
			ifile::IFilePersistence* loaderPtr = m_loadersCompPtr[i];
			if (loaderPtr != NULL){
				AppendLoaderFilterList(*loaderPtr, &data, isSaving? QF_SAVE: QF_LOAD, allExt, filterList);
			}
		}

		if (!isSaving){
			if (filterList.size() > 1){
				filterList.prepend(tr("All known file types (%1)").arg("*." + allExt.join(" *.")));
			}
		}

		QString selectedFilter;
		QString caption;

		Q_ASSERT(m_useNativeAttrPtr.IsValid());

		bool useNativeDialogs = *m_useNativeAttrPtr;

		if (isSaving){
			selectedFilter = m_lastSaveInfo.suffix();
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
			selectedFilter = m_lastOpenInfo.suffix();
			caption = tr("Select a file to open");

			if (!useNativeDialogs){
				QFileDialog dialog(
					NULL,
					caption,
					m_lastOpenInfo.absoluteFilePath());
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


ifile::IFilePersistence* CFileDialogLoaderComp::GetLoaderFor(const istd::IChangeable* dataObjectPtr, const QString& filePath, int selectionIndex, int flags, bool beQuiet) const
{
	ifile::IFilePersistence* retVal = NULL;

	QFileInfo fileInfo(filePath);
	QString fileExtension = fileInfo.suffix().toLower();

	int filtersSum = 0;
	int loadersCount = m_loadersCompPtr.GetCount();
	for (int i = 0; i < loadersCount; i++){
		ifile::IFilePersistence* loaderPtr = m_loadersCompPtr[i];
		if (loaderPtr != NULL){
			QStringList allExt;
			QStringList filters;
			AppendLoaderFilterList(*loaderPtr, dataObjectPtr, flags, allExt, filters);

			if (allExt.contains(fileExtension)){
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


} // namespace ifilegui


