#include "iview/CSelectableLayerBase.h"


// Qt includes
#include <QtCore/QMapIterator>
#include <QtGui/QPainter>

// ACF includes
#include "iqt/iqt.h"

#include "iview/IInteractiveShape.h"
#include "iview/IShapeView.h"
#include "iview/IDraggable.h"
#include "iview/IShapeView.h"
#include "iview/IViewEventObserver.h"
#include "iview/CScreenTransform.h"


namespace iview
{


CSelectableLayerBase::CSelectableLayerBase()
	:BaseClass(),
	m_focusedShapePtr(NULL)
{
}


CSelectableLayerBase::~CSelectableLayerBase()
{
	DisconnectAllShapes();
}


// reimplemented (iview::TLayerBase)

void CSelectableLayerBase::CalcBoundingBox(i2d::CRect& result) const
{
	result.Reset();

	ShapeMap::const_iterator iter;
	for (iter = m_activeShapes.begin(); iter != m_activeShapes.end(); ++iter){
		const i2d::CRect& boundingBox = iter.value();
		result.Union(boundingBox);
	}

	for (iter = m_inactiveShapes.begin(); iter != m_inactiveShapes.end(); ++iter){
		const i2d::CRect& boundingBox = iter.value();
		result.Union(boundingBox);
	}
}


// reimplemented (iview::ISelectableLayer)

bool CSelectableLayerBase::ConnectInteractiveShape(IInteractiveShape* shapePtr)
{
	shapePtr->OnConnectDisplay(this);
	shapePtr->OnDisplayChange(CF_ALL);

	i2d::CRect boundingBox = shapePtr->GetBoundingBox();

	if (shapePtr->IsSelected()){
		m_activeShapes[shapePtr] = boundingBox;
	}
	else{
		m_inactiveShapes[shapePtr] = boundingBox;
	}

	OnAreaInvalidated(i2d::CRect::GetEmpty(), boundingBox);

	return true;
}


int CSelectableLayerBase::GetUnselectedShapesCount() const
{
	return int(m_inactiveShapes.size());
}


void CSelectableLayerBase::DrawFocusedShape(QPainter& drawContext)
{
	if (m_focusedShapePtr != NULL){
		m_focusedShapePtr->Draw(drawContext);
	}
}


void CSelectableLayerBase::BeginDrag(const i2d::CVector2d& position)
{
	for (ShapeMap::iterator iter = m_activeShapes.begin(); iter != m_activeShapes.end(); ++iter){
		IDraggable* draggablePtr = dynamic_cast<IDraggable*>(iter.key());
		if ((draggablePtr != NULL) && draggablePtr->IsDraggable()){
			draggablePtr->BeginDrag(position);
		}
	}
}


void CSelectableLayerBase::SetDragPosition(const i2d::CVector2d& position)
{
	for (ShapeMap::iterator iter = m_activeShapes.begin(); iter != m_activeShapes.end(); ++iter){
		IDraggable* draggablePtr = dynamic_cast<IDraggable*>(iter.key());
		if ((draggablePtr != NULL) && draggablePtr->IsDraggable()){
			draggablePtr->SetDragPosition(position);
		}
	}
}


void CSelectableLayerBase::EndDrag()
{
	for (ShapeMap::iterator iter = m_activeShapes.begin(); iter != m_activeShapes.end(); ++iter){
		IDraggable* draggablePtr = dynamic_cast<IDraggable*>(iter.key());
		if ((draggablePtr != NULL) && draggablePtr->IsDraggable()){
			draggablePtr->EndDrag();
		}
	}
}


bool CSelectableLayerBase::OnMouseButton(istd::CIndex2d position, Qt::MouseButton buttonType, bool downFlag)
{
	IShapeView* viewPtr = GetViewPtr();
	I_ASSERT(viewPtr != NULL);

	IViewEventObserver* listenerPtr = dynamic_cast<IViewEventObserver*>(viewPtr);

	const iview::CScreenTransform& transform = GetTransform();

	QMapIterator<IShape*, i2d::CRect> iter(m_activeShapes);
	iter.toBack();
	while (iter.hasPrevious()){
		iter.previous();

		iview::IInteractiveShape* uiShapePtr = dynamic_cast<iview::IInteractiveShape*>(iter.key());
		const i2d::CRect& boundingBox = iter.value();
		if (			(uiShapePtr != m_focusedShapePtr) &&
						uiShapePtr->IsVisible() &&
						boundingBox.IsInside(position)){
			ITouchable::TouchState touchState = uiShapePtr->IsTouched(position);
			if (touchState != IInteractiveShape::TS_NONE){
				if ((listenerPtr != NULL) && listenerPtr->OnMouseButton(*viewPtr, position, buttonType, downFlag, uiShapePtr)){
					return true;
				}

				if (buttonType == Qt::LeftButton){
					if (touchState == IInteractiveShape::TS_DRAGGABLE){
						if (downFlag){
							const i2d::CVector2d& pos = transform.GetClientPosition(position);
							IDraggable* draggablePtr = dynamic_cast<IDraggable*>(viewPtr);
							if ((draggablePtr != NULL) && draggablePtr->IsDraggable()){
								draggablePtr->BeginDrag(pos);
							}
						}
					}
					else{
						uiShapePtr->OnMouseButton(position, buttonType, downFlag);
					}
				}

				return true;
			}
		}
	}

	int keysState = GetKeysState();

	QMapIterator<IShape*, i2d::CRect> inactiveIter(m_inactiveShapes);
	inactiveIter.toBack();
	while (inactiveIter.hasPrevious()){
		inactiveIter.previous();

		iview::IInteractiveShape* uiShapePtr = dynamic_cast<iview::IInteractiveShape*>(inactiveIter.key());
		const i2d::CRect& boundingBox = inactiveIter.value();
		if (			(uiShapePtr != m_focusedShapePtr) &&
						uiShapePtr->IsVisible() &&
						boundingBox.IsInside(position)){
			ITouchable::TouchState touchState = uiShapePtr->IsTouched(position);
			if (touchState != IInteractiveShape::TS_NONE){
				if ((listenerPtr != NULL) && listenerPtr->OnMouseButton(*viewPtr, position, buttonType, downFlag, uiShapePtr)){
					return true;
				}

				bool consumed = false;
				if (downFlag){
					if (!(viewPtr->IsMultiselectable() && ((keysState & Qt::ControlModifier) != 0))){
						viewPtr->DeselectAllShapes();
						consumed = true;
					}
					if (!uiShapePtr->IsSelected()){
						uiShapePtr->SetSelected();
						consumed = true;
					}
					const i2d::CVector2d& pos = transform.GetClientPosition(position);

					viewPtr->UpdateMousePointer();

					if (touchState == IInteractiveShape::TS_DRAGGABLE){
						IDraggable*  draggablePtr = dynamic_cast<IDraggable*>(viewPtr);
						if ((draggablePtr != NULL) && draggablePtr->IsDraggable()){
							draggablePtr->BeginDrag(pos);
						}
					}
				}
				return consumed;
			}
		}
	}

	return false;
}


bool CSelectableLayerBase::OnFocusedMouseButton(istd::CIndex2d position, Qt::MouseButton buttonType, bool downFlag)
{
	if (m_focusedShapePtr != NULL){
		return m_focusedShapePtr->OnMouseButton(position, buttonType, downFlag);
	}
	else{
		return false;
	}
}


bool CSelectableLayerBase::OnFocusedMouseMove(istd::CIndex2d position)
{
	if (m_focusedShapePtr != NULL){
		return m_focusedShapePtr->OnMouseMove(position);
	}
	else{
		return false;
	}
}


ITouchable::TouchState CSelectableLayerBase::IsTouched(istd::CIndex2d position, IInteractiveShape** shapePtrPtr) const
{
	QMapIterator<IShape*, i2d::CRect> iter(m_activeShapes);
	iter.toBack();
	while (iter.hasPrevious()){
		iter.previous();

		IInteractiveShape* uiShapePtr = dynamic_cast<iview::IInteractiveShape*>(iter.key());
		I_ASSERT(uiShapePtr != NULL);
		const i2d::CRect& boundingBox = iter.value();

		if (boundingBox.IsInside(position)){
			ITouchable::TouchState touchState = uiShapePtr->IsTouched(position);
			if (touchState != iview::IInteractiveShape::TS_NONE){
				if (shapePtrPtr != NULL){
					*shapePtrPtr = uiShapePtr;
				}
				return touchState;
			}
		}
	}

	QMapIterator<IShape*, i2d::CRect> inactiveIter(m_inactiveShapes);
	inactiveIter.toBack();
	while (inactiveIter.hasPrevious()){
		inactiveIter.previous();

		IInteractiveShape* uiShapePtr = dynamic_cast<iview::IInteractiveShape*>(inactiveIter.key());
		I_ASSERT(uiShapePtr != NULL);
		const i2d::CRect& boundingBox = inactiveIter.value();

		if (boundingBox.IsInside(position)){
			ITouchable::TouchState touchState = uiShapePtr->IsTouched(position);
			if (touchState != iview::IInteractiveShape::TS_NONE){
				if (shapePtrPtr != NULL){
					*shapePtrPtr = uiShapePtr;
				}
				return touchState;
			}
		}
	}

	return IInteractiveShape::TS_NONE;
}


// reimplemented (iview::ISelectable)

int CSelectableLayerBase::GetSelectedShapesCount() const
{
	return int(m_activeShapes.size());
}


void CSelectableLayerBase::InsertSelectedShapes(SelectedShapes& result) const
{
	for (ShapeMap::const_iterator iter = m_activeShapes.begin(); iter != m_activeShapes.end(); ++iter){
		IInteractiveShape* uiShape = dynamic_cast<IInteractiveShape*>(iter.key());
		I_ASSERT(uiShape != NULL);	// only iview::IInteractiveShape object are accepted.

		result.insert(uiShape);
	}
}


void CSelectableLayerBase::DeselectAllShapes()
{
	while (!m_activeShapes.empty()){
		ShapeMap::iterator iter = m_activeShapes.begin();
		I_ASSERT(dynamic_cast<iview::IInteractiveShape*>(iter.key()) != NULL);

		iview::IInteractiveShape* shapePtr = dynamic_cast<iview::IInteractiveShape*>(iter.key());
		I_ASSERT(shapePtr != NULL);
		I_ASSERT(shapePtr->IsSelected());

		shapePtr->SetSelected(false);
	}
}


void CSelectableLayerBase::OnShapeFocused(IInteractiveShape* shapePtr)
{
	I_ASSERT(shapePtr != NULL);
	I_ASSERT(m_activeShapes.find(shapePtr) != m_activeShapes.end());

	if (m_focusedShapePtr != shapePtr){
		m_focusedShapePtr = shapePtr;

		IShapeView* viewPtr = GetViewPtr();
		I_ASSERT(viewPtr != NULL);
		viewPtr->OnShapeFocused(shapePtr, this);
	}
}


void CSelectableLayerBase::OnShapeDefocused(IInteractiveShape* shapePtr)
{
	I_ASSERT(shapePtr != NULL);

	if (m_focusedShapePtr == shapePtr){
		m_focusedShapePtr = NULL;

		IShapeView* viewPtr = GetViewPtr();
		I_ASSERT(viewPtr != NULL);
		viewPtr->OnShapeDefocused(shapePtr, this);
	}
}


void CSelectableLayerBase::OnShapeSelected(IInteractiveShape& shape, bool state)
{
	if (state){
		ShapeMap::iterator iter = m_inactiveShapes.find(&shape);
		if (iter != m_inactiveShapes.end()){
			i2d::CRect newBoundingBox = shape.GetBoundingBox();
			i2d::CRect prevBoundingBox = iter.value();

			m_inactiveShapes.erase(iter);
			m_activeShapes[&shape] = newBoundingBox;

			OnAreaInvalidated(prevBoundingBox, newBoundingBox);
		}
		else{
			I_CRITICAL();
		}
	}
	else{
		ShapeMap::iterator iter = m_activeShapes.find(&shape);
		if (iter != m_activeShapes.end()){
			if (m_focusedShapePtr == &shape){
				OnShapeDefocused(&shape);
			}
			i2d::CRect newBoundingBox = shape.GetBoundingBox();
			i2d::CRect prevBoundingBox = iter.value();

			m_activeShapes.erase(iter);
			m_inactiveShapes[&shape] = newBoundingBox;

			OnAreaInvalidated(prevBoundingBox, newBoundingBox);
		}
		else{
			I_CRITICAL();
		}
	}
	ISelectable* observerPtr = dynamic_cast<ISelectable*>(GetParentDisplayPtr());
	if (observerPtr != NULL){
		observerPtr->OnShapeSelected(shape, state);
	}
}


// reimplemented (iview::ILayer)

bool CSelectableLayerBase::IsShapeConnected(IShape* shapePtr)
{
	return	(m_activeShapes.find(shapePtr) != m_activeShapes.end()) ||
			(m_inactiveShapes.find(shapePtr) != m_inactiveShapes.end());
}


void CSelectableLayerBase::ConnectShape(IShape* shapePtr)
{
	IInteractiveShape* uiShapePtr = dynamic_cast<IInteractiveShape*>(shapePtr);
	if (uiShapePtr != NULL){
		ConnectInteractiveShape(uiShapePtr);
	}
}


int CSelectableLayerBase::GetShapesCount() const
{
	return GetUnselectedShapesCount() + GetSelectedShapesCount();
}


void CSelectableLayerBase::UpdateAllShapes(int changeFlag)
{
	i2d::CRect& boundingBox = GetBoundingBoxRef();
	boundingBox.Reset();
	ShapeMap::iterator iter;
	for (iter = m_activeShapes.begin(); iter != m_activeShapes.end(); ++iter){
		iview::IShape* shapePtr = iter.key();
		i2d::CRect& shapeBox = iter.value();

		if (shapePtr->OnDisplayChange(changeFlag)){
			shapeBox = shapePtr->GetBoundingBox();
		}
		boundingBox.Union(shapeBox);
	}

	for (iter = m_inactiveShapes.begin(); iter != m_inactiveShapes.end(); ++iter){
		iview::IShape* shapePtr = iter.key();
		i2d::CRect& shapeBox = iter.value();

		if (shapePtr->OnDisplayChange(changeFlag)){
			shapeBox = shapePtr->GetBoundingBox();
		}
		boundingBox.Union(shapeBox);
	}

	SetBoundingBoxValid();
}


void CSelectableLayerBase::DisconnectAllShapes()
{
	ShapeMap::iterator iter;
	for (iter = m_inactiveShapes.begin(); iter != m_inactiveShapes.end(); ++iter){
		IShape* shapePtr = iter.key();
		I_ASSERT(shapePtr != NULL);

		OnAreaInvalidated(iter.value(), i2d::CRect::GetEmpty());
		shapePtr->OnDisconnectDisplay(this);
	}

	for (iter = m_activeShapes.begin(); iter != m_activeShapes.end(); ++iter){
		IShape* shapePtr = iter.key();
		I_ASSERT(shapePtr != NULL);

		OnAreaInvalidated(iter.value(), i2d::CRect::GetEmpty());
		shapePtr->OnDisconnectDisplay(this);
	}

	m_inactiveShapes.clear();
	m_activeShapes.clear();
}


void CSelectableLayerBase::DrawShapes(QPainter& drawContext)
{
	ShapeMap::iterator pos;
	for (pos = m_inactiveShapes.begin(); pos != m_inactiveShapes.end(); ++pos){
		iview::IShape* shapePtr = pos.key();
		if (shapePtr->IsVisible()){
			const i2d::CRect& boundingBox = pos.value();
			i2d::CRect updateRect = iqt::GetCRect(drawContext.clipRegion().boundingRect());

			if (!updateRect.IsOutside(boundingBox)){
				shapePtr->Draw(drawContext);
			}
		}
	}

	for (pos = m_activeShapes.begin(); pos != m_activeShapes.end(); ++pos){
		iview::IShape* shapePtr = pos.key();
		if (shapePtr->IsVisible() && (shapePtr != m_focusedShapePtr)){
			const i2d::CRect& boundingBox = pos.value();
			i2d::CRect updateRect = iqt::GetCRect(drawContext.clipRegion().boundingRect());

			if (!updateRect.IsOutside(boundingBox)){
				shapePtr->Draw(drawContext);
			}
		}
	}
}


// reimplemented (iview::IShapeObserver)

void CSelectableLayerBase::OnChangeShape(IShape* shapePtr)
{
	I_ASSERT(shapePtr != NULL);

	ShapeMap::iterator iter = m_activeShapes.find(shapePtr);
	if (iter != m_activeShapes.end()){
		OnChangeShapeElement(iter);
	}
	
	iter = m_inactiveShapes.find(shapePtr);
	if (iter != m_inactiveShapes.end()){
		OnChangeShapeElement(iter);
	}
}


void CSelectableLayerBase::DisconnectShape(IShape* shapePtr)
{
	I_ASSERT(shapePtr != NULL);

	ShapeMap::iterator iter = m_inactiveShapes.find(shapePtr);
	if (iter != m_inactiveShapes.end()){
		DisconnectShapeElement(m_inactiveShapes, iter);
		return;
	}

	iter = m_activeShapes.find(shapePtr);
	if (iter != m_activeShapes.end()){
		if (m_focusedShapePtr == shapePtr){
			IInteractiveShape* uiShapePtr = dynamic_cast<IInteractiveShape*>(shapePtr);
			I_ASSERT(uiShapePtr != NULL);
			OnShapeDefocused(uiShapePtr);
		}
		DisconnectShapeElement(m_activeShapes, iter);
		return;
	}

	I_CRITICAL();	// shape was not connected to this layer!
}


} // namespace iview


