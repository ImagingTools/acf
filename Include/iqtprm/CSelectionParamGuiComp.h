#ifndef iqtprm_CSelectionParamGuiComp_included
#define iqtprm_CSelectionParamGuiComp_included


// Qt includes
#include <QComboBox>


// ACF includes
#include "istd/TPointerVector.h"

#include "iprm/ISelectionParam.h"

#include "iqtgui/TDesignerGuiObserverCompBase.h"

#include "iqtgui/Generated/ui_CSelectionParamGuiComp.h"


namespace iqtprm
{


class CSelectionParamGuiComp: public iqtgui::TDesignerGuiObserverCompBase<
			Ui::CSelectionParamGuiComp,
			iprm::ISelectionParam>
{
	Q_OBJECT

public:
	typedef iqtgui::TDesignerGuiObserverCompBase<
				Ui::CSelectionParamGuiComp,
				iprm::ISelectionParam> BaseClass;

	I_BEGIN_COMPONENT(CSelectionParamGuiComp);
		I_ASSIGN(m_optionsLabelAttrPtr, "OptionsLabel", "Label for the options selector", false, "Select");
	I_END_COMPONENT;

	// reimplemented (imod::IModelEditor)
	virtual void UpdateModel() const;
	virtual void UpdateEditor(int updateFlags = 0);

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();
	virtual void OnGuiDestroyed();

protected Q_SLOTS:
	void OnSelectionChanged(int index);

private:
	I_ATTR(istd::CString, m_optionsLabelAttrPtr);

	istd::TPointerVector<QComboBox> m_comboBoxes;
};


} // namespace iqtprm


#endif // !iqtprm_CSelectionParamGuiComp_included


