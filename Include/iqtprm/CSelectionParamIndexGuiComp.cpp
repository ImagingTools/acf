#include "iqtprm/CSelectionParamIndexGuiComp.h"


// Qt includes
#include <QtGui/QFileDialog>


// ACF includes
#include "istd/TChangeNotifier.h"

#include "iprm/IOptionsList.h"


namespace iqtprm
{


// reimplemented (imod::IModelEditor)

void CSelectionParamIndexGuiComp::UpdateModel() const
{
	Q_ASSERT(IsGuiCreated());
	iprm::ISelectionParam* objectPtr = GetObjectPtr();
	Q_ASSERT(objectPtr != NULL);

	int iterationCount = OptionIndexSpin->value();

	if (iterationCount != objectPtr->GetSelectedOptionIndex()){
		istd::CChangeNotifier notifier(objectPtr);

		objectPtr->SetSelectedOptionIndex(iterationCount);
	}
}


// protected methods

// reimplemented (iqtgui::TGuiObserverWrap)

void CSelectionParamIndexGuiComp::UpdateGui(int /*updateFlags*/)
{
	Q_ASSERT(IsGuiCreated());

	const iprm::ISelectionParam* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		int maxCount = 10;
		const iprm::IOptionsList* constraintsPtr = objectPtr->GetSelectionConstraints();
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
	DoUpdateModel();
}


} // namespace iqtprm

