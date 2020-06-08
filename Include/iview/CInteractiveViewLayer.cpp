#include <iview/CInteractiveViewLayer.h>


// Qt includes
#include <QtGui/QPainter>

// ACF includes
#include <iqt/iqt.h>

#include <iview/IInteractiveShape.h>
#include <iview/IShapeView.h>
#include <iview/IDraggable.h>
#include <iview/IShapeView.h>
#include <iview/IViewEventObserver.h>
#include <iview/CScreenTransform.h>


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


IInteractiveShape* CInteractiveViewLayer::GetFirstActiveShape() const
{
	if (!m_activeShapes.isEmpty()){
		ShapeList::const_iterator iter;
		for (iter = m_activeShapes.constBegin(); iter != m_activeShapes.constEnd(); ++iter){
			iview::IInteractiveShape* shapePtr = dynamic_cast<iview::IInteractiveShape*>(iter->shapePtr);
			if (shapePtr != NULL)
				return shapePtr;
		}
	}

	return NULL;
}


// reimplemented (iview::CViewLayer)

i2d::CRect CInteractiveViewLayer::RecalcAllShapes(const istd::IChangeable::ChangeSet& changeSet)
{
	i2d::CRect boundingBox = BaseClass::RecalcAllShapes(changeSet);

	ShapeList::iterator iter;
	for (iter = m_activeShapes.begin(); iter != m_activeShapes.end(); ++iter){
		iview::IShape* shapePtr = iter->shapePtr;
		i2d::CRect& shapeBox = iter->box;

		if (shapePtr->OnDisplayChange(changeSet)){
			shapeBox = shapePtr->GetBoundingBox();
		}
		boundingBox.Union(shapeBox);
	}

	return boundingBox;
}


i2d::CRect CInteractiveViewLayer::CalcBoundingBox() const
{
	i2d::CRect boundingBox = BaseClass::CalcBoundingBox();

	for (ShapeList::ConstIterator iter = m_activeShapes.begin(); iter != m_activeShapes.end(); ++iter){
		const i2d::CRect& shapeBox = iter->box;

		boundingBox.Union(shapeBox);
	}

	return boundingBox;
}


// reimplemented (iview::ISelectableLayer)

bool CInteractiveViewLayer::ConnectInteractiveShape(IInteractiveShape* shapePtr)
{
	if (shapePtr->IsSelected()){
		shapePtr->OnConnectDisplay(this);

		static const istd::IChangeable::ChangeSet changeSet(istd::IChangeable::CF_ALL_DATA);
		shapePtr->OnDisplayChange(changeSet);

		i2d::CRect boundingBox = shapePtr->GetBoundingBox();

		ShapeList::iterator iter = m_activeShapes.begin();
		for (; iter != m_activeShapes.end(); ++iter) {
			if (iter->shapePtr == shapePtr) {
				break;
			}
		}

		if (iter != m_activeShapes.end()) {
			iter->shapePtr = shapePtr;
			iter->box = boundingBox;
		}
		else {
			m_activeShapes.push_back(ShapeWithBoundingBox(shapePtr, boundingBox));
		}

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


bool CInteractiveViewLayer::OnMouseButton(istd::CIndex2d position, Qt::MouseButton buttonType, bool downFlag)
{
	IShapeView* viewPtr = GetViewPtr();
	Q_ASSERT(viewPtr != NULL);

	IViewEventObserver* listenerPtr = dynamic_cast<IViewEventObserver*>(viewPtr);

	QVectorIterator<ShapeWithBoundingBox> activeShapeIterator(m_activeShapes);
	activeShapeIterator.toBack();

	while (activeShapeIterator.hasPrevious()){
		const ShapeWithBoundingBox& shape = activeShapeIterator.previous();

		iview::IInteractiveShape* uiShapePtr = dynamic_cast<iview::IInteractiveShape*>(shape.shapePtr);
		const i2d::CRect& boundingBox = shape.box;
		if (			(uiShapePtr != m_focusedShapePtr) &&
						uiShapePtr->IsVisible() &&
						boundingBox.IsInside(position)){
			ITouchable::TouchState touchState = uiShapePtr->IsTouched(position);
			if (touchState != IInteractiveShape::TS_NONE){
				if ((listenerPtr != NULL) && listenerPtr->OnViewMouseButton(*viewPtr, position, buttonType, downFlag, uiShapePtr)){
					return true;
				}

				if (buttonType == Qt::LeftButton){
					if (touchState == IInteractiveShape::TS_DRAGGABLE){
						if (downFlag){
							IDraggable* draggablePtr = dynamic_cast<IDraggable*>(viewPtr);
							if ((draggablePtr != NULL) && draggablePtr->IsDraggable()){
								draggablePtr->BeginDrag(position);
							}
						}
					}
					else{
						uiShapePtr->OnMouseButton(position, buttonType, downFlag);
					}
				}
				else{	// right button
					uiShapePtr->OnMouseButton(position, buttonType, downFlag);
				}

				return true;
			}
		}
	}

	int keysState = GetKeysState();

	QVectorIterator<ShapeWithBoundingBox> inactiveShapeIterator(m_shapes);
	inactiveShapeIterator.toBack();

	while (inactiveShapeIterator.hasPrevious()){
		const ShapeWithBoundingBox& inactiveShape = inactiveShapeIterator.previous();

		iview::IInteractiveShape* uiShapePtr = dynamic_cast<iview::IInteractiveShape*>(inactiveShape.shapePtr);
		const i2d::CRect& boundingBox = inactiveShape.box;
		if (			(uiShapePtr != m_focusedShapePtr) &&
						uiShapePtr->IsVisible() &&
						boundingBox.IsInside(position)){
			ITouchable::TouchState touchState = uiShapePtr->IsTouched(position);
			if (touchState != IInteractiveShape::TS_NONE){
				if ((listenerPtr != NULL) && listenerPtr->OnViewMouseButton(*viewPtr, position, buttonType, downFlag, uiShapePtr)){
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
					viewPtr->UpdateMousePointer();

					if (touchState == IInteractiveShape::TS_DRAGGABLE){
						IDraggable*  draggablePtr = dynamic_cast<IDraggable*>(viewPtr);
						if ((draggablePtr != NULL) && draggablePtr->IsDraggable()){
							draggablePtr->BeginDrag(position);
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
	QVectorIterator<ShapeWithBoundingBox> activeShapeIterator(m_activeShapes);
	activeShapeIterator.toBack();

	while (activeShapeIterator.hasPrevious()){
		const ShapeWithBoundingBox& activeShape = activeShapeIterator.previous();

		IInteractiveShape* uiShapePtr = dynamic_cast<iview::IInteractiveShape*>(activeShape.shapePtr);
		Q_ASSERT(uiShapePtr != NULL);
		const i2d::CRect& boundingBox = activeShape.box;

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

	QVectorIterator<ShapeWithBoundingBox> inactiveShapeIterator(m_shapes);
	inactiveShapeIterator.toBack();

	while (inactiveShapeIterator.hasPrevious()){
		const ShapeWithBoundingBox& inactiveShape = inactiveShapeIterator.previous();

		IInteractiveShape* uiShapePtr = dynamic_cast<iview::IInteractiveShape*>(inactiveShape.shapePtr);
		Q_ASSERT(uiShapePtr != NULL);
		const i2d::CRect& boundingBox = inactiveShape.box;

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
	for (ShapeList::ConstIterator iter = m_activeShapes.begin(); iter != m_activeShapes.end(); ++iter){
		IInteractiveShape* uiShape = dynamic_cast<IInteractiveShape*>(iter->shapePtr);
		Q_ASSERT(uiShape != NULL);	// only iview::IInteractiveShape object are accepted.

		result.insert(uiShape);
	}
}


void CInteractiveViewLayer::DeselectAllShapes()
{
	while (!m_activeShapes.isEmpty()){
		ShapeList::iterator iter = m_activeShapes.begin();
		Q_ASSERT(dynamic_cast<iview::IInteractiveShape*>(iter->shapePtr) != NULL);

		iview::IInteractiveShape* shapePtr = dynamic_cast<iview::IInteractiveShape*>(iter->shapePtr);
		Q_ASSERT(shapePtr != NULL);
		Q_ASSERT(shapePtr->IsSelected());

		shapePtr->SetSelected(false);
	}
}


void CInteractiveViewLayer::OnShapeFocused(IInteractiveShape* shapePtr)
{
	Q_ASSERT(shapePtr != NULL);

	ShapeList::iterator iter = m_activeShapes.begin();
	for (; iter != m_activeShapes.end(); ++iter) {
		if (iter->shapePtr == shapePtr) {
			break;
		}
	}
	Q_ASSERT(iter != m_activeShapes.end());

	if (m_focusedShapePtr != shapePtr){
		m_focusedShapePtr = shapePtr;

		IShapeView* viewPtr = GetViewPtr();
		Q_ASSERT(viewPtr != NULL);
		viewPtr->OnLayerShapeFocused(shapePtr, this);
	}
}


void CInteractiveViewLayer::OnShapeDefocused(IInteractiveShape* shapePtr)
{
	Q_ASSERT(shapePtr != NULL);

	if (m_focusedShapePtr == shapePtr){
		m_focusedShapePtr = NULL;

		IShapeView* viewPtr = GetViewPtr();
		Q_ASSERT(viewPtr != NULL);
		viewPtr->OnLayerShapeDefocused(shapePtr, this);
	}
}


void CInteractiveViewLayer::OnShapeSelected(IInteractiveShape& shape, bool state)
{
	if (state){
		ShapeList::iterator iter = m_shapes.begin();
		for (; iter != m_shapes.end(); ++iter) {
			if (iter->shapePtr == &shape) {
				break;
			}
		}

		if (iter != m_shapes.end()){
			i2d::CRect prevBoundingBox = iter->box;

			m_shapes.erase(iter);

			ShapeList::iterator it = m_activeShapes.begin();
			for (; it != m_activeShapes.end(); ++it) {
				if (it->shapePtr == &shape) {
					break;
				}
			}

			if (it != m_activeShapes.end()) {
				it->shapePtr = &shape;
				it->box = prevBoundingBox;
			}
			else {
				m_activeShapes.push_back(ShapeWithBoundingBox(&shape, prevBoundingBox));
			}
		}
		else{
			I_CRITICAL();
		}
	}
	else{
		ShapeList::iterator iter = m_activeShapes.begin();
		for (; iter != m_activeShapes.end(); ++iter) {
			if (iter->shapePtr == &shape) {
				break;
			}
		}

		if (iter != m_activeShapes.end()){
			if (m_focusedShapePtr == &shape){
				OnShapeDefocused(&shape);
			}
			i2d::CRect prevBoundingBox = iter->box;

			m_activeShapes.erase(iter);

			ShapeList::iterator it = m_shapes.begin();
			for (; it != m_shapes.end(); ++it) {
				if (it->shapePtr == &shape) {
					break;
				}
			}

			if (it != m_shapes.end()) {
				it->shapePtr = &shape;
				it->box = prevBoundingBox;
			}
			else {
				m_shapes.push_back(ShapeWithBoundingBox(&shape, prevBoundingBox));
			}
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
	ShapeList::iterator iter = m_activeShapes.begin();
	for (; iter != m_activeShapes.end(); ++iter) {
		if (iter->shapePtr == shapePtr) {
			break;
		}
	}

	return (iter != m_activeShapes.end()) || BaseClass::IsShapeConnected(shapePtr);
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

	for (ShapeList::iterator iter = m_activeShapes.begin(); iter != m_activeShapes.end(); ++iter){
		IShape* shapePtr = iter->shapePtr;
		Q_ASSERT(shapePtr != NULL);

		OnAreaInvalidated(iter->box, i2d::CRect::GetEmpty());
		shapePtr->OnDisconnectDisplay(this);
	}

	m_activeShapes.clear();
}


void CInteractiveViewLayer::DrawShapes(QPainter& drawContext)
{
	BaseClass::DrawShapes(drawContext);

	for (ShapeList::iterator pos = m_activeShapes.begin(); pos != m_activeShapes.end(); ++pos){
		iview::IShape* shapePtr = pos->shapePtr;
		if (shapePtr->IsVisible() && (shapePtr != m_focusedShapePtr)){
			const i2d::CRect& boundingBox = pos->box;
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
	Q_ASSERT(shapePtr != NULL);

	ShapeList::iterator iter = m_activeShapes.begin();
	for (; iter != m_activeShapes.end(); ++iter) {
		if (iter->shapePtr == shapePtr) {
			break;
		}
	}

	if (iter != m_activeShapes.end()){
		OnChangeShapeElement(iter);

		return;
	}

	BaseClass::OnChangeShape(shapePtr);
}


bool CInteractiveViewLayer::DisconnectShape(IShape* shapePtr)
{
	Q_ASSERT(shapePtr != NULL);

	if (BaseClass::DisconnectShape(shapePtr)){
		return true;
	}

	ShapeList::iterator iter = m_activeShapes.begin();
	for (; iter != m_activeShapes.end(); ++iter) {
		if (iter->shapePtr == shapePtr) {
			break;
		}
	}

	if (iter != m_activeShapes.end()){
		if (m_focusedShapePtr == shapePtr){
			IInteractiveShape* uiShapePtr = dynamic_cast<IInteractiveShape*>(shapePtr);
			Q_ASSERT(uiShapePtr != NULL);
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
		for (ShapeList::const_iterator iter = m_activeShapes.begin(); iter != m_activeShapes.end(); ++iter){
			const iview::IShape* shapePtr = iter->shapePtr;
			const i2d::CRect& boundingBox = iter->box;
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
		for (ShapeList::const_iterator iter = m_activeShapes.begin(); iter != m_activeShapes.end(); ++iter){
			const iview::IShape* shapePtr = iter->shapePtr;
			const i2d::CRect& boundingBox = iter->box;
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


// reimplemented (iview::IDraggable)

void CInteractiveViewLayer::BeginDrag(const istd::CIndex2d& position)
{
	for (ShapeList::iterator iter = m_activeShapes.begin(); iter != m_activeShapes.end(); ++iter){
		IDraggable* draggablePtr = dynamic_cast<IDraggable*>(iter->shapePtr);
		if ((draggablePtr != NULL) && draggablePtr->IsDraggable()){
			draggablePtr->BeginDrag(position);
		}
	}
}


void CInteractiveViewLayer::SetDragPosition(const istd::CIndex2d& position)
{
	for (ShapeList::iterator iter = m_activeShapes.begin(); iter != m_activeShapes.end(); ++iter){
		IDraggable* draggablePtr = dynamic_cast<IDraggable*>(iter->shapePtr);
		if ((draggablePtr != NULL) && draggablePtr->IsDraggable()){
			draggablePtr->SetDragPosition(position);
		}
	}
}


void CInteractiveViewLayer::EndDrag()
{
	for (ShapeList::iterator iter = m_activeShapes.begin(); iter != m_activeShapes.end(); ++iter) {
		IDraggable* draggablePtr = dynamic_cast<IDraggable*>(iter->shapePtr);
		if ((draggablePtr != NULL) && draggablePtr->IsDraggable()){
			draggablePtr->EndDrag();
		}
	}
}


bool CInteractiveViewLayer::IsDraggable() const
{
	return !m_activeShapes.isEmpty();
}


} // namespace iview


