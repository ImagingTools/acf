#include "iqt2d/CCircleParamsGuiComp.h"


#include "istd/TChangeNotifier.h"

#include "iqt2d/CCircleShape.h"

#include "iqt/CSignalBlocker.h"


namespace iqt2d
{


// reimplemented (iqtgui::TGuiObserverWrap)

void CCircleParamsGuiComp::OnGuiModelAttached()
{
	BaseClass::OnGuiModelAttached();

	QObject::connect(XSpin, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	QObject::connect(YSpin, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	QObject::connect(RadiusSpin, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));

	if (m_unitNameAttrPtr.IsValid()){
		PositionUnitLabel->setVisible(true);
		RadiusUnitLabel->setText(iqt::GetQString(*m_unitNameAttrPtr));
	}
	else{
		PositionUnitLabel->setVisible(false);
		RadiusUnitLabel->setVisible(false);
	}
}


void CCircleParamsGuiComp::OnGuiModelDetached()
{
	QObject::disconnect(XSpin, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	QObject::disconnect(YSpin, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	QObject::disconnect(RadiusSpin, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));

	BaseClass::OnGuiModelDetached();
}


// reimplemented (imod::IObserver)

bool CCircleParamsGuiComp::OnAttached(imod::IModel* modelPtr)
{
	if (BaseClass::OnAttached(modelPtr)){
		const ShapesMap& shapesMap = GetShapesMap();
		for (		ShapesMap::const_iterator iter = shapesMap.begin();
					iter != shapesMap.end();
					++iter){
			const Shapes& shapes = iter->second;
			int shapesCount = shapes.GetCount();
			for (int shapeIndex = 0; shapeIndex < shapesCount; ++shapeIndex){
				iqt2d::CCircleShape* shapePtr = dynamic_cast<iqt2d::CCircleShape*>(shapes.GetAt(shapeIndex));
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


bool CCircleParamsGuiComp::OnDetached(imod::IModel* modelPtr)
{
	if (BaseClass::OnDetached(modelPtr)){
		const ShapesMap& shapesMap = GetShapesMap();
		for (		ShapesMap::const_iterator iter = shapesMap.begin();
					iter != shapesMap.end();
					++iter){
			const Shapes& shapes = iter->second;
			int shapesCount = shapes.GetCount();
			for (int shapeIndex = 0; shapeIndex < shapesCount; ++shapeIndex){
				iqt2d::CCircleShape* shapePtr = dynamic_cast<iqt2d::CCircleShape*>(shapes.GetAt(shapeIndex));
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

void CCircleParamsGuiComp::UpdateModel() const
{
	i2d::CCircle* objectPtr = GetObjectPtr();
	if (objectPtr != NULL && !IsUpdateBlocked()){
		UpdateBlocker blocker(const_cast<CCircleParamsGuiComp*>(this));

		istd::CChangeNotifier notifier(NULL);

		i2d::CVector2d postion(XSpin->value(), YSpin->value());
		if (objectPtr->GetCenter() != postion){
			notifier.SetPtr(objectPtr);
			objectPtr->SetCenter(postion);
		}

		double radius = RadiusSpin->value();
		if (objectPtr->GetRadius() != radius){
			notifier.SetPtr(objectPtr);
			objectPtr->SetRadius(radius);
		}
	}
}


void CCircleParamsGuiComp::UpdateEditor(int /*updateFlags*/)
{
	i2d::CCircle* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		const i2d::CVector2d& center = objectPtr->GetCenter();

		iqt::CSignalBlocker block(XSpin);
		iqt::CSignalBlocker block2(YSpin);
		iqt::CSignalBlocker block3(RadiusSpin);

		XSpin->setValue(center.GetX());
		YSpin->setValue(center.GetY());

		RadiusSpin->setValue(objectPtr->GetRadius());
	}
}


// reimplemented (iqt2d::TSceneExtenderCompBase)

void CCircleParamsGuiComp::CreateShapes(int /*sceneId*/, bool inactiveOnly, Shapes& result)
{
	I_ASSERT(m_circleZValueAttrPtr.IsValid());	// this attribute is obligatory

	CCircleShape* shapePtr = new CCircleShape(!inactiveOnly);
	if (shapePtr != NULL){
		shapePtr->setZValue(*m_circleZValueAttrPtr);
		result.PushBack(shapePtr);

		imod::IModel* modelPtr = GetModelPtr();
		if (modelPtr != NULL){
			modelPtr->AttachObserver(shapePtr);
		}
	}
}


// protected slots

void CCircleParamsGuiComp::OnParamsChanged(double /*value*/)
{
	UpdateModel();
}


} // namespace iqt2d


