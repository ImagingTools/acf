#include "iqtsig/CSamplingParamsGuiComp.h"


#include "istd/TChangeNotifier.h"


namespace iqtsig
{


// reimplemented (imod::IModelEditor)

void CSamplingParamsGuiComp::UpdateModel() const
{
}


void CSamplingParamsGuiComp::UpdateEditor()
{
	isig::ISamplingParams* objectPtr = GetObjectPtr();
	if (IsGuiCreated() && (objectPtr != NULL)){
		IntervalSB->setValue(objectPtr->GetInterval() * 1000);
		ModeCB->setCurrentIndex(objectPtr->GetSamplingMode());
	}
}


// protected slots

void CSamplingParamsGuiComp::on_IntervalSB_valueChanged(double value)
{
	isig::ISamplingParams* objectPtr = GetObjectPtr();
	if (IsGuiCreated() && (objectPtr != NULL)){
		istd::CRange intervalRange = objectPtr->GetIntervalRange();
		double interval = intervalRange.GetNearestInRange(value * 0.001);
		if (fabs(interval - objectPtr->GetInterval()) > I_EPSILON){
			istd::CChangeNotifier notifier(objectPtr);

			objectPtr->SetInterval(interval);
		}
	}
}


void CSamplingParamsGuiComp::on_ModeCB_currentIndexChanged(int index)
{
	isig::ISamplingParams* objectPtr = GetObjectPtr();
	if (IsGuiCreated() && (objectPtr != NULL)){
		bool isSupported = objectPtr->IsSamplingModeSupported(index);
		if (isSupported && (index != objectPtr->GetSamplingMode())){
			istd::CChangeNotifier notifier(objectPtr);

			objectPtr->SetSamplingMode(index);
		}
	}
}


} // namespace iqtsig


