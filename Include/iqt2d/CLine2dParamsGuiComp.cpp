#include "iqt2d/CLine2dParamsGuiComp.h"


// ACF includes
#include "istd/TChangeNotifier.h"

#include "iqt/CSignalBlocker.h"


namespace iqt2d
{


// public methods

// reimplemented (imod::IModelEditor)

void CLine2dParamsGuiComp::UpdateModel() const
{
	I_ASSERT(IsGuiCreated());

	i2d::CLine2d* objectPtr = GetObjectPtr();
	I_ASSERT(objectPtr != NULL);

	istd::CChangeNotifier notifier(NULL);

	i2d::CVector2d point1(Point1XSB->value(), Point1YSB->value());
	if (objectPtr->GetPoint1() != point1){
		notifier.SetPtr(objectPtr);
		objectPtr->SetPoint1(point1);
	}

	i2d::CVector2d point2(Point2XSB->value(), Point2YSB->value());
	if (objectPtr->GetPoint2() != point2){
		notifier.SetPtr(objectPtr);
		objectPtr->SetPoint2(point2);
	}
}


// protected methods

// reimplemented (iqtgui::TGuiObserverWrap)

void CLine2dParamsGuiComp::OnGuiModelAttached()
{
	BaseClass::OnGuiModelAttached();

	QObject::connect(Point1XSB, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	QObject::connect(Point1YSB, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	QObject::connect(Point2XSB, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	QObject::connect(Point2YSB, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));

	if (m_unitNameAttrPtr.IsValid()){
		Point1UnitLabel->setVisible(true);
		Point1UnitLabel->setText(*m_unitNameAttrPtr);
		Point2UnitLabel->setVisible(true);
		Point2UnitLabel->setText(*m_unitNameAttrPtr);
	}
	else{
		Point1UnitLabel->setVisible(false);
		Point2UnitLabel->setVisible(false);
	}
}


void CLine2dParamsGuiComp::OnGuiModelDetached()
{
	QObject::disconnect(Point1XSB, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	QObject::disconnect(Point1YSB, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	QObject::disconnect(Point2XSB, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	QObject::disconnect(Point2YSB, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));

	BaseClass::OnGuiModelDetached();
}


void CLine2dParamsGuiComp::UpdateGui(int /*updateFlags*/)
{
	I_ASSERT(IsGuiCreated());

	i2d::CLine2d* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		const i2d::CVector2d& point1 = objectPtr->GetPoint1();

		iqt::CSignalBlocker block(Point1XSB);
		Point1XSB->setValue(point1.GetX());
	
		iqt::CSignalBlocker block2(Point1YSB);
		Point1YSB->setValue(point1.GetY());

		const i2d::CVector2d& point2 = objectPtr->GetPoint2();
		iqt::CSignalBlocker block3(Point2XSB);
		Point2XSB->setValue(point2.GetX());

		iqt::CSignalBlocker block4(Point2YSB);
		Point2YSB->setValue(point2.GetY());
	}
}


// protected slots

void CLine2dParamsGuiComp::OnParamsChanged(double /*value*/)
{
	if (!IsUpdateBlocked()){
		UpdateBlocker updateBlocker(this);

		UpdateModel();

		UpdateShapeView();
	}
}


} // namespace iqt2d


