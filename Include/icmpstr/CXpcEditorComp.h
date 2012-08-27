#ifndef icmpstr_CXpcEditorComp_included
#define icmpstr_CXpcEditorComp_included


// QT includes
#include <QtGui/QStringListModel>
#include <QtCore/QDir>
#include <QtGui/QMenu>

// ACF includes
#include "idoc/IDocumentManager.h"
#include "iqtgui/TDesignerGuiObserverCompBase.h"
#include "icomp/CXpcModel.h"

#include "Generated/ui_CXpcEditorComp.h"


namespace icmpstr
{


/**
	Editor GUI component for the XpcModel.
 */
class CXpcEditorComp:
			public iqtgui::TDesignerGuiObserverCompBase<Ui::XpcEditorComp, icomp::CXpcModel>
{
	Q_OBJECT

public:
	typedef iqtgui::TDesignerGuiObserverCompBase<Ui::XpcEditorComp, icomp::CXpcModel> BaseClass;

	I_BEGIN_COMPONENT(CXpcEditorComp);
		I_ASSIGN(m_docMgrPtr, "DocumentManager", "An object to provide current document file name, to handle relative paths", true, "DocumentManager");
		I_ASSIGN(m_configFileTypeInfo, "ConfigFileType", "File type description for filtering configuration files, ie. XPC", false, "ConfigFileType");
		I_ASSIGN(m_packageFileTypeInfo, "PackageFileType", "File type description for filtering package files, ie. ARP", false, "PackageFileType");
		I_ASSIGN(m_registryFileTypeInfo, "RegistryFileType", "File type description for filtering registry files, ie. ARX", false, "RegistryFileType");	
	I_END_COMPONENT;

	CXpcEditorComp();

protected:
	// reimplemented (imod::IModelEditor)
	virtual void UpdateModel() const;

	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void UpdateGui(int updateFlags = 0);

	// reimplemented (CGuiComponentBase)
	virtual void OnGuiCreated();

private Q_SLOTS:
	void on_ConfigPathAddButton_clicked();
	void on_PackageDirAddButton_clicked();
	void on_PackagePathAddButton_clicked();
	void on_RegistryPathAddButton_clicked();
	void on_ConfigPathRemoveButton_clicked();
	void on_PackageDirRemoveButton_clicked();
	void on_PackagePathRemoveButton_clicked();
	void on_RegistryPathRemoveButton_clicked();
	void on_ConfigPathBrowseButton_clicked();
	void on_PackageDirBrowseButton_clicked();
	void on_PackagePathBrowseButton_clicked();
	void on_RegistryPathBrowseButton_clicked();

	// undo manager support
	void OnDoUpdateModel();
	void OnModelDataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight);

	void OnConfigPathViewSelected(const QItemSelection& selected, const QItemSelection& deselected);
	void OnPackageDirViewSelected(const QItemSelection& selected, const QItemSelection& deselected);
	void OnPackagePathViewSelected(const QItemSelection& selected, const QItemSelection& deselected);
	void OnRegistryPathViewSelected(const QItemSelection& selected, const QItemSelection& deselected);

	// navigation
	void on_ConfigPathView_doubleClicked(const QModelIndex& index);
	void on_RegistryPathView_doubleClicked(const QModelIndex& index);

	// update selected table row
	void on_ConfigPathEdit_editingFinished();
	void on_PackageDirEdit_editingFinished();
	void on_PackagePathEdit_editingFinished();
	void on_RegistryPathEdit_editingFinished();

	// maintain selection on lost focus
	void on_ConfigPathEdit_selectionChanged();
	void on_PackageDirEdit_selectionChanged();
	void on_PackagePathEdit_selectionChanged();
	void on_RegistryPathEdit_selectionChanged();

	void on_ConfigPathEdit_cursorPositionChanged(int oldPosition, int newPosition);
	void on_PackageDirEdit_cursorPositionChanged(int oldPosition, int newPosition);
	void on_PackagePathEdit_cursorPositionChanged(int oldPosition, int newPosition);
	void on_RegistryPathEdit_cursorPositionChanged(int oldPosition, int newPosition);

	void OnInsertVariable(QAction* action);

Q_SIGNALS:
	void DoLateUpdateModel();

private:
	enum Section
	{
		S_CONFIG_PATH = 0,
		S_PACKAGE_DIR,
		S_PACKAGE_PATH,
		S_REGISTRY_PATH,
		S_FIRST = S_CONFIG_PATH,
		S_LAST = S_REGISTRY_PATH
	};

	/**
		Retrieve strings from a table.
	*/
	QStringList GetStringList(int tableIdx) const;

	/**
		Fill a table with strings (replaces existing content)
		\param table table index in m_tablePointer
	*/
	void SetStringList(int tableIdx, QStringList);

	QDir GetCurrentDocumentDir();

	/**
		Open a file dialog using a directory from currently selected file path
		If a user picks a file, its path replaces currently selected line.
		\param lineEdit a line edit to take path from and fill with new path
		\param SelectDirectories whether to select directories instead of plain files
		\param typeInfo ACF file info structure to configure file filter
	*/
	void PickFileNameForLineEdit(
				QLineEdit& lineEdit,
				bool SelectDirectories = false,
				iser::IFileTypeInfo* typeInfo = NULL);

	// static methods
	/**
		A hack to fix a known issue that QLineEdit clears selection when it loses focus.
	*/
	static void MaintainLineEditSelection(QLineEdit* lineEdit);

	/** MVC models to handle data lists for tables */
	QStringListModel* m_guiModel[S_LAST + 1];

	QMenu m_startVariableMenus[S_LAST + 1];
	QMenu m_variableMenus[S_LAST + 1];

	/**
		Fills m_variablesMenu with variable names depending on cursor position
		If a variable is an absolute path, it can only be inserted at the 
		beginning, ie. before a relative path or instead of part of an absolute 
		path (which requires a selection). If a variable is a relative path, 
		it can be inserted after an absolute path. Some environment variables 
		cannot be used whatsoever because of invalid characters.
	*/
	void SetupVariablesMenu(bool isBegin, int sectionIndex, QMenu& menu);

	I_REF(idoc::IDocumentManager, m_docMgrPtr);
	I_REF(iser::IFileTypeInfo, m_configFileTypeInfo);
	I_REF(iser::IFileTypeInfo, m_packageFileTypeInfo);
	I_REF(iser::IFileTypeInfo, m_registryFileTypeInfo);
};


} // namespace icmpstr


#endif // !icmpstr_CXpcEditorComp_included


