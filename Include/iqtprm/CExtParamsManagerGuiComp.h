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
#include "iqtgui/TDesignerGuiObserverCompBase.h"
#include "iprm/IParamsManager.h"
#include "GeneratedFiles/iqtprm/ui_CExtParamsManagerGuiComp.h"
#include "GeneratedFiles/iqtprm/ui_CElementEditorGui.h"


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
		I_ASSIGN_MULTI_0(_parameterEditorIdsAttrPtr, "ParameterEditorIds", "List of parameter editors type IDs used for showing the job's parameter for the corresponding worker types", false);
		I_ASSIGN_MULTI_0(_parameterGuisCompPtr, "ParameterEditors", "List of parameter editors used for showing the job's parameter for the corresponding worker types", false);
		I_ASSIGN_TO(_parameterObserversCompPtr, _parameterGuisCompPtr, true);
		I_ASSIGN_TO(_parameterEditorsCompPtr, _parameterGuisCompPtr, true);
	I_END_COMPONENT;

protected:
	int GetParameterTypeIndexById(const QByteArray& parameterTypeId) const;

	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void OnGuiCreated();
	virtual void UpdateGui(const istd::IChangeable::ChangeSet& changeSet);

protected Q_SLOTS:
	void on_AddButton_clicked();
	void on_ConfigureButton_clicked();
	void on_CloneButton_clicked();
	void on_EditButton_clicked();
	void on_RemoveButton_clicked();
	void on_ElementList_currentRowChanged(int);
	void on_ElementList_doubleClicked(const QModelIndex &index);

protected:

	/**
		Data roles used in the list view items.
	*/
	enum DataRoles
	{
		/**
			Description of the parameter set.
		*/
		DR_DESCRIPTION_ROLE = Qt::UserRole + 1,

		/**
			Version of the parameter set.
		*/
		DR_VERSION_ROLE,

		/**
			Index of the parameter type.
		*/
		DR_PARAMETER_TYPE_ROLE,

		/**
			Index of the parameter set in the list.
		*/
		DR_INDEX_ROLE
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

		/**
			Start the dialog with the predefined name and description parameters. 
		*/
		bool Execute(QString &name, QString &description);

	private:
		Ui::CElementEditorGui _ui;
	};

	/**
		Internal structure used for binding of model editor, observer and gui editor for the job parameters in a one object
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
		Mapping between the worker type ID and corresponding editor.
	*/
	typedef QMap<QByteArray, ParameterEditor> ParameterEditorMap;
	ParameterEditorMap _parameterEditorMap;

	/**
		List of parameter editors type IDs used for showing the job's parameter for the corresponding worker types.
	*/
	I_MULTIATTR(QByteArray, _parameterEditorIdsAttrPtr);

	/**
		List of parameter editors used for showing the job's parameter for the corresponding worker types.
	*/
	I_MULTIREF(iqtgui::IGuiObject, _parameterGuisCompPtr);
	I_MULTIREF(imod::IObserver, _parameterObserversCompPtr);
	I_MULTIREF(imod::IModelEditor, _parameterEditorsCompPtr);
};


} // namespace iqtprm




