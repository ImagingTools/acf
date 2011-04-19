#ifndef iqtgui_CFileSystemExplorerGuiComp_included
#define iqtgui_CFileSystemExplorerGuiComp_included


// Qt includes
#include <QTreeWidget>
#include <QFileSystemModel>
#include <QItemSelectionModel>
#include <QFileIconProvider>


// ACF includes
#include "iser/IFileTypeInfo.h"
#include "iprm/IFileNameParam.h"

#include "iqtgui/TDesignerGuiObserverCompBase.h"
#include "iqtgui/CExtLineEdit.h"

#include "iqtgui/Generated/ui_CFileSystemExplorerGuiComp.h"


namespace iqtgui
{


/**
	UI component for visualization of file system contents.

	CFileSystemExplorerGuiComp is implemented as an observer of IFileNameParam.
	Connected to model, this component will show the file system item, provided by the data model.
	Model data will be changed, if the the user selects an supported file item in the file system view.
*/
class CFileSystemExplorerGuiComp:
			public iqtgui::TDesignerGuiObserverCompBase<
						Ui::CFileSystemExplorerGuiComp, iprm::IFileNameParam>
{
	Q_OBJECT
public:
	typedef iqtgui::TDesignerGuiObserverCompBase<
				Ui::CFileSystemExplorerGuiComp, iprm::IFileNameParam> BaseClass;
	
	I_BEGIN_COMPONENT(CFileSystemExplorerGuiComp);
		I_ASSIGN(m_filterInfoCompPtr, "FilterInfo", "Provides information about supported files used to filter shown files", false, "FilterInfo");
		I_ASSIGN(m_showUserFilterAttrPtr, "ShowUserFilter", "If activated user filter line will be shown", true, false);
		I_ASSIGN(m_showFileTypeAttrPtr, "ShowFileType", "Show file type description", true, false);
		I_ASSIGN(m_showFileModificationTimeAttrPtr, "ShowFileModificationTime", "If activated file modification time stamp will be shown for each file", true, false);
		I_ASSIGN(m_useSystemDecoratedIconsAttrPtr, "UseSystemDecoratedIcons", "If enabled, system decorated icons are used", false, false);
		I_ASSIGN(m_allowOpenFileAttrPtr, "AllowOpenFile", "If enabled, the file will be opened by operation system shell", false, false);
	I_END_COMPONENT;

public:
	// reimplemented (imod::IModelEditor)
	virtual void UpdateModel() const;
	virtual void UpdateEditor(int updateFlags = 0);

protected:
	/**
		Get default list of filters.
		This list will be created using component parameters.
	*/
	QStringList GetDefaultFilters() const;

	// reimplemented (CGuiComponentBase)
	virtual void OnGuiCreated();

private Q_SLOTS:
	void OnFilterChanged();
	void OnSelectionChanged(const QItemSelection& selected, const QItemSelection& deselected);
	void OnDoubleClicked(const QModelIndex& index);

private:
	void InvalidateFileSystemModel(const QString& currentFilePath);

private:
	I_REF(iser::IFileTypeInfo, m_filterInfoCompPtr);
	I_ATTR(bool, m_showUserFilterAttrPtr);
	I_ATTR(bool, m_showFileTypeAttrPtr);
	I_ATTR(bool, m_showFileModificationTimeAttrPtr);
	I_ATTR(bool, m_useSystemDecoratedIconsAttrPtr);
	I_ATTR(bool, m_allowOpenFileAttrPtr);

	class FileIconProvider: public QFileIconProvider
	{
	public:
		typedef QFileIconProvider BaseClass;

		// reimplemented (QFileIconProvider)
		virtual QIcon icon(const QFileInfo& fileInfo) const;
	};

	iqtgui::CExtLineEdit* m_filterEdit;

	FileIconProvider m_fileIconProvider;

	QFileSystemModel m_fileSystemModel;
};


} // namespace iqtgui


#endif // !iqtgui_CFileSystemExplorerGuiComp_included


