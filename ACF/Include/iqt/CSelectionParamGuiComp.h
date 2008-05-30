#ifndef iqt_CSelectionParamGuiComp_included
#define iqt_CSelectionParamGuiComp_included


#include "iprm/ISelectionParam.h"

#include "iqt/TDesignerGuiObserverCompBase.h"

#include "iqt/Generated/ui_CSelectionParamGuiComp.h"


namespace iqt
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
	virtual void UpdateEditor();

protected slots:
	void on_SelectionCB_currentIndexChanged(int index);

	// reimplemented (iqt::TGuiObserverWrap)
	virtual void OnGuiModelShown();
};


} // namespace iqt


#endif // !iqt_CSelectionParamGuiComp_included


