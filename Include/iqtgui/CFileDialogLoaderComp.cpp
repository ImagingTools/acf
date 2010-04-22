#include "iqtgui/CFileDialogLoaderComp.h"


// Qt includes
#include <QObject>
#include <QAction>
#include <QFileDialog>


namespace iqtgui
{


// reimplemented (iser::IFileLoader)

bool CFileDialogLoaderComp::IsOperationSupported(
			const istd::IChangeable* dataObjectPtr,
			const istd::CString* filePathPtr,
			int flags,
			bool beQuiet) const
{
	if (filePathPtr != NULL && !filePathPtr->IsEmpty()){
		const iser::IFileLoader* loaderPtr = GetLoaderFor(iqt::GetQString(*filePathPtr), -1, flags, beQuiet);
		if (loaderPtr != NULL){
			return loaderPtr->IsOperationSupported(dataObjectPtr, filePathPtr, flags | QF_NAMED_ONLY, beQuiet);
		}
		return false;
	}

	if (m_loadersCompPtr.IsValid()){
		int correctedFlags = (flags | QF_NAMED_ONLY) & (~QF_ANONYMOUS_ONLY);
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


int CFileDialogLoaderComp::LoadFromFile(istd::IChangeable& data, const istd::CString& filePath) const
{
	int selectionIndex = -1;
	QString openFileName = GetFileName(filePath, false, selectionIndex);
	if (openFileName.isEmpty()){
		return StateAborted;
	}

	iser::IFileLoader* loaderPtr = GetLoaderFor(openFileName, selectionIndex, QF_NO_SAVING, false);
	if (loaderPtr != NULL){
		return loaderPtr->LoadFromFile(data, iqt::GetCString(openFileName));
	}

	return StateFailed;
}


int CFileDialogLoaderComp::SaveToFile(const istd::IChangeable& data, const istd::CString& filePath) const
{
	int selectionIndex = -1;

	QString saveFileName = GetFileName(filePath, true, selectionIndex);
	if (saveFileName.isEmpty()){
		return StateAborted;
	}

	iser::IFileLoader* loaderPtr = GetLoaderFor(saveFileName, selectionIndex, QF_NO_LOADING, false);
	if (loaderPtr != NULL){
		return loaderPtr->SaveToFile(data, iqt::GetCString(saveFileName));
	}

	return StateFailed;
}


// reimplemented (iser::IFileTypeInfo)

bool CFileDialogLoaderComp::GetFileExtensions(istd::CStringList& result, int flags, bool doAppend) const
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


istd::CString CFileDialogLoaderComp::GetTypeDescription(const istd::CString* /*extensionPtr*/) const
{
	return "";
}


// reimplemented (iser::IFileLoaderInfo)

istd::CString CFileDialogLoaderComp::GetLastFilePath(OperationType operationType, PathType pathType) const
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

int CFileDialogLoaderComp::AppendLoaderFilterList(const iser::IFileTypeInfo& fileTypeInfo, int flags, QString& allExt, QString& result)
{
	int retVal = 0;

	istd::CStringList docExtensions;
	fileTypeInfo.GetFileExtensions(docExtensions, flags);

	istd::CString commonDescription = fileTypeInfo.GetTypeDescription();

	if (!commonDescription.IsEmpty()){
		if (!result.isEmpty()){
			result += "\n";
		}

		result += iqt::GetQString(commonDescription + " (");
	}

	for (		istd::CStringList::const_iterator extIter = docExtensions.begin();
				extIter != docExtensions.end();
				++extIter){
		const istd::CString& extension = *extIter;

		if (!extension.IsEmpty()){
			if (commonDescription.IsEmpty()){
				istd::CString description = fileTypeInfo.GetTypeDescription(&extension);

				if (!result.isEmpty()){
					result += "\n";
				}

				result += iqt::GetQString(description + " (*." + extension + ")");

				++retVal;
			}
			else{
				if (extIter != docExtensions.begin()){
					result += ";";
				}

				result += iqt::GetQString(istd::CString("*.") + extension);
			}

			if (!allExt.isEmpty()){
				allExt += ";";
			}

			allExt += "*." + iqt::GetQString(extension).toLower();
		}
	}

	if (!commonDescription.IsEmpty()){
		result += ")";

		++retVal;
	}

	return retVal;
}


// protected methods

istd::CString CFileDialogLoaderComp::GetPathForType(const QFileInfo& fileInfo, PathType pathType) const
{
	switch (pathType){
	case OT_FILENAME:
		return iqt::GetCString(fileInfo.fileName());

	case OT_DIR:
		return iqt::GetCString(fileInfo.absolutePath());

	default:
		return iqt::GetCString(fileInfo.absoluteFilePath());
	}
}


QString CFileDialogLoaderComp::GetFileName(const istd::CString& filePath, bool isSaving, int& selectionIndex) const
{
	selectionIndex = -1;

	QString retVal = iqt::GetQString(filePath);
	if (retVal.isEmpty()){
		QString filterList;
		QString allExt;
		int filtersCount = 0;

		int loadersCount = m_loadersCompPtr.GetCount();
		for (int i = 0; i < loadersCount; ++i){
			iser::IFileLoader* loaderPtr = m_loadersCompPtr[i];
			if (loaderPtr != NULL){
				filtersCount += iqtgui::CFileDialogLoaderComp::AppendLoaderFilterList(*loaderPtr, isSaving? QF_NO_LOADING: QF_NO_SAVING, allExt, filterList);
			}
		}

		if (filtersCount > 1){
			filterList += "\n";
			filterList += QObject::tr("All known file types (%1)").arg(allExt);
		}

		QString selectedFilter;
		if (isSaving){
			retVal = QFileDialog::getSaveFileName(
						NULL,
						QObject::tr("Enter file name"), 
						m_lastSaveInfo.absolutePath(),
						filterList,
						&selectedFilter); 
		}
		else{
			retVal = QFileDialog::getOpenFileName(
						NULL,
						QObject::tr("Select a file to open"),
						m_lastOpenInfo.absolutePath(),
						filterList,
						&selectedFilter);
		}

		int selectedPos = filterList.indexOf(selectedFilter);
		filterList.truncate(selectedPos);
		selectionIndex = filterList.count('\n');
	}

	QFileInfo fileInfo(retVal);
	if (isSaving){
		m_lastSaveInfo = fileInfo;
	}
	else{
		m_lastOpenInfo = fileInfo;
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
			QString filters;
			int filtersCount = AppendLoaderFilterList(*loaderPtr, flags, extensions, filters);
		
			if (extensions.contains(fileExtension)){
				if ((selectionIndex < 0) || ((selectionIndex >= filtersSum) && (selectionIndex < filtersSum + filtersCount))){
					return loaderPtr;
				}

				retVal = loaderPtr;
			}

			filtersSum += filtersCount;
		}
	}

	if (!beQuiet){
		SendInfoMessage(MI_NONE_SUPPORTS, "None registered serializer supports specified file");
	}

	return retVal;
}


} // namespace iqtgui


