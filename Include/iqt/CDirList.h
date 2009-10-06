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
		Constructs a list of directories in a given root directory \c root.
		All filters which are installed in \c root, are applied.
		\sa Create()
		\param	minRecursionDepth	minimal recursion depth.
		\param	maxRecursionDepth	maximal recursion depth, if negative no depth is specified.
	*/
	CDirList(	QObject* parent,
				const QDir& root,
				int minRecursionDepth = 0,
				int maxRecursionDepth = 0);

	/**
		\overload
		Several filters can be applied to the QDir object before call of this function.
		\param	minRecursionDepth	minimal recursion depth.
		\param	maxRecursionDepth	maximal recursion depth, if negative no depth is specified.
	*/
	bool Create(const QDir& root,
				int minRecursionDepth = 0,
				int maxRecursionDepth = 0,
				bool doAppend = false);

signals:
	/**
		This signal is emitted whenever the directory \c dir has been processed.
	*/
	void currentDir(const QString& dir);

protected:
	void DoSearch(
				const QDir& root,
				int minRecursionDepth,
				int maxRecursionDepth);
};


} // namespace iqt


#endif // !iqt_CDirList_included


