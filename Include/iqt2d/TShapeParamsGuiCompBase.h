#ifndef iqt2d_TShapeParamsGuiCompBase_included
#define iqt2d_TShapeParamsGuiCompBase_included


// ACF includes
#include "iqtgui/TDesignerGuiObserverCompBase.h"

#include "iview/CShapeControl.h"

#include "iqt2d/TViewExtenderCompBase.h"


namespace iqt2d
{


template <class Ui, class Shape, class ShapeModel>
class TShapeParamsGuiCompBase:
			public iqt2d::TViewExtenderCompBase<
						iqtgui::TDesignerGuiObserverCompBase<Ui, ShapeModel> >
{
public:
	typedef iqt2d::TViewExtenderCompBase<iqtgui::TDesignerGuiObserverCompBase<Ui, ShapeModel> > BaseClass;

	I_BEGIN_COMPONENT(TShapeParamsGuiCompBase);
		I_ASSIGN(m_unitNameAttrPtr, "UnitName", "Name of geometric units e.g. mm", false, "mm");
	I_END_COMPONENT;

	// reimplemented (imod::IObserver)
	virtual bool OnAttached(imod::IModel* modelPtr);
	virtual bool OnDetached(imod::IModel* modelPtr);

protected:
	typedef typename BaseClass::Shapes Shapes;
	typedef typename BaseClass::ShapesMap ShapesMap;

	// reimplemented (iqt2d::TViewExtenderCompBase)
	virtual void CreateShapes(int sceneId, Shapes& result);

	I_ATTR(QString, m_unitNameAttrPtr);
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
			const Shapes& shapes = iter.value();
			int shapesCount = shapes.GetCount();
			for (int shapeIndex = 0; shapeIndex < shapesCount; ++shapeIndex){
				Shape* shapePtr = dynamic_cast<Shape*>(shapes.GetAt(shapeIndex));
				if (shapePtr != NULL){
					if (modelPtr->AttachObserver(shapePtr)){
						iview::CShapeBase* shapeBasePtr = dynamic_cast<iview::CShapeBase*>(shapePtr);
						if (shapeBasePtr != NULL){
							shapeBasePtr->SetVisible(true);
						}
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
			const Shapes& shapes = iter.value();
			int shapesCount = shapes.GetCount();
			for (int shapeIndex = 0; shapeIndex < shapesCount; ++shapeIndex){
				Shape* shapePtr = dynamic_cast<Shape*>(shapes.GetAt(shapeIndex));
				if (shapePtr != NULL){
					modelPtr->DetachObserver(shapePtr);

					iview::CShapeBase* shapeBasePtr = dynamic_cast<iview::CShapeBase*>(shapePtr);
					if (shapeBasePtr != NULL){
						shapeBasePtr->SetVisible(false);
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


// protected methods

// reimplemented (iqt2d::TViewExtenderCompBase)

template <class Ui, class Shape, class ShapeModel>
void TShapeParamsGuiCompBase<Ui, Shape, ShapeModel>::CreateShapes(int /*sceneId*/, Shapes& result)
{
	Shape* shapePtr = new Shape();
	if (shapePtr != NULL){
		result.PushBack(shapePtr);

		imod::IModel* modelPtr = BaseClass::GetModelPtr();
		if (modelPtr != NULL){
			if (modelPtr->AttachObserver(shapePtr)){
				iview::CShapeBase* shapeBasePtr = dynamic_cast<iview::CShapeBase*>(shapePtr);
				if (shapeBasePtr != NULL){
					shapeBasePtr->SetVisible(true);
				}
			}
		}
	}
}


} // namespace iview


#endif // !iview_TShapeParamsGuiCompBase_included


