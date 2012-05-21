#include "iview/CInteractiveViewLayer.h"


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


CInteractiveViewLayer::CInteractiveViewLayer()
:	BaseClass(),
	m_focusedShapePtr(NULL)
{
}


CInteractiveViewLayer::~CInteractiveViewLayer()
{
	DisconnectAllShapes();
}


// reimplemented (iview::CViewLayer)

i2d::CRect CInteractiveViewLayer::RecalcAllShapes(int changeFlag)
{
	i2d::CRect boundingBox = BaseClass::RecalcAllShapes(changeFlag);

	ShapeMap::iterator iter;
	for (iter = m_activeShapes.begin(); iter != m_activeShapes.end(); ++iter){
		iview::IShape* shapePtr = iter.key();
		i2d::CRect& shapeBox = iter.value();

		if (shapePtr->OnDisplayChange(changeFlag)){
			shapeBox = shapePtr->GetBoundingBox();
		}
		boundingBox.Union(shapeBox);
	}

	return boundingBox;
}


i2d::CRect CInteractiveViewLayer::CalcBoundingBox() const
{
	i2d::CRect boundingBox = BaseClass::CalcBoundingBox();

	for (ShapeMap::ConstIterator iter = m_activeShapes.begin(); iter != m_activeShapes.end(); ++iter){
		const i2d::CRect& shapeBox = iter.value();

		boundingBox.Union(shapeBox);
	}

	return boundingBox;
}


// reimplemented (iview::ISelectableLayer)

bool CInteractiveViewLayer::ConnectInteractiveShape(IInteractiveShape* shapePtr)
{
	if (shapePtr->IsSelected()){
		shapePtr->OnConnectDisplay(this);
		shapePtr->OnDisplayChange(CF_ALL);

		i2d::CRect boundingBox = shapePtr->GetBoundingBox();

		m_activeShapes[shapePtr] = boundingBox;

		OnAreaInvalidated(i2d::CRect::GetEmpty(), boundingBox);

		return true;
	}
	else{
		return BaseClass::ConnectShape(shapePtr);
	}
}


int CInteractiveViewLayer::GetUnselectedShapesCount() const
{
	return BaseClass::GetShapesCount();
}


void CInteractiveViewLayer::DrawFocusedShape(QPainter& drawContext)
{
	if (m_focusedShapePtr != NULL){
		m_focusedShapePtr->Draw(drawContext);
	}
}


void CInteractiveViewLayer::BeginDrag(const i2d::CVector2d& position)
{
	for (ShapeMap::iterator iter = m_activeShapes.begin(); iter != m_activeShapes.end(); ++iter){
		IDraggable* draggablePtr = dynamic_cast<IDraggable*>(iter.key());
		if ((draggablePtr != NULL) && draggablePtr->IsDraggable()){
			draggablePtr->BeginDrag(position);
		}
	}
}


void CInteractiveViewLayer::SetDragPosition(const i2d::CVector2d& position)
{
	for (ShapeMap::iterator iter = m_activeShapes.begin(); iter != m_activeShapes.end(); ++iter){
		IDraggable* draggablePtr = dynamic_cast<IDraggable*>(iter.key());
		if ((draggablePtr != NULL) && draggablePtr->IsDraggable()){
			draggablePtr->SetDragPosition(position);
		}
	}
}


void CInteractiveViewLayer::EndDrag()
{
	for (ShapeMap::iterator iter = m_activeShapes.begin(); iter != m_activeShapes.end(); ++iter){
		IDraggable* draggablePtr = dynamic_cast<IDraggable*>(iter.key());
		if ((draggablePtr != NULL) && draggablePtr->IsDraggable()){
			draggablePtr->EndDrag();
		}
	}
}


bool CInteractiveViewLayer::OnMouseButton(istd::CIndex2d position, Qt::MouseButton buttonType, bool downFlag)
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

	QMapIterator<IShape*, i2d::CRect> inactiveIter(m_shapes);
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


bool CInteractiveViewLayer::OnFocusedMouseButton(istd::CIndex2d position, Qt::MouseButton buttonType, bool downFlag)
{
	if (m_focusedShapePtr != NULL){
		return m_focusedShapePtr->OnMouseButton(position, buttonType, downFlag);
	}
	else{
		return false;
	}
}


bool CInteractiveViewLayer::OnFocusedMouseMove(istd::CIndex2d position)
{
	if (m_focusedShapePtr != NULL){
		return m_focusedShapePtr->OnMouseMove(position);
	}
	else{
		return false;
	}
}


ITouchable::TouchState CInteractiveViewLayer::IsTouched(istd::CIndex2d position, IInteractiveShape** shapePtrPtr) const
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
			if (touchState != TS_NONE){
				if (shapePtrPtr != NULL){
					*shapePtrPtr = uiShapePtr;
				}
				return touchState;
			}
		}
	}

	QMapIterator<IShape*, i2d::CRect> inactiveIter(m_shapes);
	inactiveIter.toBack();
	while (inactiveIter.hasPrevious()){
		inactiveIter.previous();

		IInteractiveShape* uiShapePtr = dynamic_cast<iview::IInteractiveShape*>(inactiveIter.key());
		I_ASSERT(uiShapePtr != NULL);
		const i2d::CRect& boundingBox = inactiveIter.value();

		if (boundingBox.IsInside(position)){
			ITouchable::TouchState touchState = uiShapePtr->IsTouched(position);
			if (touchState != TS_NONE){
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

int CInteractiveViewLayer::GetSelectedShapesCount() const
{
	return m_activeShapes.size();
}


void CInteractiveViewLayer::InsertSelectedShapes(SelectedShapes& result) const
{
	for (ShapeMap::ConstIterator iter = m_activeShapes.begin(); iter != m_activeShapes.end(); ++iter){
		IInteractiveShape* uiShape = dynamic_cast<IInteractiveShape*>(iter.key());
		I_ASSERT(uiShape != NULL);	// only iview::IInteractiveShape object are accepted.

		result.insert(uiShape);
	}
}


void CInteractiveViewLayer::DeselectAllShapes()
{
	while (!m_activeShapes.isEmpty()){
		ShapeMap::iterator iter = m_activeShapes.begin();
		I_ASSERT(dynamic_cast<iview::IInteractiveShape*>(iter.key()) != NULL);

		iview::IInteractiveShape* shapePtr = dynamic_cast<iview::IInteractiveShape*>(iter.key());
		I_ASSERT(shapePtr != NULL);
		I_ASSERT(shapePtr->IsSelected());

		shapePtr->SetSelected(false);
	}
}


void CInteractiveViewLayer::OnShapeFocused(IInteractiveShape* shapePtr)
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


void CInteractiveViewLayer::OnShapeDefocused(IInteractiveShape* shapePtr)
{
	I_ASSERT(shapePtr != NULL);

	if (m_focusedShapePtr == shapePtr){
		m_focusedShapePtr = NULL;

		IShapeView* viewPtr = GetViewPtr();
		I_ASSERT(viewPtr != NULL);
		viewPtr->OnShapeDefocused(shapePtr, this);
	}
}


void CInteractiveViewLayer::OnShapeSelected(IInteractiveShape& shape, bool state)
{
	if (state){
		ShapeMap::iterator iter = m_shapes.find(&shape);
		if (iter != m_shapes.end()){
			i2d::CRect newBoundingBox = shape.GetBoundingBox();
			i2d::CRect prevBoundingBox = iter.value();

			m_shapes.erase(iter);
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
			m_shapes[&shape] = newBoundingBox;

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


// reimplemented (iview::IViewLayer)

bool CInteractiveViewLayer::IsShapeConnected(IShape* shapePtr)
{
	return (m_activeShapes.find(shapePtr) != m_activeShapes.end()) || BaseClass::IsShapeConnected(shapePtr);
}


bool CInteractiveViewLayer::ConnectShape(IShape* shapePtr)
{
	IInteractiveShape* uiShapePtr = dynamic_cast<IInteractiveShape*>(shapePtr);
	if (uiShapePtr != NULL){
		return ConnectInteractiveShape(uiShapePtr);
	}

	return false;
}


int CInteractiveViewLayer::GetShapesCount() const
{
	return BaseClass::GetShapesCount() + m_activeShapes.size();
}


void CInteractiveViewLayer::DisconnectAllShapes()
{
	BaseClass::DisconnectAllShapes();

	for (ShapeMap::iterator iter = m_activeShapes.begin(); iter != m_activeShapes.end(); ++iter){
		IShape* shapePtr = iter.key();
		I_ASSERT(shapePtr != NULL);

		OnAreaInvalidated(iter.value(), i2d::CRect::GetEmpty());
		shapePtr->OnDisconnectDisplay(this);
	}

	m_activeShapes.clear();
}


void CInteractiveViewLayer::DrawShapes(QPainter& drawContext)
{
	BaseClass::DrawShapes(drawContext);

	for (ShapeMap::iterator pos = m_activeShapes.begin(); pos != m_activeShapes.end(); ++pos){
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

void CInteractiveViewLayer::OnChangeShape(IShape* shapePtr)
{
	I_ASSERT(shapePtr != NULL);

	ShapeMap::iterator iter = m_activeShapes.find(shapePtr);
	if (iter != m_activeShapes.end()){
		OnChangeShapeElement(iter);

		return;
	}

	BaseClass::OnChangeShape(shapePtr);
}


bool CInteractiveViewLayer::DisconnectShape(IShape* shapePtr)
{
	I_ASSERT(shapePtr != NULL);

	if (BaseClass::DisconnectShape(shapePtr)){
		return true;
	}

	ShapeMap::Iterator iter = m_activeShapes.find(shapePtr);
	if (iter != m_activeShapes.end()){
		if (m_focusedShapePtr == shapePtr){
			IInteractiveShape* uiShapePtr = dynamic_cast<IInteractiveShape*>(shapePtr);
			I_ASSERT(uiShapePtr != NULL);
			OnShapeDefocused(uiShapePtr);
		}
		DisconnectShapeElement(m_activeShapes, iter);

		return true;
	}

	return false;
}


// reimplemented (iview::ITouchable)

ITouchable::TouchState CInteractiveViewLayer::IsTouched(istd::CIndex2d position) const
{
	if (IsVisible()){
		for (ShapeMap::const_iterator iter = m_activeShapes.begin(); iter != m_activeShapes.end(); ++iter){
			const iview::IShape* shapePtr = iter.key();
			const i2d::CRect& boundingBox = iter.value();
			if (boundingBox.IsInside(position) && shapePtr->IsVisible()){
				ITouchable::TouchState touchState = shapePtr->IsTouched(position);
				if (touchState > ITouchable::TS_NONE){
					return touchState;
				}
			}
		}
	}

	return BaseClass::IsTouched(position);
}


QString CInteractiveViewLayer::GetShapeDescriptionAt(istd::CIndex2d position) const
{
	if (IsVisible()){
		for (ShapeMap::const_iterator iter = m_activeShapes.begin(); iter != m_activeShapes.end(); ++iter){
			const iview::IShape* shapePtr = iter.key();
			const i2d::CRect& boundingBox = iter.value();
			if (boundingBox.IsInside(position) && shapePtr->IsVisible()){
				ITouchable::TouchState touchState = shapePtr->IsTouched(position);
				if (touchState > ITouchable::TS_NONE){
					return shapePtr->GetShapeDescriptionAt(position);
				}
			}
		}
	}

	return BaseClass::GetShapeDescriptionAt(position);
}


} // namespace iview


