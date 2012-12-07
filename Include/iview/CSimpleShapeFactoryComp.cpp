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
	istd::TDelPtr<IShape> shapePtr(CreateShapeInstance(object));

	if (shapePtr.IsValid()){
		if (m_colorSchemaCompPtr.IsValid()){
			shapePtr->SetUserColorShema(m_colorSchemaCompPtr.GetPtr());
		}

		if (connectToModel){
			imod::IModel* modelPtr = dynamic_cast<imod::IModel*>(const_cast<i2d::IObject2d*>(&object));
			if (modelPtr != NULL){
				if (!modelPtr->AttachObserver(shapePtr.GetPtr())){
					return NULL;
				}
			}
		}
	}

	return shapePtr.PopPtr();
}


// protected methods

IShape* CSimpleShapeFactoryComp::CreateShapeInstance(const i2d::IObject2d& object) const
{
	const i2d::CAnnulusSegment* annulusSegmentPtr = dynamic_cast<const i2d::CAnnulusSegment*>(&object);
	if (annulusSegmentPtr != NULL){
		iview::CInteractiveAnnulusSegmentShape* objectShapePtr = new iview::CInteractiveAnnulusSegmentShape();
		objectShapePtr->SetEditablePosition(*m_useInteractiveShapesAttrPtr);
		objectShapePtr->SetEditableRadius(*m_useInteractiveShapesAttrPtr);
		objectShapePtr->SetEditableAngles(*m_useInteractiveShapesAttrPtr);

		return objectShapePtr;
	}

	const i2d::CAnnulus* annulusPtr = dynamic_cast<const i2d::CAnnulus*>(&object);
	if (annulusPtr != NULL){
		iview::CInteractiveAnnulusShape* objectShapePtr = new iview::CInteractiveAnnulusShape();
		objectShapePtr->SetEditablePosition(*m_useInteractiveShapesAttrPtr);
		objectShapePtr->SetEditableRadius(*m_useInteractiveShapesAttrPtr);

		return objectShapePtr;
	}

	const i2d::CCircle* circlePtr = dynamic_cast<const i2d::CCircle*>(&object);
	if (circlePtr != NULL){
		iview::CInteractiveCircleShape* objectShapePtr = new iview::CInteractiveCircleShape();
		objectShapePtr->SetEditablePosition(*m_useInteractiveShapesAttrPtr);
		objectShapePtr->SetEditableRadius(*m_useInteractiveShapesAttrPtr);

		return objectShapePtr;
	}

	const i2d::CRectangle* rectPtr = dynamic_cast<const i2d::CRectangle*>(&object);
	if (rectPtr != NULL){
		iview::CInteractiveRectangleShape* objectShapePtr = new iview::CInteractiveRectangleShape();
		objectShapePtr->SetEditablePosition(*m_useInteractiveShapesAttrPtr);

		return objectShapePtr;
	}

	const i2d::CLine2d* linePtr = dynamic_cast<const i2d::CLine2d*>(&object);
	if (linePtr != NULL){
		iview::CInteractiveLineShape* objectShapePtr = new iview::CInteractiveLineShape();
		objectShapePtr->SetEditablePosition(*m_useInteractiveShapesAttrPtr);

		return objectShapePtr;
	}

	const i2d::CTubePolyline* tubePolylinePtr = dynamic_cast<const i2d::CTubePolyline*>(&object);
	if (tubePolylinePtr != NULL){
		iview::CInteractiveTubePolylineShape* objectShapePtr = new iview::CInteractiveTubePolylineShape();
		objectShapePtr->SetEditablePosition(*m_useInteractiveShapesAttrPtr);

		return objectShapePtr;
	}

	const i2d::CPolyline* polylinePtr = dynamic_cast<const i2d::CPolyline*>(&object);
	if (polylinePtr != NULL){
		iview::CInteractivePolylineShape* objectShapePtr = new iview::CInteractivePolylineShape();
		objectShapePtr->SetEditablePosition(*m_useInteractiveShapesAttrPtr);
		objectShapePtr->SetEditableRotation(*m_useInteractiveShapesAttrPtr);
		objectShapePtr->SetEditableHeight(*m_useInteractiveShapesAttrPtr);
		objectShapePtr->SetEditableWidth(*m_useInteractiveShapesAttrPtr);

		return objectShapePtr;
	}

	const i2d::CPolygon* polygonPtr = dynamic_cast<const i2d::CPolygon*>(&object);
	if (polygonPtr != NULL){
		iview::CInteractivePolygonShape* objectShapePtr = new iview::CInteractivePolygonShape();
		objectShapePtr->SetEditablePosition(*m_useInteractiveShapesAttrPtr);
		objectShapePtr->SetEditableRotation(*m_useInteractiveShapesAttrPtr);
		objectShapePtr->SetEditableHeight(*m_useInteractiveShapesAttrPtr);
		objectShapePtr->SetEditableWidth(*m_useInteractiveShapesAttrPtr);

		return objectShapePtr;
	}

	const i2d::CPosition2d* positionPtr = dynamic_cast<const i2d::CPosition2d*>(&object);
	if (positionPtr != NULL){
		iview::CInteractivePinShape* objectShapePtr = new iview::CInteractivePinShape();
		objectShapePtr->SetEditablePosition(*m_useInteractiveShapesAttrPtr);

		return objectShapePtr;
	}

	return NULL;
}


} // namespace iview


