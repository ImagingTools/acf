#ifndef iqt2d_TShapeParamsGuiCompBase_included
#define iqt2d_TShapeParamsGuiCompBase_included


// ACF includes
#include "i2d/ICalibration2d.h"
#include "iqtgui/TDesignerGuiObserverCompBase.h"
#include "iview/IColorSchema.h"
#include "iview/IShapeFactory.h"
#include "iqt2d/TViewExtenderCompBase.h"


namespace iqt2d
{


template <class Ui, class Shape, class ShapeModel>
class TShapeParamsGuiCompBase:
			public iqt2d::TViewExtenderCompBase<
						iqtgui::TDesignerGuiObserverCompBase<Ui, ShapeModel> >,
			virtual public iview::IShapeFactory
{
public:
	typedef iqt2d::TViewExtenderCompBase<iqtgui::TDesignerGuiObserverCompBase<Ui, ShapeModel> > BaseClass;

	I_BEGIN_COMPONENT(TShapeParamsGuiCompBase);
		I_REGISTER_INTERFACE(iview::IShapeFactory);
		I_ASSIGN(m_defaultUnitInfoAttrPtr, "DefaultUnitInfo", "Provide default information about the logical value units e.g. mm, this will be used if no unit information found in model", false, "DefaultUnitInfo");
		I_ASSIGN(m_colorSchemaCompPtr, "ShapeColorSchema", "Color schema used by displayed shape", false, "ShapeColorSchema");
	I_END_COMPONENT;

	// reimplemented (imod::IObserver)
	virtual bool OnModelAttached(imod::IModel* modelPtr, istd::IChangeable::ChangeSet& changeMask);
	virtual bool OnModelDetached(imod::IModel* modelPtr);

	// reimplemented (iview::IShapeFactory)
	virtual iview::IShape* CreateShape(const i2d::IObject2d* objectPtr, bool connectToModel = false) const;

protected:
	typedef typename BaseClass::Shapes Shapes;
	typedef typename BaseClass::ShapesMap ShapesMap;

	QString GetUnitName() const;

	// reimplemented (iqt2d::TViewExtenderCompBase)
	virtual void CreateShapes(int sceneId, Shapes& result);

private:
	I_REF(imath::IUnitInfo, m_defaultUnitInfoAttrPtr);
	I_REF(iview::IColorSchema, m_colorSchemaCompPtr);
};


// public methods

// reimplemented (imod::IObserver)

template <class Ui, class Shape, class ShapeModel>
bool TShapeParamsGuiCompBase<Ui, Shape, ShapeModel>::OnModelAttached(imod::IModel* modelPtr, istd::IChangeable::ChangeSet& changeMask)
{
	if (BaseClass::OnModelAttached(modelPtr, changeMask)){
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
						shapePtr->SetVisible(true);
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
bool TShapeParamsGuiCompBase<Ui, Shape, ShapeModel>::OnModelDetached(imod::IModel* modelPtr)
{
	if (BaseClass::OnModelDetached(modelPtr)){
		const ShapesMap& shapesMap = BaseClass::GetShapesMap();
		for (		typename ShapesMap::const_iterator iter = shapesMap.begin();
					iter != shapesMap.end();
					++iter){
			const Shapes& shapes = iter.value();
			int shapesCount = shapes.GetCount();
			for (int shapeIndex = 0; shapeIndex < shapesCount; ++shapeIndex){
				Shape* shapePtr = dynamic_cast<Shape*>(shapes.GetAt(shapeIndex));
				if (shapePtr != NULL){
					if (modelPtr->IsAttached(shapePtr)){
						modelPtr->DetachObserver(shapePtr);
					}

					shapePtr->SetVisible(false);
				}
			}
		}

		return true;
	}
	else{
		return false;
	}
}


// reimplemented (iview::IShapeFactory)

template <class Ui, class Shape, class ShapeModel>
iview::IShape* TShapeParamsGuiCompBase<Ui, Shape, ShapeModel>::CreateShape(const i2d::IObject2d* objectPtr, bool connectToModel) const
{
	Shape* shapePtr = new Shape();

	if (connectToModel){
		imod::IModel* modelPtr = dynamic_cast<imod::IModel*>(const_cast<i2d::IObject2d*>(objectPtr));
		if (modelPtr != NULL){
			if (modelPtr->AttachObserver(shapePtr)){
				shapePtr->SetVisible(true);
			}
		}
	}

	return shapePtr;
}


// protected methods

template <class Ui, class Shape, class ShapeModel>
QString TShapeParamsGuiCompBase<Ui, Shape, ShapeModel>::GetUnitName() const
{
	const imath::IUnitInfo* unitInfoPtr = m_defaultUnitInfoAttrPtr.GetPtr();

	const ShapeModel* objectPtr = BaseClass::GetObjectPtr();
	if (objectPtr != NULL){
		const i2d::ICalibrationProvider* calibrationProviderPtr = dynamic_cast<const i2d::ICalibrationProvider*>(objectPtr);
		if (calibrationProviderPtr != NULL){
			const i2d::ICalibration2d* calibrationPtr = calibrationProviderPtr->GetCalibration();
			if (calibrationPtr != NULL){
				unitInfoPtr = calibrationPtr->GetArgumentUnitInfo();
			}
		}
	}

	if (unitInfoPtr != NULL){
		return unitInfoPtr->GetUnitName();
	}

	return "";
}


// reimplemented (iqt2d::TViewExtenderCompBase)

template <class Ui, class Shape, class ShapeModel>
void TShapeParamsGuiCompBase<Ui, Shape, ShapeModel>::CreateShapes(int /*sceneId*/, Shapes& result)
{
	iview::IShape* shapePtr = CreateShape(BaseClass::GetObjectPtr(), true);
	if (shapePtr != NULL){
		if (m_colorSchemaCompPtr.IsValid()){
			shapePtr->SetUserColorSchema(m_colorSchemaCompPtr.GetPtr());
		}

		result.PushBack(shapePtr);
	}
}


} // namespace iview


#endif // !iview_TShapeParamsGuiCompBase_included


