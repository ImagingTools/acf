#ifndef iqt_CDirList_included
#define iqt_CDirList_included


#include "iqt/iqt.h"


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
	/**
		Constructs an empty list of directories.
	*/
	CDirList(QObject* parent = 0);

	/**
		Constructs a list of directories in a given root directory \c root. No filters are apllying.
		\sa Create()
	*/
	CDirList(const QString& root, bool isRecursive = true, QObject* parent = 0);

	/**
		Constructs a list of directories in a given root directory \c root.
		All filters which are installed in \c root, are applied.
		\sa Create()
	*/
	CDirList(const QDir& root, const bool isRecursive = true, QObject* parent = 0);

	/**
		Creates the list of all directories and all subdirectories (if the flag \c isRecursive equals \c TRUE ) found in the given 
		root directory \c root.  
	*/
	bool Create(const QString& root, bool isRecursive = true);

	/**
		\overload
		Several filters can be applied to the QDir object before call of this function.
	*/
	bool Create(const QDir& root, bool isRecursive = true);

protected:
	void DoSearch(QDir &, bool rec);

signals:
	/**
		This signal is emitted whenever the directory \c dir has been processed.
	*/
	void current(const QString & dir);
};


} // namespace iqt


#endif // !iqt_CDirList_included


