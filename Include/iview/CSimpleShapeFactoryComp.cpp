#include "iview/CSimpleShapeFactoryComp.h"


// ACF includes
#include "i2d/CAnnulusSegment.h"
#include "i2d/CTubePolyline.h"

#include "iview/CInteractiveAnnulusSegmentShape.h"
#include "iview/CInteractiveCircleShape.h"
#include "iview/CInteractiveRectangleShape.h"
#include "iview/CInteractiveLineShape.h"
#include "iview/CInteractiveTubePolylineShape.h"


namespace iview
{


// public methds

// reimplemented (IShapeFactory)

IShape* CSimpleShapeFactoryComp::CreateShape(const i2d::IObject2d& object, bool connectToModel) const
{
	istd::TDelPtr<IShape> shapePtr;

	const i2d::CAnnulusSegment* annulusSegmentPtr = dynamic_cast<const i2d::CAnnulusSegment*>(&object);
	if (annulusSegmentPtr != NULL){
		shapePtr.SetPtr(new iview::CInteractiveAnnulusSegmentShape);
	}
	else{
		const i2d::CAnnulus* annulusPtr = dynamic_cast<const i2d::CAnnulus*>(&object);
		if (annulusPtr != NULL){
			shapePtr.SetPtr(new iview::CInteractiveAnnulusShape);
		}
	}

	const i2d::CCircle* circlePtr = dynamic_cast<const i2d::CCircle*>(&object);
	if (circlePtr != NULL){
		shapePtr.SetPtr(new iview::CInteractiveCircleShape);
	}

	const i2d::CRectangle* rectPtr = dynamic_cast<const i2d::CRectangle*>(&object);
	if (rectPtr != NULL){
		shapePtr.SetPtr(new iview::CInteractiveRectangleShape);
	}

	const i2d::CLine2d* linePtr = dynamic_cast<const i2d::CLine2d*>(&object);
	if (linePtr != NULL){
		shapePtr.SetPtr(new iview::CInteractiveLineShape);
	}

	const i2d::CTubePolyline* tubePolylinePtr = dynamic_cast<const i2d::CTubePolyline*>(&object);
	if (tubePolylinePtr != NULL){
		shapePtr.SetPtr(new iview::CInteractiveTubePolylineShape);
	}
	else{
		const i2d::CPolyline* polylinePtr = dynamic_cast<const i2d::CPolyline*>(&object);
		if (polylinePtr != NULL){
			shapePtr.SetPtr(new iview::CInteractivePolylineShape);
		}
		else{
			const i2d::CPolygon* polygonPtr = dynamic_cast<const i2d::CPolygon*>(&object);
			if (polygonPtr != NULL){
				shapePtr.SetPtr(new iview::CInteractivePolygonShape);
			}
		}
	}

	const i2d::CPosition2d* positionPtr = dynamic_cast<const i2d::CPosition2d*>(&object);
	if (positionPtr != NULL){
		shapePtr.SetPtr(new iview::CInteractivePinShape);
	}

	if (connectToModel){
		imod::IModel* modelPtr = dynamic_cast<imod::IModel*>(const_cast<i2d::IObject2d*>(&object));
		if (modelPtr != NULL){
			if (!modelPtr->AttachObserver(shapePtr.GetPtr())){
				shapePtr.Reset();
			}
		}
	}

	return shapePtr.PopPtr();
}


} // namespace iview


