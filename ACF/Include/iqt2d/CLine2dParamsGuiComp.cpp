#include "iqt2d/CLine2dParamsGuiComp.h"


#include "istd/TChangeNotifier.h"

#include "iqt2d/CLine2dShape.h"


namespace iqt2d
{


// reimplemented (iqt::TGuiObserverWrap)

void CLine2dParamsGuiComp::OnGuiModelAttached()
{
	BaseClass::OnGuiModelAttached();

	QObject::connect(Point1XSB, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	QObject::connect(Point1YSB, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	QObject::connect(Point2XSB, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	QObject::connect(Point2YSB, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));

	if (m_unitNameAttrPtr.IsValid()){
		Point1UnitLabel->setVisible(true);
		Point1UnitLabel->setText(iqt::GetQString(*m_unitNameAttrPtr));
		Point2UnitLabel->setVisible(true);
		Point2UnitLabel->setText(iqt::GetQString(*m_unitNameAttrPtr));
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


// reimplemented (imod::IModelEditor)

void CLine2dParamsGuiComp::UpdateModel() const
{
	i2d::CLine2d* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		UpdateBlocker blocker(const_cast<CLine2dParamsGuiComp*>(this));

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
}


void CLine2dParamsGuiComp::UpdateEditor()
{
	i2d::CLine2d* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		const i2d::CVector2d& point1 = objectPtr->GetPoint1();
		Point1XSB->setValue(point1.GetX());
		Point1YSB->setValue(point1.GetY());

		const i2d::CVector2d& point2 = objectPtr->GetPoint1();
		Point2XSB->setValue(point2.GetX());
		Point2YSB->setValue(point2.GetY());
	}
}


// reimplemented (iqt2d::TSceneExtenderCompBase)

void CLine2dParamsGuiComp::CreateShapes(int /*sceneId*/, bool /*inactiveOnly*/, Shapes& result)
{
	imod::IModel* modelPtr = GetModelPtr();
	if (modelPtr != NULL){
		istd::TDelPtr<CLine2dShape> shapePtr(new CLine2dShape());
		if (shapePtr.IsValid()){
			if (modelPtr->AttachObserver(shapePtr.GetPtr())){
				result.PushBack(shapePtr.PopPtr());
			}
		}
	}
}


// protected slots

void CLine2dParamsGuiComp::OnParamsChanged(double /*value*/)
{
	UpdateModel();
}


} // namespace iqt2d


