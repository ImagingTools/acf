#include "iqt/CDirList.h"

#include <QFileInfo>


namespace iqt
{


CDirList::CDirList(QObject* parent) 
	:QObject(parent)
{
}


CDirList::CDirList( const QString& root, bool isRecursive, QObject* parent) 
	:QObject(parent)
{
	Create(root, isRecursive);
}


CDirList::CDirList( const QDir& root, bool isRecursive, QObject* parent) 
	:QObject(parent)
{
	Create(root, isRecursive);
}



bool CDirList::Create(const QString& rootDirectory, bool isRecursive)
{	
	clear();

	QFileInfo fileInfo(rootDirectory);
	if (!fileInfo.isDir())
		return false;
	
	push_back(rootDirectory);
	
	QDir inputDirectory = QDir(rootDirectory);

	DoSearch(inputDirectory, isRecursive);

	return true;
}


bool CDirList::Create(const QDir& root, bool isRecursive)
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

	DoSearch(temp, isRecursive);

	return true;
}


void CDirList::DoSearch(QDir& root, bool isRecursive)
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

		if (isRecursive){
			QDir temp(entry);
			temp.setNameFilters(root.nameFilters());
			temp.setSorting( root.sorting() );
			temp.setFilter( root.filter() );
			
			DoSearch(temp, isRecursive);
		}
	}
}


} // namespace iqt

