#include "iqtprm/CSelectionParamIndexGuiComp.h"


// Qt includes
#include <QFileDialog>


// ACF includes
#include "istd/TChangeNotifier.h"

#include "iprm/ISelectionConstraints.h"


namespace iqtprm
{


// reimplemented (imod::IModelEditor)

void CSelectionParamIndexGuiComp::UpdateModel() const
{
	I_ASSERT(IsGuiCreated());
	iprm::ISelectionParam* objectPtr = GetObjectPtr();
	I_ASSERT(objectPtr != NULL);

	int iterationCount = OptionIndexSpin->value();

	if (iterationCount != objectPtr->GetSelectedOptionIndex()){
		istd::CChangeNotifier notifier(objectPtr);

		objectPtr->SetSelectedOptionIndex(iterationCount);
	}
}


void CSelectionParamIndexGuiComp::UpdateEditor(int /*updateFlags*/)
{
	I_ASSERT(IsGuiCreated());

	const iprm::ISelectionParam* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		int maxCount = 10;
		const iprm::ISelectionConstraints* constraintsPtr = objectPtr->GetConstraints();
		if (constraintsPtr != NULL){
			maxCount = constraintsPtr->GetOptionsCount() - 1;
		}

		OptionIndexSpin->setMaximum(maxCount);
		OptionIndexSlider->setMaximum(maxCount);

		OptionIndexSpin->setValue(objectPtr->GetSelectedOptionIndex());
	}
}


// protected slots

void CSelectionParamIndexGuiComp::on_OptionIndexSlider_valueChanged(int /*value*/)
{
	if (!IsUpdateBlocked()){
		UpdateBlocker blockUpdate(this);

		UpdateModel();
	}
}


} // namespace iqtprm

