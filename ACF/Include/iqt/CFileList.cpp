#include <QApplication>
#include <QFileInfo> 

#include "CFileList.h"


namespace iqt
{


CDirList::CDirList(QObject* parent) 
	:QObject(parent)
{
}


CDirList::CDirList( const QString& root, bool recursive, QObject* parent) 
	:QObject(parent)
{
	Create(root, recursive);
}


CDirList::CDirList( const QDir& root, bool recursive, QObject* parent) 
	:QObject(parent)
{
	Create(root, recursive);
}



bool CDirList::Create(const QString& rootDirectory, bool isRecursive)
{	
	clear();

	QFileInfo fileInfo(rootDirectory);
	if (!fileInfo.isDir())
		return false;
	
	push_back(rootDirectory);
	
	QDir inputDirectory = QDir(rootDirectory);

	doSearch(inputDirectory, isRecursive);

	return true;
}


bool CDirList::Create(const QDir& root, bool rec)
{	
	clear();
	QString rootPath = root.canonicalPath();

	QFileInfo fileInfo( rootPath );
	if (!fileInfo.isDir()){
		return false;
	}
	
	if (rootPath.isEmpty()){
		return false;
	}

	push_back(rootPath);
	
	QDir temp = root;
	doSearch(temp, rec);

	return true;
}


void CDirList::doSearch( QDir & root, bool recursive)
{
	QStringList entries = root.entryList( QDir::Dirs );
	
	for (QStringList::size_type j = 0; j < entries.size(); j++ ){
		QString entry = entries[j];
		if( entry == "." || entry == "..")
			continue;
		
		QFileInfo fi(root, entry );
		entry = fi.absoluteFilePath();
		entry = QDir::convertSeparators(entry);

		emit current(entry);
		push_back(entry);

		if (recursive){
			QDir temp(entry);
			temp.setNameFilters(root.nameFilters());
			temp.setSorting( root.sorting() );
			temp.setFilter( root.filter() );
			doSearch(temp, recursive);
		}
	}
}


CFileList::CFileList(QObject* parent) 
	:QObject(parent)
{
}


bool CFileList::Create(const QDir & root, bool recursive, const QStringList& nameFilters, QDir::SortFlags sortSpec)

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

} // namespace kusoft
