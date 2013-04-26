#ifndef ifile_CFileTreeProviderComp_included
#define ifile_CFileTreeProviderComp_included


// Qt includes
#include <QtCore/QDir>
#include <QtGui/QStandardItemModel>
#include <QtGui/QFileIconProvider>

// ACF includes
#include "imod/IModel.h"
#include "imod/CSingleModelObserverBase.h"
#include "icomp/CComponentBase.h"
#include "ibase/IQtItemModelProvider.h"
#include "ifile/IFileNameParam.h"
#include "ifile/IFileTypeInfo.h"


namespace ifile
{


/**
	Provide tree of files using recursive search of some file directory.
*/
class CFileTreeProviderComp:
	public QObject,
	public icomp::CComponentBase,
	virtual public ibase::IQtItemModelProvider,
	virtual public istd::IChangeable,
	protected imod::CSingleModelObserverBase
{
	Q_OBJECT
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CFileTreeProviderComp);
		I_REGISTER_INTERFACE(istd::IChangeable);
		I_REGISTER_INTERFACE(ibase::IQtItemModelProvider);
		I_ASSIGN(m_dirParamCompPtr, "DirParam", "Parameter storing root directory", true, "DirParam");
		I_ASSIGN(m_dirParamModelCompPtr, "DirParam", "Parameter storing root directory", false, "DirParam");
		I_ASSIGN(m_fileTypeInfoCompPtr, "FileTypeInfo", "File type info used to create file filters", false, "FileTypeInfo");
		I_ASSIGN_MULTI_0(m_filtersAttrPtr, "Filters", "List of filters if no loader is specified", false);
	I_END_COMPONENT;

	enum DataRoles{
		DR_PATH = Qt::UserRole + 1
	};

	// reimplemented (ibase::IQtItemModelProvider)
	virtual const QAbstractItemModel* GetItemModel() const;

protected:
	// reimplemented (imod::CSingleModelObserverBase)
	virtual void OnUpdate(int updateFlags, istd::IPolymorphic* updateParamsPtr);

	// pseudo-reimplemented (istd::IChangeable)
	virtual void OnEndChanges(int changeFlags, istd::IPolymorphic* changeParamsPtr);

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

private:
	/**
	Creates the list of files in a root directory \c root.
	Several filters can be applied to the \c root before call of this function.
	So you can do a separate filtering of files and directories.
	*/
	bool CreateFileList(const QDir& root,
		const QStringList& nameFilters,
		QDir::SortFlags sortSpec,
		QStandardItem* parentItemPtr);

	/**
	Several filters can be applied to the QDir object before call of this function.
	*/
	bool CreateDirectoryList(
		const QDir& root,
		const QStringList& nameFilters,
		QDir::SortFlags sortSpec,
		QStandardItem* parentItemPtr);

	void EnumerateDirectory(
		const QDir& root,
		const QStringList& nameFilters,
		QDir::SortFlags sortSpec,
		QStandardItem* parentItemPtr);

private:
	I_REF(ifile::IFileNameParam, m_dirParamCompPtr);
	I_REF(imod::IModel, m_dirParamModelCompPtr);
	I_REF(ifile::IFileTypeInfo, m_fileTypeInfoCompPtr);
	I_MULTIATTR(QString, m_filtersAttrPtr);

	QStandardItemModel m_itemModel;
	QFileIconProvider m_iconProvider;
};


} // namespace ifile


#endif //!ifile_CFileTreeProviderComp_included


