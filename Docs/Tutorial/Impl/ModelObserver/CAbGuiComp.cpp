#include "CAbGuiComp.h"


// ACF includes
#include "istd/TChangeNotifier.h"


// protected methods

// reimplemented (imod::IModelEditor)

void CAbGuiComp::UpdateModel()
{
	IAb* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		istd::TChangeNotifier<IAb> changePtr(objectPtr);

		changePtr->SetA(AValueSpinBox->value());
		changePtr->SetB(BValueEdit->text().toStdString());
	}
}


// reimplemented (iqtgui::TGuiObserverWrap)

void CAbGuiComp::UpdateGui(int /*updateFlags*/)
{
	I_ASSERT(IsGuiCreated());

	IAb* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		AValueSpinBox->setValue(objectPtr->GetA());

		BValueEdit->setText(objectPtr->GetB().c_str());
	}
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
	if (!IsUpdateBlocked()){
		UpdateBlocker blockUpdates(this);

		UpdateModel();
	}
}


void CAbGuiComp::OnBValueChanged(const QString& /*value*/)
{
	if (!IsUpdateBlocked()){
		UpdateBlocker blockUpdates(this);

		UpdateModel();
	}
}


