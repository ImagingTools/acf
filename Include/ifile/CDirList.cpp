#include "ifile/CDirList.h"


// Qt includes
#include <QtCore/QFileInfo>


namespace ifile
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
	Q_ASSERT(minRecursionDepth >= 0);

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

	DoSearch(root, minRecursionDepth, maxRecursionDepth);

	return true;
}


// protected methods

void CDirList::DoSearch(
			const QDir& root,
			int minRecursionDepth,
			int maxRecursionDepth)
{
	if (minRecursionDepth <= 0){
		QString rootPath = root.absolutePath();

		push_back(rootPath);

		Q_EMIT currentDir(rootPath);
	}

	if (maxRecursionDepth == 0){
		return;
	}

	QStringList entries = root.entryList(QDir::Dirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);
	for (		QStringList::const_iterator iter = entries.begin();
				iter != entries.end();
				++iter){
		const QString& subDirName = *iter;

		QDir subDir = root;
		subDir.setPath(root.absoluteFilePath(subDirName));

		DoSearch(subDir, minRecursionDepth - 1, maxRecursionDepth - 1);
	}
}


} // namespace ifile


