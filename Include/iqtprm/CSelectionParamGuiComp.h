#ifndef iqtprm_CSelectionParamGuiComp_included
#define iqtprm_CSelectionParamGuiComp_included


// Qt includes
#include <QtGui/QComboBox>
#include <QtGui/QRadioButton>

// ACF includes
#include "istd/TPointerVector.h"

#include "imod/CMultiModelDispatcherBase.h"

#include "iprm/ISelectionParam.h"
#include "iqtgui/IIconProvider.h"

#include "iqtgui/TDesignerGuiObserverCompBase.h"

#include "iqtprm/Generated/ui_CSelectionParamGuiComp.h"


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
		I_ASSIGN(m_uiModeAttrPtr, "UiMode", "Selection representation mode.\n0 - Combo box,\n1 - Horizonal layouted radio button group\n2 - Vertical layouted radio button group", true, UM_COMBOBOX);
		I_ASSIGN(m_labelPositionAttrPtr, "LabelPosition", "Selection label position.\n0 - Left from the selector,\n1 - On top of the selector", false, LP_LEFT);
		I_ASSIGN(m_labelAlignAttrPtr, "LabelAlignment", "Selection label alignment.\n0 - Left-Top,\n1 - Center-Top,\n2 - Right-Top,\n3 - Left-Center,\n4 - Center,\n5 - Right-Center,\n6 - Left-Bottom,\n7 - Center-Bottom,\n8 - Right-Bottom", false, LA_LEFT_CENTER);
		I_ASSIGN(m_labelWidthAttrPtr, "LabelWidth", "Fixed label width (in pixels)", false, 100);
	I_END_COMPONENT;

	// reimplemented (imod::IModelEditor)
	virtual void UpdateModel() const;

protected:
	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void OnGuiModelAttached();
	virtual void OnGuiModelDetached();
	virtual void UpdateGui(int updateFlags = 0);

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();
	virtual void OnGuiDestroyed();
	virtual void OnGuiShown();

	// reimplemented (imod::CMultiModelDispatcherBase)
	virtual void OnModelChanged(int modelId, int changeFlags, istd::IPolymorphic* updateParamsPtr);

protected Q_SLOTS:
	void OnSelectionChanged(int index);
	void OnRadioButtonSelectionChanged(bool isSelected);

private:
	void UpdateComboBoxesView();
	void UpdateRadioButtonView();
	void UpdateDescriptionFrame();

private:
	I_ATTR(QString, m_optionsLabelAttrPtr);
	I_ATTR(QString, m_infoLabelAttrPtr);
	I_ATTR(int, m_uiModeAttrPtr);
	I_ATTR(int, m_labelPositionAttrPtr);
	I_ATTR(int, m_labelAlignAttrPtr);
	I_ATTR(int, m_labelWidthAttrPtr);
	I_REF(iqtgui::IIconProvider, m_infoIconProviderCompPtr);
	I_ATTR(int, m_iconSizeAttrPtr);

	istd::TPointerVector<QComboBox> m_comboBoxes;
	istd::TPointerVector<QRadioButton> m_radioButtons;
	istd::TDelPtr<QFrame> m_radioButtonFramePtr;
};


} // namespace iqtprm


#endif // !iqtprm_CSelectionParamGuiComp_included


