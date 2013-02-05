#include "CAbGuiComp.h"


// ACF includes
#include "istd/TChangeNotifier.h"


// Tutorial includes
#include "IAconstraints.h"


// reimplemented (imod::IModelEditor)

void CAbGuiComp::UpdateModel() const
{
	IAb* objectPtr = GetObjectPtr();
	Q_ASSERT(objectPtr != NULL);

	istd::TChangeNotifier<IAb> changePtr(objectPtr);

	changePtr->SetA(AValueSpinBox->value());
	changePtr->SetB(BValueEdit->text().toLocal8Bit());
}


// protected methods

// reimplemented (iqtgui::TGuiObserverWrap)

void CAbGuiComp::UpdateGui(int /*updateFlags*/)
{
	Q_ASSERT(IsGuiCreated());

	IAb* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		AValueSpinBox->setValue(objectPtr->GetA());

		BValueEdit->setText(objectPtr->GetB());
	}
}

void CAbGuiComp::OnGuiModelAttached()
{
	// Set data model constraints to the editor of A:
	IAb* objectPtr = GetObjectPtr();
	Q_ASSERT(objectPtr != NULL);

	const IAConstraints& constraints = objectPtr->GetAConstraints();
	AValueSpinBox->setMinimum(constraints.GetARange().GetMinValue());
	AValueSpinBox->setMaximum(constraints.GetARange().GetMaxValue());

	// Call basic functionality:
	BaseClass::OnGuiModelAttached();
}


// reimplemented (CGuiComponentBase)

void CAbGuiComp::OnGuiCreated()
{
	// Connect signal from A editor:
	connect(AValueSpinBox, SIGNAL(valueChanged(int)), this, SLOT(OnAValueChanged(int)));

	// Connect signal from B editor:
	connect(BValueEdit, SIGNAL(textEdited(const QString&)), this, SLOT(OnBValueChanged(const QString&)));

	BaseClass::OnGuiCreated();
}


// private slots

void CAbGuiComp::OnAValueChanged(int /*value*/)
{
	DoUpdateModel();
}


void CAbGuiComp::OnBValueChanged(const QString& /*value*/)
{
	DoUpdateModel();
}


