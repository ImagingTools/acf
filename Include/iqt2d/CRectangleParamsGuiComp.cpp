#include "iqt2d/CRectangleParamsGuiComp.h"


// ACF includes
#include "istd/TChangeNotifier.h"

#include "iqt2d/CRectangleShape.h"

#include "iqt/CSignalBlocker.h"


namespace iqt2d
{


// reimplemented (iqtgui::TGuiObserverWrap)

void CRectangleParamsGuiComp::OnGuiModelAttached()
{
	BaseClass::OnGuiModelAttached();

	QObject::connect(LeftSpin, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	QObject::connect(RightSpin, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	QObject::connect(BottomSpin, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	QObject::connect(TopSpin, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
}


void CRectangleParamsGuiComp::OnGuiModelDetached()
{
	QObject::disconnect(LeftSpin, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	QObject::disconnect(RightSpin, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	QObject::disconnect(BottomSpin, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));
	QObject::disconnect(TopSpin, SIGNAL(valueChanged(double)), this, SLOT(OnParamsChanged(double)));

	BaseClass::OnGuiModelDetached();
}


// reimplemented (imod::IModelEditor)

void CRectangleParamsGuiComp::UpdateModel() const
{
	I_ASSERT(IsGuiCreated());

	i2d::CRectangle* objectPtr = GetObjectPtr();
	I_ASSERT(objectPtr != NULL);

	istd::CChangeNotifier notifier(NULL);

	if (objectPtr->GetLeft() != LeftSpin->value()){
		notifier.SetPtr(objectPtr);
		objectPtr->SetLeft(LeftSpin->value());
	}

	if (objectPtr->GetRight() != RightSpin->value()){
		notifier.SetPtr(objectPtr);
		objectPtr->SetRight(RightSpin->value());
	}

	if (objectPtr->GetTop() != TopSpin->value()){
		notifier.SetPtr(objectPtr);
		objectPtr->SetTop(TopSpin->value());
	}

	if (objectPtr->GetBottom() != BottomSpin->value()){
		notifier.SetPtr(objectPtr);
		objectPtr->SetBottom(BottomSpin->value());
	}
}


void CRectangleParamsGuiComp::UpdateEditor(int /*updateFlags*/)
{
	I_ASSERT(IsGuiCreated());

	i2d::CRectangle* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		iqt::CSignalBlocker block(LeftSpin);
		LeftSpin->setValue(objectPtr->GetLeft());

		iqt::CSignalBlocker block2(RightSpin);
		RightSpin->setValue(objectPtr->GetRight());

		iqt::CSignalBlocker block3(BottomSpin);
		BottomSpin->setValue(objectPtr->GetBottom());

		iqt::CSignalBlocker block4(TopSpin);
		TopSpin->setValue(objectPtr->GetTop());
	}
}


// reimplemented (imod::IObserver)

bool CRectangleParamsGuiComp::OnAttached(imod::IModel* modelPtr)
{
	if (BaseClass::OnAttached(modelPtr)){
		const ShapesMap& shapesMap = GetShapesMap();
		for (		ShapesMap::const_iterator iter = shapesMap.begin();
					iter != shapesMap.end();
					++iter){
			const Shapes& shapes = iter->second;
			int shapesCount = shapes.GetCount();
			for (int shapeIndex = 0; shapeIndex < shapesCount; ++shapeIndex){
				iqt2d::CRectangleShape* shapePtr = dynamic_cast<iqt2d::CRectangleShape*>(shapes.GetAt(shapeIndex));
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


bool CRectangleParamsGuiComp::OnDetached(imod::IModel* modelPtr)
{
	if (BaseClass::OnDetached(modelPtr)){
		const ShapesMap& shapesMap = GetShapesMap();
		for (		ShapesMap::const_iterator iter = shapesMap.begin();
					iter != shapesMap.end();
					++iter){
			const Shapes& shapes = iter->second;
			int shapesCount = shapes.GetCount();
			for (int shapeIndex = 0; shapeIndex < shapesCount; ++shapeIndex){
				iqt2d::CRectangleShape* shapePtr = dynamic_cast<iqt2d::CRectangleShape*>(shapes.GetAt(shapeIndex));
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


// reimplemented (iqt2d::TSceneExtenderCompBase)

void CRectangleParamsGuiComp::CreateShapes(int /*sceneId*/, bool inactiveOnly, Shapes& result)
{
	imod::IModel* modelPtr = GetModelPtr();
	if (modelPtr != NULL){
		CRectangleShape* shapePtr = new CRectangleShape(!inactiveOnly);
		if (shapePtr != NULL){
			shapePtr->setZValue(*m_rectZValueAttrPtr);
			result.PushBack(shapePtr);

			imod::IModel* modelPtr = GetModelPtr();
			if (modelPtr != NULL){
				modelPtr->AttachObserver(shapePtr);
			}
		}
	}
}


// protected slots

void CRectangleParamsGuiComp::OnParamsChanged(double /*value*/)
{
	if (!IsUpdateBlocked()){
		UpdateBlocker blockUpdate(this);

		UpdateModel();
	}
}


} // namespace iqt2d

