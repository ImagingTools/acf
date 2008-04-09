#ifndef iqt_CFileList_included
#define iqt_CFileList_included


#include <QObject>
#include <QStringList>
#include <QDir>
 

namespace iqt
{


/**
	This class serves for creation of the list of directories in a given root directory.
*/
class CDirList : public QObject, public QStringList
{
	Q_OBJECT

public:
	//! Constructs an empty list of directories.
	CDirList(QObject* parent = 0);

	//! Constructs a list of directories in a given root directory \c root. No filters are apllying.
	//! \sa Create()
	CDirList(const QString& root, bool isRecursive = true, QObject* parent = 0);

	//! Constructs a list of directories in a given root directory \c root.
	//! All filters which are installed in \c root, are applied.
	//! \sa Create()
	CDirList(const QDir& root, const bool isRecursive = true, QObject* parent = 0);

	//! Creates the list of all directories and all subdirectories (if the flag \c isRecursive equals \c TRUE ) found in the given 
	//! root directory \c root.  
	bool Create(const QString& root, bool isRecursive = true);

	//! \overload
	//! Several filters can be applied to the QDir object before call of this function.
	bool Create(const QDir& root, bool isRecursive = true);

protected:
	//! 
	void doSearch(QDir &, bool rec);

signals:
	//! This signal is emitted whenever the directory \c dir has been processed.
	void current(const QString & dir);
};


/**

	This class serves for creation of the list of files in a given root directory.
*/
class CFileList : public QObject, public QStringList
{
	Q_OBJECT

public:
	//! Constructs the empty object. To create the file list you have to use the Create() function.
	CFileList(QObject * parent = 0);
	
	//! \overload
	//! Several filters can be applied to the \c root before call of this function. 
	//! So you can do a separate filtering of files and directories
	bool Create(const QDir & root, bool isRecursive = true, const QStringList& nameFilters = QStringList(),  
		QDir::SortFlags sortSpec = QDir::Name | QDir::IgnoreCase);	

	//! Creates the list of files in a root directory \c root. 
	//! For the filtration of the file objects the name filter \c nameFilter is used.
	//! If \c isRecursive equals \c TRUE the function works isRecursive.
	bool Create(const QString & root, bool isRecursive = true, const QStringList& nameFilters = QStringList(),  
		QDir::SortFlags sortSpec = QDir::Name | QDir::IgnoreCase);	

signals:
	//! This signal is emitted whenever the file \c file has been processed.
	void current(const QString & file);
};


} // namespace iqt


#endif // !iqt_CFileList_included