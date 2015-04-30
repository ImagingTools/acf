#pragma once


// Qt includes
#include <QtCore/QtGlobal>

#if QT_VERSION >= 0x050000
#include <QtWidgets/QMenu>
#include <QtWidgets/QStyledItemDelegate>
#include <QtWidgets/QDialog>
#else
#include <QtGui/QMenu>
#include <QtGui/QStyledItemDelegate>
#include <QtGui/QPainter>
#include <QtGui/QDialog>
#endif

// ACF includes
#include <iqtgui/IIconProvider.h>
#include <iprm/IParamsManager.h>
#include <iqtgui/TDesignerGuiObserverCompBase.h>
#include <GeneratedFiles/iqtprm/ui_CExtParamsManagerGuiComp.h>
#include <GeneratedFiles/iqtprm/ui_CElementEditorGui.h>


namespace iqtprm
{


/**
	UI manager of the parameter sets.
	With help of this UI-editor the parameter sets can be added, removed, copied and edited.
	The editor is realized as observer of the iprm::IParamsManager data model interface.
*/
class CExtParamsManagerGuiComp: 
			public iqtgui::TDesignerGuiObserverCompBase<
						Ui::CExtParamsManagerGuiComp, iprm::IParamsManager>
{
	Q_OBJECT
public:
	typedef iqtgui::TDesignerGuiObserverCompBase<
				Ui::CExtParamsManagerGuiComp, iprm::IParamsManager> BaseClass;

	I_BEGIN_COMPONENT(CExtParamsManagerGuiComp);
		I_ASSIGN_MULTI_0(m_parameterEditorIdsAttrPtr, "ParameterEditorIds", "List of parameter editors type IDs used for showing the job's parameter for the corresponding worker types", false);
		I_ASSIGN_MULTI_0(m_parameterGuisCompPtr, "ParameterEditors", "List of parameter editors used for showing the job's parameter for the corresponding worker types", false);
		I_ASSIGN_TO(m_parameterObserversCompPtr, m_parameterGuisCompPtr, true);
		I_ASSIGN_TO(m_parameterEditorsCompPtr, m_parameterGuisCompPtr, true);
	I_END_COMPONENT;

protected:
	int GetParameterTypeIndexById(const QByteArray& parameterTypeId) const;
	void UpdateCommands();

	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void UpdateGui(const istd::IChangeable::ChangeSet& changeSet);

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();

protected Q_SLOTS:
	void on_AddButton_clicked();
	void on_CloneButton_clicked();
	void on_EditButton_clicked();
	void on_RemoveButton_clicked();
	void on_ReloadButton_clicked();
	void on_ElementList_doubleClicked(const QModelIndex &index);
	void on_ElementList_itemSelectionChanged();

protected:

	/**
		Data roles used in the list view items.
	*/
	enum DataRoles
	{
		/**
			Parameter set description
		*/
		DR_DESCRIPTION = Qt::UserRole + 1,

		/**
			Parameter type ID.
		*/
		DR_PARAMETER_TYPE,

		/**
			Index of the parameter set in the list.
		*/
		DR_INDEX,

		/**
			ID of the parameter set
		*/
		DR_UUID
	};

	/**
		Delegate implementation for customization of the parameter set item view in the parameter set list.
	*/
	class CElementItemDelegate: public QStyledItemDelegate
	{
	public:
		// reimplemented (QItemDelegate)
		virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
		virtual QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
	};

	/**
		Edit dialog for the parameter set name and description.
		The dialog will be started by double click on the parameter set item or by triggering of the Edit-button.
	*/
	class CElementEditorDialog: public QDialog
	{
	public:
		CElementEditorDialog();
		virtual ~CElementEditorDialog();

		/**
			Start the dialog with the predefined name and description parameters. 
		*/
		int Execute(QString &name, QString &description, iqtgui::IGuiObject* paramsGuiPtr);

	private:
		Ui::CElementEditorGui m_ui;

		iqtgui::IGuiObject* m_guiObjectPtr;
	};

	/**
		Internal structure used for binding of model editor, observer and gui editor for the parameter set in a one object
	*/
	struct ParameterEditor
	{
		ParameterEditor()
			:observerPtr(NULL),
			editorPtr(NULL),
			guiPtr(NULL)
		{
		}
		imod::IObserver* observerPtr;
		imod::IModelEditor* editorPtr;
		iqtgui::IGuiObject* guiPtr;
	};

	/**
		Mapping between the parameter type ID and corresponding editor.
	*/
	typedef QMap<QByteArray, ParameterEditor> ParameterEditorMap;
	ParameterEditorMap m_parameterEditorMap;

	/**
		List of parameter editors type IDs used for showing the job's parameter for the corresponding parameter set type.
	*/
	I_MULTIATTR(QByteArray, m_parameterEditorIdsAttrPtr);

	/**
		List of parameter editors used for showing the parameter for the corresponding parameter set type.
	*/
	I_MULTIREF(iqtgui::IGuiObject, m_parameterGuisCompPtr);
	I_MULTIREF(imod::IObserver, m_parameterObserversCompPtr);
	I_MULTIREF(imod::IModelEditor, m_parameterEditorsCompPtr);

	QByteArray m_lastSelectedParameterSetId;
};


} // namespace iqtprm




