#include "iview/CSimpleShapeFactoryComp.h"


// ACF includes
#include "i2d/CAnnulusSegment.h"
#include "i2d/CTubePolyline.h"
#include "i2d/CArc.h"
#include "iview/IViewLayer.h"
#include "iview/CAnnulusSegmentShape.h"
#include "iview/CCircleShape.h"
#include "iview/CRectangleShape.h"
#include "iview/CLineShape.h"
#include "iview/CTubePolylineShape.h"
#include "iview/CLabelShape.h"
#include "iview/CArcShape.h"


namespace iview
{


// public methds

// reimplemented (iview::IShapeFactory)

IShape* CSimpleShapeFactoryComp::CreateShape(const i2d::IObject2d* objectPtr, bool connectToModel) const
{
	if (objectPtr == NULL){
		return NULL;
	}

	istd::TDelPtr<CInteractiveShapeBase> shapePtr(CreateShapeInstance(*objectPtr));

	if (shapePtr.IsValid()){
		shapePtr->SetEditablePosition(*m_useInteractiveShapesAttrPtr);
		if (!*m_useInteractiveShapesAttrPtr){
			shapePtr->AssignToLayer(IViewLayer::LT_INACTIVE);
		}

		if (m_colorSchemaCompPtr.IsValid()){
			shapePtr->SetUserColorSchema(m_colorSchemaCompPtr.GetPtr());
		}

		if (connectToModel){
			imod::IModel* modelPtr = dynamic_cast<imod::IModel*>(const_cast<i2d::IObject2d*>(objectPtr));
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

CInteractiveShapeBase* CSimpleShapeFactoryComp::CreateShapeInstance(const i2d::IObject2d& object) const
{
	const i2d::CAnnulusSegment* annulusSegmentPtr = dynamic_cast<const i2d::CAnnulusSegment*>(&object);
	if (annulusSegmentPtr != NULL){
		iview::CAnnulusSegmentShape* objectShapePtr = new iview::CAnnulusSegmentShape();
		objectShapePtr->SetEditableRadius(*m_useInteractiveShapesAttrPtr);
		objectShapePtr->SetEditableAngles(*m_useInteractiveShapesAttrPtr);

		return objectShapePtr;
	}

	const i2d::CAnnulus* annulusPtr = dynamic_cast<const i2d::CAnnulus*>(&object);
	if (annulusPtr != NULL){
		iview::CAnnulusShape* objectShapePtr = new iview::CAnnulusShape();
		objectShapePtr->SetEditableRadius(*m_useInteractiveShapesAttrPtr);

		return objectShapePtr;
	}

	const i2d::CCircle* circlePtr = dynamic_cast<const i2d::CCircle*>(&object);
	if (circlePtr != NULL){
		iview::CCircleShape* objectShapePtr = new iview::CCircleShape();
		objectShapePtr->SetEditableRadius(*m_useInteractiveShapesAttrPtr);

		return objectShapePtr;
	}

	const i2d::CRectangle* rectPtr = dynamic_cast<const i2d::CRectangle*>(&object);
	if (rectPtr != NULL){
		iview::CRectangleShape* objectShapePtr = new iview::CRectangleShape();

		return objectShapePtr;
	}

	const i2d::CLine2d* linePtr = dynamic_cast<const i2d::CLine2d*>(&object);
	if (linePtr != NULL){
		iview::CLineShape* objectShapePtr = new iview::CLineShape();

		return objectShapePtr;
	}

	const i2d::CTubePolyline* tubePolylinePtr = dynamic_cast<const i2d::CTubePolyline*>(&object);
	if (tubePolylinePtr != NULL){
		iview::CTubePolylineShape* objectShapePtr = new iview::CTubePolylineShape();

		return objectShapePtr;
	}

	const i2d::CPolyline* polylinePtr = dynamic_cast<const i2d::CPolyline*>(&object);
	if (polylinePtr != NULL){
		iview::CPolylineShape* objectShapePtr = new iview::CPolylineShape();
		objectShapePtr->SetEditableRotation(*m_useInteractiveShapesAttrPtr);
		objectShapePtr->SetEditableHeight(*m_useInteractiveShapesAttrPtr);
		objectShapePtr->SetEditableWidth(*m_useInteractiveShapesAttrPtr);

		return objectShapePtr;
	}

	const i2d::CPolygon* polygonPtr = dynamic_cast<const i2d::CPolygon*>(&object);
	if (polygonPtr != NULL){
		iview::CPolygonShape* objectShapePtr = new iview::CPolygonShape();
		objectShapePtr->SetEditableRotation(*m_useInteractiveShapesAttrPtr);
		objectShapePtr->SetEditableHeight(*m_useInteractiveShapesAttrPtr);
		objectShapePtr->SetEditableWidth(*m_useInteractiveShapesAttrPtr);

		return objectShapePtr;
	}

	const i2d::CLabel* labelPtr = dynamic_cast<const i2d::CLabel*>(&object);
	if (labelPtr != NULL){
		iview::CLabelShape* objectShapePtr = new iview::CLabelShape();

		return objectShapePtr;
	}

	const i2d::CPosition2d* positionPtr = dynamic_cast<const i2d::CPosition2d*>(&object);
	if (positionPtr != NULL){
		iview::CPinShape* objectShapePtr = new iview::CPinShape();

		return objectShapePtr;
	}

	const i2d::CArc* arcPtr = dynamic_cast<const i2d::CArc*>(&object);
	if (arcPtr != NULL){
		iview::CArcShape* objectShapePtr = new iview::CArcShape();

		return objectShapePtr;
	}

	return NULL;
}


} // namespace iview


