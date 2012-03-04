#include "iqt2d/CQuadrangleParamsGuiComp.h"


// ACF includes
#include "istd/TChangeNotifier.h"

#include "iqt/CSignalBlocker.h"


namespace iqt2d
{


// public methods

// reimplemented (imod::IModelEditor)

void CQuadrangleParamsGuiComp::UpdateModel() const
{
	I_ASSERT(IsGuiCreated());

	i2d::CQuadrangle* objectPtr = GetObjectPtr();
	I_ASSERT(objectPtr != NULL);

	i2d::CVector2d point1(FirstDiagPoint1XSpin->value(), FirstDiagPoint1YSpin->value());
	i2d::CVector2d point2(FirstDiagPoint2XSpin->value(), FirstDiagPoint2YSpin->value());

	i2d::CVector2d point3(SecondDiagPoint1XSpin->value(), SecondDiagPoint1YSpin->value());
	i2d::CVector2d point4(SecondDiagPoint2XSpin->value(), SecondDiagPoint2YSpin->value());

	i2d::CLine2d firstDiag(point1, point2);
	i2d::CLine2d secondDiag(point3, point4);

	istd::CChangeNotifier changePtr(NULL);

	if (firstDiag != objectPtr->GetFirstDiagonal() || secondDiag != objectPtr->GetSecondDiagonal()){
		changePtr.SetPtr(objectPtr);
	
		objectPtr->SetFirstDiagonal(i2d::CLine2d(point1, point2));
		objectPtr->SetSecondDiagonal(i2d::CLine2d(point3, point4));
	}
}


// protected methods

// reimplemented (iqtgui::TGuiObserverWrap)

void CQuadrangleParamsGuiComp::OnGuiModelAttached()
{
	BaseClass::OnGuiModelAttached();

	QObject::connect(FirstDiagPoint1XSpin, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	QObject::connect(FirstDiagPoint1YSpin, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	QObject::connect(FirstDiagPoint2XSpin, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	QObject::connect(FirstDiagPoint2YSpin, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));

	QObject::connect(SecondDiagPoint1XSpin, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	QObject::connect(SecondDiagPoint1YSpin, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	QObject::connect(SecondDiagPoint2XSpin, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	QObject::connect(SecondDiagPoint2YSpin, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));

	if (m_unitNameAttrPtr.IsValid()){
		FirstDiagPoint1UnitLabel->setVisible(true);
		FirstDiagPoint1UnitLabel->setText(*m_unitNameAttrPtr);
		FirstDiagPoint2UnitLabel->setVisible(true);
		FirstDiagPoint2UnitLabel->setText(*m_unitNameAttrPtr);
		
		SecondDiagPoint1UnitLabel->setVisible(true);
		SecondDiagPoint1UnitLabel->setText(*m_unitNameAttrPtr);
		SecondDiagPoint2UnitLabel->setVisible(true);
		SecondDiagPoint2UnitLabel->setText(*m_unitNameAttrPtr);
	}
	else{
		FirstDiagPoint1UnitLabel->setVisible(false);
		FirstDiagPoint2UnitLabel->setVisible(false);
		SecondDiagPoint1UnitLabel->setVisible(false);
		SecondDiagPoint2UnitLabel->setVisible(false);
	}
}


void CQuadrangleParamsGuiComp::OnGuiModelDetached()
{
	QObject::disconnect(FirstDiagPoint1XSpin, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	QObject::disconnect(FirstDiagPoint1YSpin, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	QObject::disconnect(FirstDiagPoint2XSpin, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	QObject::disconnect(FirstDiagPoint2YSpin, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));

	QObject::disconnect(SecondDiagPoint1XSpin, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	QObject::disconnect(SecondDiagPoint1YSpin, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	QObject::disconnect(SecondDiagPoint2XSpin, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	QObject::disconnect(SecondDiagPoint2YSpin, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));

	BaseClass::OnGuiModelDetached();
}


void CQuadrangleParamsGuiComp::UpdateGui(int /*updateFlags*/)
{
	I_ASSERT(IsGuiCreated());

	i2d::CQuadrangle* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		const i2d::CVector2d& point1 = objectPtr->GetFirstDiagonal().GetPoint1();
		const i2d::CVector2d& point2 = objectPtr->GetFirstDiagonal().GetPoint2();
		const i2d::CVector2d& point3 = objectPtr->GetSecondDiagonal().GetPoint1();
		const i2d::CVector2d& point4 = objectPtr->GetSecondDiagonal().GetPoint2();

		iqt::CSignalBlocker block(GetWidget(), true);

		FirstDiagPoint1XSpin->setValue(point1.GetX());
		FirstDiagPoint1YSpin->setValue(point1.GetY());
		FirstDiagPoint2XSpin->setValue(point2.GetX());
		FirstDiagPoint2YSpin->setValue(point2.GetY());

		SecondDiagPoint1XSpin->setValue(point3.GetX());
		SecondDiagPoint1YSpin->setValue(point3.GetY());
		SecondDiagPoint2XSpin->setValue(point4.GetX());
		SecondDiagPoint2YSpin->setValue(point4.GetY());
	}
}


// protected slots

void CQuadrangleParamsGuiComp::OnParamsChanged(double /*value*/)
{
	if (!IsUpdateBlocked()){
		UpdateBlocker updateBlocker(this);

		UpdateModel();

		UpdateShapeView();
	}
}


} // namespace iqt2d


