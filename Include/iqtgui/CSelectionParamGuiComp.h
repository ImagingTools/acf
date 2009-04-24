#ifndef iqtgui_CSelectionParamGuiComp_included
#define iqtgui_CSelectionParamGuiComp_included


#include <QComboBox>

#include "istd/TPointerVector.h"

#include "iprm/ISelectionParam.h"

#include "iqtgui/TDesignerGuiObserverCompBase.h"

#include "iqtgui/Generated/ui_CSelectionParamGuiComp.h"


namespace iqtgui
{


class CSelectionParamGuiComp: public TDesignerGuiObserverCompBase<
			Ui::CSelectionParamGuiComp,
			iprm::ISelectionParam>
{
	Q_OBJECT

public:
	typedef TDesignerGuiObserverCompBase<
				Ui::CSelectionParamGuiComp,
				iprm::ISelectionParam> BaseClass;

	I_BEGIN_COMPONENT(CSelectionParamGuiComp)
	I_END_COMPONENT

	// reimplemented (imod::IModelEditor)
	virtual void UpdateModel() const;
	virtual void UpdateEditor(int updateFlags = 0);

protected slots:
	void OnSelectionChanged(int index);

	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void OnGuiModelShown();

private:
	istd::TPointerVector<QComboBox> m_comboBoxes;
};


} // namespace iqtgui


#endif // !iqtgui_CSelectionParamGuiComp_included


