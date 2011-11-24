#ifndef iqtprm_COptionsManagerGuiComp_included
#define iqtprm_COptionsManagerGuiComp_included


// Qt includes
#include <QComboBox>


// ACF includes
#include "istd/TPointerVector.h"

#include "imod/CMultiModelDispatcherBase.h"

#include "iprm/ISelectionParam.h"
#include "iprm/IOptionsManager.h"

#include "iqtgui/TDesignerGuiObserverCompBase.h"

#include "iqtprm/Generated/ui_COptionsManagerGuiComp.h"


namespace iqtprm
{


/**
	Simple editor for an exclusive option selection.
*/
class COptionsManagerGuiComp:
			public iqtgui::TDesignerGuiObserverCompBase<
						Ui::COptionsManagerGuiComp, iprm::ISelectionParam>,
			protected imod::CMultiModelDispatcherBase
{
	Q_OBJECT

public:
	typedef iqtgui::TDesignerGuiObserverCompBase<
				Ui::COptionsManagerGuiComp, iprm::ISelectionParam> BaseClass;

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

	I_BEGIN_COMPONENT(COptionsManagerGuiComp);
		I_ASSIGN(m_optionsLabelAttrPtr, "OptionsLabel", "Command label for the options selector", false, "Select");
		I_ASSIGN(m_infoLabelAttrPtr, "InfoLabel", "Information label for the options selector", false, "Info");
		I_ASSIGN(m_labelPositionAttrPtr, "LabelPosition", "Selection label position. 0 - Left from the selector,\n1 - On top of the selector", false, LP_LEFT);
		I_ASSIGN(m_optionsManagerCompPtr, "OptionsManager", "Manager for editing of option list", false, "OptionsManager");
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
	void on_EditEnableButton_toggled(bool toggled);
	void OnEditingFinished();

private:
	void UpdateComboBoxesView();
	void UpdateDescriptionFrame();

private:
	I_ATTR(istd::CString, m_optionsLabelAttrPtr);
	I_ATTR(istd::CString, m_infoLabelAttrPtr);
	I_ATTR(int, m_labelPositionAttrPtr);

	I_REF(iprm::IOptionsManager, m_optionsManagerCompPtr);
};


} // namespace iqtprm


#endif // !iqtprm_COptionsManagerGuiComp_included


