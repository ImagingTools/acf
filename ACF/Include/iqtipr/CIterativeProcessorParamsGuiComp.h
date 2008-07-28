#ifndef iqt_CIterativeProcessorParamsGuiComp_included
#define iqt_CIterativeProcessorParamsGuiComp_included


#include "iipr/CIterativeProcessorParams.h"

#include "iqt/TDesignerGuiObserverCompBase.h"

#include "iqtipr/Generated/ui_CIterativeProcessorParamsGuiComp.h"


namespace iqtipr
{


class CIterativeProcessorParamsGuiComp: public iqt::TDesignerGuiObserverCompBase<
			Ui::CIterativeProcessorParamsGuiComp,
			iipr::CIterativeProcessorParams>
{
	Q_OBJECT
public:
	typedef iqt::TDesignerGuiObserverCompBase<
				Ui::CIterativeProcessorParamsGuiComp,
				iipr::CIterativeProcessorParams> BaseClass;

	I_BEGIN_COMPONENT(CIterativeProcessorParamsGuiComp)
	I_END_COMPONENT

	// reimplemented (imod::IModelEditor)
	virtual void UpdateModel() const;
	virtual void UpdateEditor();

protected slots:
	void on_IterationsSlider_valueChanged(int value);
};


} // namespace iqtipr


#endif // !iqt_CIterativeProcessorParamsGuiComp_included


