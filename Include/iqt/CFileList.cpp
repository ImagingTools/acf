#include "iqt/CFileList.h"
#include "iqt/CDirList.h"


#include <QFileInfo> 


namespace iqt
{


CFileList::CFileList(QObject* parent) 
	:QObject(parent)
{
}


bool CFileList::Create(const QDir& root, bool isRecursive, const QStringList& nameFilters, QDir::SortFlags sortSpec)
{
	clear();

	CDirList dirList;

	if (isRecursive){
		if (!dirList.Create(root, isRecursive)) 
			return false;
	}
	else{
		dirList.push_back(root.path() );
	}

	for (QStringList::Iterator itDir = dirList.begin(); itDir != dirList.end(); ++itDir){
		QString currenDirectoryPath = *itDir;
		QDir dir(currenDirectoryPath);
		dir.setFilter(QDir::Files);

		QStringList entries = dir.entryList(nameFilters, QDir::NoFilter, sortSpec);

		for (QStringList::size_type j = 0; j < entries.size(); j++){
			if (entries[j] == "." || entries[j] == ".."){
				continue;
			}
		
			QFileInfo ff( currenDirectoryPath, entries[j]);
			QString pat = ff.absoluteFilePath();
			entries[j] = pat;
		}
		
		for (QStringList::size_type i = 0; i < entries.size(); i++){
			QString directoryEntry = entries[i];
			QFileInfo fileInfo(directoryEntry);
			
			QString fileName = fileInfo.fileName();
			
			if (fileName == "." || fileName == ".."){
				continue;
			}
			
			if (fileInfo.isDir()){
				continue;
			}
		
			directoryEntry = QDir::convertSeparators(directoryEntry);
			emit current(directoryEntry);
			push_back(directoryEntry);	
		}
	}

	return true;
}


bool CFileList::Create(const QString& root, bool isRecursive, const QStringList& nameFilters, QDir::SortFlags sortSpec)
{
	return Create(QDir(root), isRecursive, nameFilters, sortSpec);
}


} // namespace iqt
