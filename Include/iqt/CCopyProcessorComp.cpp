#include "iqt/CCopyProcessorComp.h"


// Qt includes
#include <QRegExp>


// ACF includes
#include "iqt/CFileSystem.h"


namespace iqt
{


// protected methods

bool CCopyProcessorComp::CheckFileExistTree(
			const QDir& inputDir,
			const QStringList& filters,
			const QStringList& excludeFilters,
			int recursionDepth) const
{
	QFileInfoList fileList = inputDir.entryInfoList(filters, QDir::Files | QDir::NoDotAndDotDot);
	for (		QFileInfoList::const_iterator fileIter = fileList.begin();
				fileIter != fileList.end();
				++fileIter){
		const QFileInfo& fileInfo = *fileIter;
		QString fileName = fileInfo.fileName();

		if (!CheckIfExcluded(fileName, excludeFilters)){
			return true;
		}
	}

	if (recursionDepth > 0){
		QFileInfoList dirList = inputDir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
		for (		QFileInfoList::const_iterator dirIter = dirList.begin();
					dirIter != dirList.end();
					++dirIter){
			const QFileInfo& fileInfo = *dirIter;
			QString fileName = fileInfo.fileName();

			if (CheckIfExcluded(fileName, excludeFilters)){
				continue;
			}

			QDir inputSubDir(inputDir.absoluteFilePath(fileName));

			if (CheckFileExistTree(inputSubDir, filters, excludeFilters, recursionDepth - 1)){
				return true;
			}
		}
	}

	return false;
}


bool CCopyProcessorComp::CopyFileTree(
			const QDir& inputDir,
			const QDir& outputDir,
			const QStringList& filters,
			const QStringList& excludeFilters,
			int recursionDepth,
			int& counter) const
{
	if (!inputDir.exists()){
		SendWarningMessage(MI_NO_INPUT, iqt::GetCString(tr("No input directory %1").arg(inputDir.absolutePath())));

		return false;
	}

	outputDir.mkpath(outputDir.absolutePath());
	if (!outputDir.exists()){
		SendWarningMessage(MI_NO_OUTPUT, iqt::GetCString(tr("No output directory %1").arg(outputDir.absolutePath())));

		return false;
	}

	bool retVal = true;

	QFileInfoList fileList = inputDir.entryInfoList(filters, QDir::Files | QDir::NoDotAndDotDot);
	for (		QFileInfoList::const_iterator fileIter = fileList.begin();
				fileIter != fileList.end();
				++fileIter){
		const QFileInfo& fileInfo = *fileIter;
		QString fileName = fileInfo.fileName();

		if (CheckIfExcluded(fileName, excludeFilters)){
			continue;
		}

		QString inputFilePath = inputDir.absoluteFilePath(fileName);
		QString outputFilePath = outputDir.absoluteFilePath(fileName);

		I_ASSERT(m_fileCopyCompPtr.IsValid());	// it should be checked before whole process started

		if (m_fileCopyCompPtr->CopyFile(iqt::GetCString(inputFilePath), iqt::GetCString(outputFilePath))){
			counter++;
		}
	}

	if (recursionDepth > 0){
		QFileInfoList dirList = inputDir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
		for (		QFileInfoList::const_iterator dirIter = dirList.begin();
					dirIter != dirList.end();
					++dirIter){
			const QFileInfo& fileInfo = *dirIter;
			QString fileName = fileInfo.fileName();

			if (CheckIfExcluded(fileName, excludeFilters)){
				continue;
			}

			QDir inputSubDir(inputDir.absoluteFilePath(fileName));
			QDir outputSubDir(outputDir.absoluteFilePath(fileName));

			if (CheckFileExistTree(inputSubDir, filters, excludeFilters, recursionDepth - 1)){
				retVal = CopyFileTree(inputSubDir, outputSubDir, filters, excludeFilters, recursionDepth - 1, counter) && retVal;
			}
		}
	}

	return retVal;
}


bool CCopyProcessorComp::CheckIfExcluded(const QString& fileName, const QStringList& excludeFilters) const
{
	for (		QStringList::const_iterator excludeIter = excludeFilters.begin();
				excludeIter != excludeFilters.end();
				++excludeIter){
		QRegExp rx(*excludeIter);
		rx.setPatternSyntax(QRegExp::Wildcard);
		rx.setCaseSensitivity(Qt::CaseInsensitive);
		if (rx.exactMatch(fileName)){
			return true;
		}
	}

	return false;
}


// reimplemented (icomp::CComponentBase)

void CCopyProcessorComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (!m_fileCopyCompPtr.IsValid()){
		SendErrorMessage(MI_END_STATUS, iqt::GetCString(tr("File copy provider is not present")));

		return;
	}

	QStringList filters;
	for (int filterIndex = 0; filterIndex < m_filtersAttrPtr.GetCount(); ++filterIndex){
		const istd::CString& filter = m_filtersAttrPtr[filterIndex];

		filters << iqt::GetQString(filter);
	}

	QStringList excludeFilters;
	for (int excludeIndex = 0; excludeIndex < m_excludeFiltersAttrPtr.GetCount(); ++excludeIndex){
		const istd::CString& excludeFilter = m_excludeFiltersAttrPtr[excludeIndex];

		excludeFilters << iqt::GetQString(excludeFilter);
	}

	int counter = 0;
	if (CopyFileTree(
				iqt::CFileSystem::GetEnrolledPath(iqt::GetQString(*m_inputPathAttrPtr)),
				iqt::CFileSystem::GetEnrolledPath(iqt::GetQString(*m_outputPathAttrPtr)),
				filters,
				excludeFilters,
				*m_recursionDepthAttrPtr,
				counter)){

		SendInfoMessage(MI_END_STATUS, iqt::GetCString(tr("Success: %1 files copied").arg(counter)));
	}
	else{
		SendErrorMessage(MI_END_STATUS, iqt::GetCString(tr("Failed: %1 files copied").arg(counter)));
	}
}


} // namespace iqt


