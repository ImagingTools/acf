#include <CAbGuiComp.h>


// ACF includes
#include <istd/CChangeGroup.h>


// Tutorial includes
#include <IAconstraints.h>


// protected methods

// reimplemented (iqtgui::TGuiObserverWrap)

void CAbGuiComp::UpdateModel() const
{
	IAb* objectPtr = GetObservedObject();
	Q_ASSERT(objectPtr != NULL);

	istd::CChangeGroup changeGroup(objectPtr);
	Q_UNUSED(changeGroup);

	objectPtr->SetA(AValueSpinBox->value());
	objectPtr->SetB(BValueEdit->text().toLocal8Bit());
}


void CAbGuiComp::UpdateGui(const istd::IChangeable::ChangeSet& /*changeSet*/)
{
	Q_ASSERT(IsGuiCreated());

	IAb* objectPtr = GetObservedObject();
	if (objectPtr != NULL){
		AValueSpinBox->setValue(objectPtr->GetA());

		BValueEdit->setText(objectPtr->GetB());
	}
}

void CAbGuiComp::OnGuiModelAttached()
{
	// Set data model constraints to the editor of A:
	IAb* objectPtr = GetObservedObject();
	Q_ASSERT(objectPtr != NULL);

	const IAConstraints& constraints = objectPtr->GetAConstraints();
	AValueSpinBox->setMinimum(constraints.GetARange().GetMinValue());
	AValueSpinBox->setMaximum(constraints.GetARange().GetMaxValue());

	// Call basic functionality:
	BaseClass::OnGuiModelAttached();
}


// reimplemented (iqtgui::CGuiComponentBase)

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


