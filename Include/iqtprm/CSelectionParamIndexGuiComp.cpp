#include <iqtprm/CSelectionParamIndexGuiComp.h>


// Qt includes
#include <QtCore/QtGlobal>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QFileDialog>
#else
#include <QtGui/QFileDialog>
#endif


// ACF includes
#include <istd/CChangeNotifier.h>

#include <iprm/IOptionsList.h>


namespace iqtprm
{


// protected methods

// reimplemented (iqtgui::TGuiObserverWrap)

void CSelectionParamIndexGuiComp::UpdateModel() const
{
	Q_ASSERT(IsGuiCreated());
	iprm::ISelectionParam* objectPtr = GetObservedObject();
	Q_ASSERT(objectPtr != NULL);

	int iterationCount = OptionIndexSpin->value();

	if (iterationCount != objectPtr->GetSelectedOptionIndex()){
		istd::CChangeNotifier notifier(objectPtr);

		objectPtr->SetSelectedOptionIndex(iterationCount);
	}
}


void CSelectionParamIndexGuiComp::UpdateGui(const istd::IChangeable::ChangeSet& /*changeSet*/)
{
	Q_ASSERT(IsGuiCreated());

	const iprm::ISelectionParam* objectPtr = GetObservedObject();
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

