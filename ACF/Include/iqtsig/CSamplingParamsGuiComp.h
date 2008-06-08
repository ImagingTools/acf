#ifndef iqtsig_CSamplingParamsGuiComp_included
#define iqtsig_CSamplingParamsGuiComp_included


#include "isig/ISamplingParams.h"

#include "iqt/TDesignerGuiObserverCompBase.h"

#include "iqtsig/iqtsig.h"

#include "iqtsig/Generated/ui_CSamplingParamsGuiComp.h"


namespace iqtsig
{


class CSamplingParamsGuiComp: public iqt::TDesignerGuiObserverCompBase<
			Ui::CSamplingParamsGuiComp,
			isig::ISamplingParams>
{
	Q_OBJECT

public:
	typedef iqt::TDesignerGuiObserverCompBase<
				Ui::CSamplingParamsGuiComp,
				isig::ISamplingParams> BaseClass;

	I_BEGIN_COMPONENT(CSamplingParamsGuiComp)
	I_END_COMPONENT

	// reimplemented (imod::IModelEditor)
	virtual void UpdateModel() const;
	virtual void UpdateEditor();

protected slots:
	void on_IntervalSB_valueChanged(double value);
	void on_ModeCB_currentIndexChanged(int index);
};


} // namespace iqtsig


#endif // !iqtsig_CSamplingParamsGuiComp_included


