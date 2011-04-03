#include "iqt2d/CAnnulusSegmentParamsGuiComp.h"


// ACF includes
#include "istd/TChangeNotifier.h"

#include "iqt2d/CAnnulusSegmentShape.h"


namespace iqt2d
{


// reimplemented (imod::IObserver)

bool CAnnulusSegmentParamsGuiComp::OnAttached(imod::IModel* modelPtr)
{
	if (BaseClass::OnAttached(modelPtr)){
		const ShapesMap& shapesMap = GetShapesMap();
		for (		ShapesMap::const_iterator iter = shapesMap.begin();
					iter != shapesMap.end();
					++iter){
			const Shapes& shapes = iter->second;
			int shapesCount = shapes.GetCount();
			for (int shapeIndex = 0; shapeIndex < shapesCount; ++shapeIndex){
				iqt2d::CAnnulusShape* shapePtr = dynamic_cast<iqt2d::CAnnulusShape*>(shapes.GetAt(shapeIndex));
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


bool CAnnulusSegmentParamsGuiComp::OnDetached(imod::IModel* modelPtr)
{
	if (BaseClass::OnDetached(modelPtr)){
		const ShapesMap& shapesMap = GetShapesMap();
		for (		ShapesMap::const_iterator iter = shapesMap.begin();
					iter != shapesMap.end();
					++iter){
			const Shapes& shapes = iter->second;
			int shapesCount = shapes.GetCount();
			for (int shapeIndex = 0; shapeIndex < shapesCount; ++shapeIndex){
				iqt2d::CAnnulusShape* shapePtr = dynamic_cast<iqt2d::CAnnulusShape*>(shapes.GetAt(shapeIndex));
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

void CAnnulusSegmentParamsGuiComp::UpdateModel() const
{
	I_ASSERT(IsGuiCreated());

	i2d::CAnnulusSegment* objectPtr = GetObjectPtr();
	I_ASSERT(objectPtr != NULL);

	istd::CChangeNotifier notifier(NULL);

	i2d::CVector2d postion(XSpin->value(), YSpin->value());
	if (objectPtr->GetCenter() != postion){
		notifier.SetPtr(objectPtr);
		objectPtr->SetPosition(postion);
	}

	double innerRadius = InnerRadiusSpin->value();
	if (objectPtr->GetInnerRadius() != innerRadius){
		notifier.SetPtr(objectPtr);
		objectPtr->SetInnerRadius(innerRadius);
	}

	double outerRadius = OuterRadiusSpin->value();
	if (objectPtr->GetOuterRadius() != outerRadius){
		notifier.SetPtr(objectPtr);
		objectPtr->SetOuterRadius(outerRadius);
	}

	double beginAngle = imath::GetRadianFromDegree(BeginAngleSB->value());
	if (beginAngle != objectPtr->GetBeginAngle()){
		notifier.SetPtr(objectPtr);
		objectPtr->SetBeginAngle(beginAngle);
	}

	double endAngle = imath::GetRadianFromDegree(EndAngleSB->value());
	if (endAngle != objectPtr->GetEndAngle()){
		notifier.SetPtr(objectPtr);
		objectPtr->SetEndAngle(endAngle);
	}
}


void CAnnulusSegmentParamsGuiComp::UpdateEditor(int /*updateFlags*/)
{
	I_ASSERT(IsGuiCreated());

	i2d::CAnnulusSegment* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		const i2d::CVector2d& center = objectPtr->GetCenter();
		XSpin->setValue(center.GetX());
		YSpin->setValue(center.GetY());

		InnerRadiusSpin->setValue(objectPtr->GetInnerRadius());
		OuterRadiusSpin->setValue(objectPtr->GetOuterRadius());

		BeginAngleSB->setValue(imath::GetDegreeFromRadian(objectPtr->GetBeginAngle()));
		EndAngleSB->setValue(imath::GetDegreeFromRadian(objectPtr->GetEndAngle()));
	}
}


// reimplemented (iqt2d::TSceneExtenderCompBase)

void CAnnulusSegmentParamsGuiComp::CreateShapes(int /*sceneId*/, bool inactiveOnly, Shapes& result)
{
	I_ASSERT(m_annulusZValueAttrPtr.IsValid());	// this attribute is obligatory

	CAnnulusSegmentShape* shapePtr = new CAnnulusSegmentShape(!inactiveOnly);
	if (shapePtr != NULL){
		shapePtr->setZValue(*m_annulusZValueAttrPtr);
		result.PushBack(shapePtr);

		imod::IModel* modelPtr = GetModelPtr();
		if (modelPtr != NULL){
			modelPtr->AttachObserver(shapePtr);
		}
	}
}


// protected methods

// reimplemented (iqtgui::CGuiComponentBase)

void CAnnulusSegmentParamsGuiComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	QObject::connect(XSpin, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	QObject::connect(YSpin, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	QObject::connect(InnerRadiusSpin, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	QObject::connect(OuterRadiusSpin, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	QObject::connect(BeginAngleSB, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	QObject::connect(EndAngleSB, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));

	if (m_unitNameAttrPtr.IsValid()){
		PositionUnitLabel->setVisible(true);
		RadiusUnitLabel->setText(iqt::GetQString(*m_unitNameAttrPtr));
	}
	else{
		PositionUnitLabel->setVisible(false);
		RadiusUnitLabel->setVisible(false);
	}
}


void CAnnulusSegmentParamsGuiComp::OnGuiDestroyed()
{
	QObject::disconnect(XSpin, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	QObject::disconnect(YSpin, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	QObject::disconnect(InnerRadiusSpin, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	QObject::disconnect(OuterRadiusSpin, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	QObject::disconnect(BeginAngleSB, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	QObject::disconnect(EndAngleSB, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));

	BaseClass::OnGuiDestroyed();
}


// protected slots

void CAnnulusSegmentParamsGuiComp::OnParamsChanged(double /*value*/)
{
	if (!IsUpdateBlocked()){
		UpdateBlocker blockUpdate(this);

		UpdateModel();
	}
}


} // namespace iqt2d


