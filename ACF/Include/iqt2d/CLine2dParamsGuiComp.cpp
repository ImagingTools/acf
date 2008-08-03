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


// reimplemented (imod::IObserver)

bool CLine2dParamsGuiComp::OnAttached(imod::IModel* modelPtr)
{
	if (BaseClass::OnAttached(modelPtr)){
		const ShapesMap& shapesMap = GetShapesMap();
		for (		ShapesMap::const_iterator iter = shapesMap.begin();
					iter != shapesMap.end();
					++iter){
			const Shapes& shapes = iter->second;
			int shapesCount = shapes.GetCount();
			for (int shapeIndex = 0; shapeIndex < shapesCount; ++shapeIndex){
				iqt2d::CLine2dShape* shapePtr = dynamic_cast<iqt2d::CLine2dShape*>(shapes.GetAt(shapeIndex));
				if (shapePtr != NULL){
					modelPtr->AttachObserver(shapePtr);
				}
			}
		}

		return true;
	}
	else{
		return false;
	}
}


bool CLine2dParamsGuiComp::OnDetached(imod::IModel* modelPtr)
{
	if (BaseClass::OnDetached(modelPtr)){
		const ShapesMap& shapesMap = GetShapesMap();
		for (		ShapesMap::const_iterator iter = shapesMap.begin();
					iter != shapesMap.end();
					++iter){
			const Shapes& shapes = iter->second;
			int shapesCount = shapes.GetCount();
			for (int shapeIndex = 0; shapeIndex < shapesCount; ++shapeIndex){
				iqt2d::CLine2dShape* shapePtr = dynamic_cast<iqt2d::CLine2dShape*>(shapes.GetAt(shapeIndex));
				if (shapePtr != NULL){
					modelPtr->DetachObserver(shapePtr);
				}
			}
		}

		return true;
	}
	else{
		return false;
	}
}


// reimplemented (imod::IModelEditor)

void CLine2dParamsGuiComp::UpdateModel() const
{
	i2d::CLine2d* objectPtr = GetObjectPtr();
	if (objectPtr != NULL && !IsUpdateBlocked()){
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

		const i2d::CVector2d& point2 = objectPtr->GetPoint2();
		Point2XSB->setValue(point2.GetX());
		Point2YSB->setValue(point2.GetY());
	}
}


// reimplemented (iqt2d::TSceneExtenderCompBase)

void CLine2dParamsGuiComp::CreateShapes(int /*sceneId*/, bool inactiveOnly, Shapes& result)
{
	I_ASSERT(m_lineZValueAttrPtr.IsValid());	// this attribute is obligatory

	CLine2dShape* shapePtr = new CLine2dShape(!inactiveOnly);
	if (shapePtr != NULL){
		shapePtr->setZValue(*m_lineZValueAttrPtr);
		result.PushBack(shapePtr);

		imod::IModel* modelPtr = GetModelPtr();
		if (modelPtr != NULL){
			modelPtr->AttachObserver(shapePtr);
		}
	}
}


// protected slots

void CLine2dParamsGuiComp::OnParamsChanged(double /*value*/)
{
	UpdateModel();
}


} // namespace iqt2d


