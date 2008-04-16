#include "iqt/CFileList.h"
#include "iqt/CDirList.h"


#include <QApplication>
#include <QFileInfo> 


namespace iqt
{


CFileList::CFileList(QObject* parent) 
	:QObject(parent)
{
}


bool CFileList::Create(const QDir& root, bool recursive, const QStringList& nameFilters, QDir::SortFlags sortSpec)
{
	clear();

	CDirList dirList;

	if (recursive){
		if (!dirList.Create(root, recursive)) 
			return false;
	}
	else{
		dirList.push_back( root.path() );
	}

	for (QStringList::Iterator itDir = dirList.begin(); itDir != dirList.end(); ++itDir){
		QString strCurDirectory = *itDir;
		QDir dir(strCurDirectory);
		dir.setFilter(QDir::Files);

		QStringList entries = dir.entryList(nameFilters, QDir::NoFilter, sortSpec);

		for (QStringList::size_type j = 0; j < entries.size(); j++){
			if (entries[j] == "." || entries[j] == ".."){
				continue;
			}
		
			QFileInfo ff( strCurDirectory, entries[j]);
			QString pat = ff.absoluteFilePath();
			entries[j] = pat;
		}
		
		for (QStringList::size_type i = 0; i < entries.size(); i++){
			QString e = entries[i];
			QFileInfo fileInfo(e);
			
			QString strName = fileInfo.fileName();
			
			if (strName == "." || strName == ".."){
				continue;
			}
			
			if (fileInfo.isDir()){
				continue;
			}
		
			e = QDir::convertSeparators(e);
			emit current(e);
			push_back(e);	
		}
	}

	return true;
}


bool CFileList::Create(const QString& root, bool rec, const QStringList& nameFilters, QDir::SortFlags sortSpec)
{
	return Create(QDir(root), rec, nameFilters, sortSpec);
}


} // namespace iqt
