#ifndef iqt2d_TShapeParamsGuiCompBase_included
#define iqt2d_TShapeParamsGuiCompBase_included


#include "iqtgui/TDesignerGuiObserverCompBase.h"

#include "iqt2d/TSceneExtenderCompBase.h"


namespace iqt2d
{


template <class Ui, class Shape, class ShapeModel>
class TShapeParamsGuiCompBase:
			public iqt2d::TSceneExtenderCompBase<
						iqtgui::TDesignerGuiObserverCompBase<Ui, ShapeModel> >
{
public:
	typedef iqt2d::TSceneExtenderCompBase<iqtgui::TDesignerGuiObserverCompBase<Ui, ShapeModel> > BaseClass;

	I_BEGIN_COMPONENT(TShapeParamsGuiCompBase);
		I_ASSIGN(m_unitNameAttrPtr, "UnitName", "Name of geometric units e.g. mm", false, "mm");
		I_ASSIGN(m_zValueAttrPtr, "ZValue", "Describe draw priority on display console (the objects with bigger value will overlap the other ones)", true, 1);
	I_END_COMPONENT;

	// reimplemented (imod::IObserver)
	virtual bool OnAttached(imod::IModel* modelPtr);
	virtual bool OnDetached(imod::IModel* modelPtr);

protected:
	typedef typename BaseClass::Shapes Shapes;
	typedef typename BaseClass::ShapesMap ShapesMap;

	// reimplemented (iqt2d::TSceneExtenderCompBase)
	virtual void CreateShapes(int sceneId, bool inactiveOnly, Shapes& result);

	I_ATTR(QString, m_unitNameAttrPtr);
	I_ATTR(double, m_zValueAttrPtr);
};


// public methods

// reimplemented (imod::IObserver)

template <class Ui, class Shape, class ShapeModel>
bool TShapeParamsGuiCompBase<Ui, Shape, ShapeModel>::OnAttached(imod::IModel* modelPtr)
{
	if (BaseClass::OnAttached(modelPtr)){
		const ShapesMap& shapesMap = BaseClass::GetShapesMap();
		for (		typename ShapesMap::const_iterator iter = shapesMap.begin();
					iter != shapesMap.end();
					++iter){
			const Shapes& shapes = iter->second;
			int shapesCount = shapes.GetCount();
			for (int shapeIndex = 0; shapeIndex < shapesCount; ++shapeIndex){
				Shape* shapePtr = dynamic_cast<Shape*>(shapes.GetAt(shapeIndex));
				if (shapePtr != NULL){
					if (modelPtr->AttachObserver(shapePtr)){
						shapePtr->setVisible(true);
					}
				}
			}
		}

		return true;
	}
	else{
		return false;
	}
}


template <class Ui, class Shape, class ShapeModel>
bool TShapeParamsGuiCompBase<Ui, Shape, ShapeModel>::OnDetached(imod::IModel* modelPtr)
{
	if (BaseClass::OnDetached(modelPtr)){
		const ShapesMap& shapesMap = BaseClass::GetShapesMap();
		for (		typename ShapesMap::const_iterator iter = shapesMap.begin();
					iter != shapesMap.end();
					++iter){
			const Shapes& shapes = iter->second;
			int shapesCount = shapes.GetCount();
			for (int shapeIndex = 0; shapeIndex < shapesCount; ++shapeIndex){
				Shape* shapePtr = dynamic_cast<Shape*>(shapes.GetAt(shapeIndex));
				if (shapePtr != NULL){
					modelPtr->DetachObserver(shapePtr);

					shapePtr->setVisible(false);
				}
			}
		}

		return true;
	}
	else{
		return false;
	}
}


// protected methods

// reimplemented (iqt2d::TSceneExtenderCompBase)

template <class Ui, class Shape, class ShapeModel>
void TShapeParamsGuiCompBase<Ui, Shape, ShapeModel>::CreateShapes(int /*sceneId*/, bool inactiveOnly, Shapes& result)
{
	I_ASSERT(m_zValueAttrPtr.IsValid());	// this attribute is obligatory
	Shape* shapePtr = new Shape(!inactiveOnly);
	if (shapePtr != NULL){
		shapePtr->setZValue(*m_zValueAttrPtr);
		shapePtr->setVisible(false);
		result.PushBack(shapePtr);

		imod::IModel* modelPtr = BaseClass::GetModelPtr();
		if (modelPtr != NULL){
			if (modelPtr->AttachObserver(shapePtr)){
				shapePtr->setVisible(true);
			}
		}
	}
}


} // namespace iqt2d


#endif // !iqt2d_TShapeParamsGuiCompBase_included


