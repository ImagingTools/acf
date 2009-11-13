#include "iqt/CFileList.h"
#include "iqt/CDirList.h"


#include <QFileInfo> 


namespace iqt
{


CFileList::CFileList(QObject* parent) 
:	QObject(parent)
{
}


bool CFileList::Create(
			const QDir& root,
			int minRecursionDepth,
			int maxRecursionDepth,
			const QStringList& nameFilters,
			QDir::SortFlags sortSpec)
{
	clear();

	CDirList dirList;
	if (!dirList.Create(root, minRecursionDepth, maxRecursionDepth)){
		return false;
	}

	for (QStringList::const_iterator dirIter = dirList.begin(); dirIter != dirList.end(); ++dirIter){
		QString dirPath = *dirIter;

		QDir dir(dirPath);
		dir.setFilter(QDir::Files);

		QStringList files = dir.entryList(nameFilters, QDir::Files, sortSpec);

		for (		QStringList::const_iterator fileIter = files.begin();
					fileIter != files.end();
					++fileIter){
			const QString& fileName = *fileIter;

			QString filePath = dir.absoluteFilePath(fileName);

			push_back(filePath);	

			Q_EMIT currentFile(filePath);
		}
	}

	return true;
}


} // namespace iqt


