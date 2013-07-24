#ifndef ifilegui_CFileTreeViewGuiComp_included
#define ifilegui_CFileTreeViewGuiComp_included


// Qt includes
#include <QtCore/QDir>
#include <QtCore/QTimer>
#include <QtCore/QElapsedTimer>
#include <QtCore/QThread>
#include <QtCore/QMutexLocker>
#include <QtGui/QStandardItemModel>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QFileIconProvider>
#else
#include <QtGui/QFileIconProvider>
#endif


// ACF includes
#include "ifile/IFileNameParam.h"
#include "ifile/IFileTypeInfo.h"
#include "ifile/CFileNameParamComp.h"

#include "ibase/IQtItemModelProvider.h"
#include "imod/CMultiModelDispatcherBase.h"

#include "iqtgui/TDesignerGuiObserverCompBase.h"

#include "ifilegui/Generated/ui_CFileTreeViewGuiComp.h"


namespace ifilegui
{


/**
	File system explorer-like component.
	Observes a directory which is to be the root of the tree view.
	Provides on-demand refresh of the view (i.e. via button click).
*/
class CFileTreeViewGuiComp:
			public iqtgui::TDesignerGuiObserverCompBase<
						Ui::CFileTreeViewGuiComp, ifile::IFileNameParam>,
			public imod::CMultiModelDispatcherBase
{
	Q_OBJECT
public:
	typedef iqtgui::TDesignerGuiObserverCompBase<
				Ui::CFileTreeViewGuiComp, ifile::IFileNameParam> BaseClass;

	I_BEGIN_COMPONENT(CFileTreeViewGuiComp);
		I_REGISTER_SUBELEMENT(CurrentFile);
		I_REGISTER_SUBELEMENT_INTERFACE(CurrentFile, ifile::IFileNameParam, ExtractCurrentFile);
		I_REGISTER_SUBELEMENT_INTERFACE(CurrentFile, imod::IModel, ExtractCurrentFile);
		I_REGISTER_SUBELEMENT_INTERFACE(CurrentFile, istd::IChangeable, ExtractCurrentFile);
		I_REGISTER_SUBELEMENT_INTERFACE(CurrentFile, iser::ISerializable, ExtractCurrentFile);
		I_ASSIGN(m_fileTypeInfoCompPtr, "FileTypeInfo", "File type info used to create file filters", false, "FileTypeInfo");
		I_ASSIGN_MULTI_0(m_filtersAttrPtr, "Filters", "List of filters if no FileTypeInfo is specified", false);
		I_ASSIGN(m_noEmptyDirsAttrPtr, "NoEmptyFolders", "Recursively skip folder which containing no files", true, true);
	I_END_COMPONENT;

	enum DataRoles{
		DR_PATH = Qt::UserRole + 1,
		DR_ISDIR
	};

protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();

	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void UpdateGui(int updateFlags = 0);

	// reimplemented (imod::CMultiModelDispatcherBase)
	virtual void OnModelChanged(int modelId, int changeFlags, istd::IPolymorphic* updateParamsPtr);

private Q_SLOTS:
	void OnSelectionChanged(const QItemSelection& selected, const QItemSelection& deselected);
	void on_Refresh_clicked();
	void OnTreeModelUpdated();
	void on_FilterText_textChanged(QString filterText);

private:
	void RebuildTreeModel();
	void DoTreeModelUpdate();
	void UpdateCurrentSelection();
	void UpdateChildItems(QStandardItem* itemPtr);

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
	class CurrentFile: public ifile::CFileNameParamComp
	{
	public:
		// reimplemented (ifile::IFileNameParam)
		virtual int GetPathType() const
		{
			return PT_FILE;
		}
	};

	imod::TModelWrap<CurrentFile> m_currentFile;

	// static template methods for subelement access
	template <class InterfaceType>
	static InterfaceType* ExtractCurrentFile(CFileTreeViewGuiComp& component)
	{
		return &component.m_currentFile;
	}


	class InternalThread: public QThread
	{
	public:
		InternalThread(CFileTreeViewGuiComp* parentPtr): QThread(parentPtr),
			m_parentPtr(parentPtr)
		{
		}

		// reimplemented (QThread)
		virtual void run()
		{
			m_parentPtr->DoTreeModelUpdate();
		}

	private:
		CFileTreeViewGuiComp* m_parentPtr;
	};

	friend class InternalThread;
	InternalThread* m_internalThreadPtr;

	QMutex m_lock;


	bool m_fileModelUpdateAllowed;
	int m_filesCount;
	int m_dirsCount;

	mutable QStandardItemModel m_itemModel;
	QFileIconProvider m_iconProvider;

	mutable QMap<QString, QIcon> m_extToIconMap;

	QRegExp m_filterMatcher;
	QString m_userFilter;
	QTimer m_filterTimer;
	QElapsedTimer m_performanceTimer;

	I_REF(ifile::IFileTypeInfo, m_fileTypeInfoCompPtr);
	I_MULTIATTR(QString, m_filtersAttrPtr);
	I_ATTR(bool, m_noEmptyDirsAttrPtr);
};


} // namespace ifilegui


#endif // !ifilegui_CFileTreeProviderGuiComp_included


