#include "iqt/CDirList.h"

#include <QFileInfo>


namespace iqt
{


CDirList::CDirList(QObject* parent) 
:	QObject(parent)
{
}


CDirList::CDirList(
			QObject* parent,
			const QDir& root,
			int minRecursionDepth,
			int maxRecursionDepth) 
:	QObject(parent)
{
	Create(root, minRecursionDepth, maxRecursionDepth);
}



bool CDirList::Create(
			const QDir& root,
			int minRecursionDepth,
			int maxRecursionDepth,
			bool doAppend)
{
	I_ASSERT(minRecursionDepth >= 0);

	if (!doAppend){
		clear();
	}

	QString rootPath = root.absolutePath();

	if (rootPath.isEmpty()){
		return false;
	}

	QFileInfo fileInfo(rootPath);
	if (!fileInfo.isDir()){
		return false;
	}

	if (minRecursionDepth == 0){
		push_back(rootPath);
	}
	
	DoSearch(root, minRecursionDepth, maxRecursionDepth);

	return true;
}


// protected methods

void CDirList::DoSearch(
			const QDir& root,
			int minRecursionDepth,
			int maxRecursionDepth)
{
	QStringList entries = root.entryList(QDir::Dirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);

	for (		QStringList::const_iterator iter = entries.begin();
				iter != entries.end();
				++iter){
		const QString& subDirName = *iter;

		QString subDirPath = root.absoluteFilePath(subDirName);

		if (minRecursionDepth <= 0){
			push_back(subDirPath);

			emit currentDir(subDirPath);
		}

		if (maxRecursionDepth != 0){
			QDir subDir = root;

			subDir.setPath(subDirPath);
			
			DoSearch(subDir, minRecursionDepth - 1, maxRecursionDepth - 1);
		}
	}
}


} // namespace iqt


