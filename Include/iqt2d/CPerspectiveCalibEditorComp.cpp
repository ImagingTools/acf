#include <iqt2d/CPerspectiveCalibEditorComp.h>


// ACF includes
#include <istd/CChangeGroup.h>

#include <iqt/CSignalBlocker.h>


namespace iqt2d
{


// public methods

// reimplemented (imod::IModelEditor)

void CPerspectiveCalibEditorComp::UpdateModel() const
{
	Q_ASSERT(IsGuiCreated());

	i2d::CPerspectiveCalibration2d* objectPtr = GetObservedObject();
	Q_ASSERT(objectPtr != NULL);

	i2d::CMatrix2d deform(Rotation11Spin->value(), Rotation12Spin->value(), Rotation21Spin->value(), Rotation22Spin->value());
	i2d::CVector2d translation(TranslationXSpin->value(), TranslationYSpin->value());
	i2d::CVector2d perspective(PerspectiveXSpin->value() / 1000.0, PerspectiveYSpin->value() / 1000.0);

	istd::CChangeGroup changeGroup(objectPtr);
	Q_UNUSED(changeGroup);

	objectPtr->SetAffinePart(i2d::CAffine2d(deform, translation));
	objectPtr->SetPerspAxis(perspective);
}


// protected methods

// reimplemented (iqtgui::TGuiObserverWrap)

void CPerspectiveCalibEditorComp::UpdateGui(const istd::IChangeable::ChangeSet& /*changeSet*/)
{
	Q_ASSERT(IsGuiCreated());

	i2d::CPerspectiveCalibration2d* objectPtr = GetObservedObject();
	if (objectPtr != NULL){
		const i2d::CAffine2d& affinePart = objectPtr->GetAffinePart();
		const i2d::CMatrix2d& deform = affinePart.GetDeformMatrix();
		const i2d::CVector2d& translation = affinePart.GetTranslation();
		const i2d::CVector2d& perspective = objectPtr->GetPerspAxis();

		iqt::CSignalBlocker block(GetWidget(), true);

		Rotation11Spin->setValue(deform.GetAt(0, 0));
		Rotation12Spin->setValue(deform.GetAt(0, 1));
		Rotation21Spin->setValue(deform.GetAt(1, 0));
		Rotation22Spin->setValue(deform.GetAt(1, 1));

		TranslationXSpin->setValue(translation.GetX());
		TranslationYSpin->setValue(translation.GetY());

		PerspectiveXSpin->setValue(perspective.GetX() * 1000);
		PerspectiveYSpin->setValue(perspective.GetY() * 1000);

		RotationGB->setEnabled(true);
		PerspectiveGB->setEnabled(true);
		TranslationGB->setEnabled(true);
	}
	else{
		RotationGB->setEnabled(false);
		PerspectiveGB->setEnabled(false);
		TranslationGB->setEnabled(false);
	}
}


void CPerspectiveCalibEditorComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	CalibrateButton->setVisible(m_calibProviderCompPtr.IsValid());

	QObject::connect(Rotation11Spin, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	QObject::connect(Rotation12Spin, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	QObject::connect(Rotation21Spin, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	QObject::connect(Rotation22Spin, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));

	QObject::connect(TranslationXSpin, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	QObject::connect(TranslationYSpin, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));

	QObject::connect(PerspectiveXSpin, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	QObject::connect(PerspectiveYSpin, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));

	if (m_calibProviderCompPtr.IsValid() && m_calibProviderModelCompPtr.IsValid()){
		RegisterModel(m_calibProviderModelCompPtr.GetPtr(), 0);
	}
}


void CPerspectiveCalibEditorComp::OnGuiDestroyed()
{
	UnregisterAllModels();

	QObject::disconnect(Rotation11Spin, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	QObject::disconnect(Rotation12Spin, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	QObject::disconnect(Rotation21Spin, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	QObject::disconnect(Rotation22Spin, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));

	QObject::disconnect(TranslationXSpin, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	QObject::disconnect(TranslationYSpin, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));

	QObject::disconnect(PerspectiveXSpin, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	QObject::disconnect(PerspectiveYSpin, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));

	BaseClass::OnGuiDestroyed();
}


// reimplemented (iqtgui::CGuiComponentBase)

void CPerspectiveCalibEditorComp::OnModelChanged(int /*modelId*/, const istd::IChangeable::ChangeSet& /*changeSet*/)
{
	Q_ASSERT(m_calibProviderCompPtr.IsValid());

	const i2d::ICalibration2d* calibrationPtr = m_calibProviderCompPtr->GetCalibration();
	CalibrateButton->setEnabled(calibrationPtr != NULL);
}


// protected slots

void CPerspectiveCalibEditorComp::on_CalibrateButton_clicked()
{
	Q_ASSERT(m_calibProviderCompPtr.IsValid());

	const i2d::ICalibration2d* newCalibrationPtr = m_calibProviderCompPtr->GetCalibration();

	i2d::CPerspectiveCalibration2d* objectPtr = GetObservedObject();
	if ((objectPtr != NULL) && (newCalibrationPtr != NULL)){
		objectPtr->CopyFrom(*newCalibrationPtr);
	}
}


void CPerspectiveCalibEditorComp::OnParamsChanged(double /*value*/)
{
	DoUpdateModel();
}


} // namespace iqt2d


