#ifndef iqt_CFileList_included
#define iqt_CFileList_included


#include "iqt/iqt.h"


#include <QObject>
#include <QStringList>
#include <QDir>
 

namespace iqt
{


/**
	This class serves for creation of the list of files in a given root directory.
*/
class CFileList : public QObject, public QStringList
{
	Q_OBJECT

public:
	/**
		Constructs the empty object. 
		To create the file list you have to use the Create() function.
	*/
	CFileList(QObject* parent = 0);
	
	/**
		Creates the list of files in a root directory \c root. 
		Several filters can be applied to the \c root before call of this function. 
		So you can do a separate filtering of files and directories.
		\param	minRecursionDepth	minimal recursion depth.
		\param	maxRecursionDepth	maximal recursion depth, if negative no depth is specified.
	*/
	bool Create(const QDir& root,
				int minRecursionDepth = 0,
				int maxRecursionDepth = 0,
				const QStringList& nameFilters = QStringList(),  
				QDir::SortFlags sortSpec = QDir::Name | QDir::IgnoreCase);	

signals:
	/**
		This signal is emitted whenever the file \c file has been processed.
	*/
	void currentFile(const QString& file);
};


} // namespace iqt


#endif // !iqt_CFileList_included


