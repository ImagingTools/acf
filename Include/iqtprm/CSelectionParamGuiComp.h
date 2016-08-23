#ifndef iqtprm_CSelectionParamGuiComp_included
#define iqtprm_CSelectionParamGuiComp_included


// Qt includes
#include <QtCore/QtGlobal>
#include <QtGui/QStandardItemModel>
#include <QtCore/QSortFilterProxyModel>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QCompleter>
#else
#include <QtGui/QLineEdit>
#include <QtGui/QComboBox>
#include <QtGui/QRadioButton>
#include <QtGui/QPushButton>
#include <QtGui/QCompleter>
#endif

// ACF includes
#include "istd/TPointerVector.h"

#include "imod/CMultiModelDispatcherBase.h"
#include "iprm/ISelectionParam.h"
#include "iqtgui/IIconProvider.h"
#include "iqtgui/TDesignerGuiObserverCompBase.h"
#include "GeneratedFiles/iqtprm/ui_CSelectionParamGuiComp.h"


namespace iqtprm
{


/**
	Simple editor for an exclusive option selection.
*/
class CSelectionParamGuiComp:
			public iqtgui::TDesignerGuiObserverCompBase<
						Ui::CSelectionParamGuiComp, iprm::ISelectionParam>,
			protected imod::CMultiModelDispatcherBase
{
	Q_OBJECT

public:
	typedef iqtgui::TDesignerGuiObserverCompBase<
				Ui::CSelectionParamGuiComp, iprm::ISelectionParam> BaseClass;

	/**
		GUI mode for selection representation
	*/
	enum UiMode
	{
		/**
			Combo box selection
		*/
		UM_COMBOBOX,

		/**
			Horizontal radio buttons group
		*/
		UM_RADIOBUTTON_HORIZONTAL,

		/**
			Vertical radio buttons group
		*/
		UM_RADIOBUTTON_VERTICAL
	};

	/**
		Command label position
	*/
	enum LabelPosition
	{
		/**
			Left from the selector
		*/
		LP_LEFT,

		/**
			On top of the selector
		*/
		LP_TOP
	};

	enum LabelAlignment
	{
		LA_LEFT_TOP, LA_CENTER_TOP, LA_RIGHT_TOP,
		LA_LEFT_CENTER, LA_CENTER, LA_RIGHT_CENTER,
		LA_LEFT_BOTTOM, LA_CENTER_BOTTOM, LA_RIGHT_BOTTOM
	};

	I_BEGIN_COMPONENT(CSelectionParamGuiComp);
		I_ASSIGN(m_optionsLabelAttrPtr, "OptionsLabel", "Command label for the options selector", false, "Select");
		I_ASSIGN(m_infoLabelAttrPtr, "InfoLabel", "Information label for the options selector", false, "Info");
		I_ASSIGN(m_infoIconProviderCompPtr, "InfoIconProvider", "Provider of the info icon", false, "InfoIconProvider");
		I_ASSIGN(m_iconSizeAttrPtr, "IconSize", "Size of the used icons", false, 32);
		I_ASSIGN(m_uiModeAttrPtr, "UiMode", "Selection representation mode.\n0 - Combo box\n1 - Horizontally layouted radio button group\n2 - Vertically layouted radio button group", true, UM_COMBOBOX);
		I_ASSIGN(m_labelPositionAttrPtr, "LabelPosition", "Selection label position.\n0 - Left from the selector,\n1 - On top of the selector", false, LP_LEFT);
		I_ASSIGN(m_labelAlignAttrPtr, "LabelAlignment", "Selection label alignment.\n0 - Left-Top,\n1 - Center-Top,\n2 - Right-Top,\n3 - Left-Center,\n4 - Center,\n5 - Right-Center,\n6 - Left-Bottom,\n7 - Center-Bottom,\n8 - Right-Bottom", false, LA_LEFT_CENTER);
		I_ASSIGN(m_labelWidthAttrPtr, "LabelWidth", "Fixed label width (in pixels)", false, 100);
		I_ASSIGN(m_disableWhenEmptyAttrPtr, "DisableWhenEmpty", "Disable the control when no Option is available", true, false);
		I_ASSIGN(m_noSelectionAllowedAttrPtr, "NoSelectionAllowed", "Allow the control to reset current index (i.e. set to -1)", true, false);
		I_ASSIGN(m_fillWithDisabledOptionsEnabledAttrPtr, "FillWithDisabledOptionsEnabled", "Fill combo box also with disabled options, if enabled", true, false);
		I_ASSIGN(m_useCompleterAttrPtr, "UseCompleter", "Enable completer for combo box", true, false);
	I_END_COMPONENT;

protected:
	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void OnGuiModelAttached();
	virtual void OnGuiModelDetached();
	virtual void UpdateGui(const istd::IChangeable::ChangeSet& changeSet);

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();
	virtual void OnGuiDestroyed();
	virtual void OnGuiShown();
	virtual void OnGuiRetranslate();

	// reimplemented (imod::CMultiModelDispatcherBase)
	virtual void OnModelChanged(int modelId, const istd::IChangeable::ChangeSet& changeSet);

protected Q_SLOTS:
	void OnSelectionChanged(int index);
	void OnRadioButtonSelectionChanged(bool isSelected);
	void OnResetButtonClicked();

private:
	void UpdateComboBoxesView();
	void UpdateRadioButtonView();
	void UpdateDescriptionFrame();
	void UpdateSelectorLabel();
	void ResetWidgets();
	iprm::ISelectionParam* GetActiveSubselection(const iprm::ISelectionParam* selectionPtr) const;
	void SetupInfoLabelIcon(QLabel& label);
	QPixmap GetInfoIcon() const;
	void UpdateCompletionModel();
	void ApplyFilterToProxyModel(const QString& filter) const;

private:
	I_ATTR(QString, m_optionsLabelAttrPtr);
	I_ATTR(QString, m_infoLabelAttrPtr);
	I_ATTR(int, m_uiModeAttrPtr);
	I_ATTR(int, m_labelPositionAttrPtr);
	I_ATTR(int, m_labelAlignAttrPtr);
	I_ATTR(int, m_labelWidthAttrPtr);
	I_REF(iqtgui::IIconProvider, m_infoIconProviderCompPtr);
	I_ATTR(int, m_iconSizeAttrPtr);
	I_ATTR(bool, m_disableWhenEmptyAttrPtr);
	I_ATTR(bool, m_noSelectionAllowedAttrPtr);
	I_ATTR(bool, m_fillWithDisabledOptionsEnabledAttrPtr);
	I_ATTR(bool, m_useCompleterAttrPtr);

	class RadioButtonWidget: public QFrame
	{
	public:
		typedef QFrame BaseClass;

		RadioButtonWidget(
					const QPixmap& infoIcon,
					const QString& optionName,
					const QString& optionDescription,
					QButtonGroup* buttonGroupPtr,
					QWidget& parentFrame);

		QRadioButton* GetRadioButton() const;

	private:
		QRadioButton* m_radioButtonPtr;
		QButtonGroup* m_buttonGroupPtr;
	};

	istd::TDelPtr<QLabel> m_selectorLabelPtr;
	istd::TPointerVector<QComboBox> m_comboBoxes;
	istd::TPointerVector<RadioButtonWidget> m_radioButtons;
	istd::TPointerVector<QButtonGroup> m_radioButtonGroups;
	istd::TDelPtr<QFrame> m_radioButtonFramePtr;
	QPushButton* m_resetButtonWidget;

	QStandardItemModel m_completionModel;
};


} // namespace iqtprm


#endif // !iqtprm_CSelectionParamGuiComp_included


