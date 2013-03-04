#include "iview/CViewBase.h"


// ACF includes
#include "imod/IModel.h"

#include "iimg/IBitmap.h"

#include "iview/IInteractiveShape.h"
#include "iview/ISelectable.h"
#include "iview/IViewEventObserver.h"
#include "iview/CViewLayer.h"
#include "iview/CInteractiveViewLayer.h"


namespace iview
{


// public constructors

CViewBase::CViewBase()
{
	m_transform.Reset();

	m_invalidatedBox.Reset();
	m_isBackgroundBufferValid = false;
	m_isBoundingBoxValid = false;
	m_mouseShapePtr = NULL;
	m_isMouseShapeValid = false;
	m_lastBackgroundLayerIndex = -1;

	m_viewMode = VM_NONE;
	m_editMode = EM_NONE;
	
	m_isMultiselectable = true;
	m_isViewDraggable = false;
	m_isDraggable = true;

	m_isLastMouseButtonDown = false;
	m_keysState = 0;

	m_fitArea.Reset();

	m_focusedLayerPtr = NULL;
	m_backgroundLayerIndex = -1;
	m_inactiveLayerIndex = -1;
	m_activeLayerIndex = -1;
}


CViewBase::~CViewBase()
{
	for (Layers::iterator iter = m_layers.begin(); iter != m_layers.end(); ++iter){
		IViewLayer* layerPtr = *iter;
		Q_ASSERT(layerPtr != NULL);

		layerPtr->OnDisconnectView(this);
	}
	m_layers.clear();
}


// public methods

void CViewBase::SetZoom(ZoomMode zoom)
{
	i2d::CRect clientRect = GetClientRect();
	istd::CIndex2d screenCenter((clientRect.GetLeft() + clientRect.GetRight()) / 2, (clientRect.GetTop() + clientRect.GetBottom()) / 2);
	i2d::CVector2d center = m_transform.GetClientPosition(screenCenter);

	switch (zoom){
	case ZM_FIT:
	case ZM_FIT_COVER:
	case ZM_FIT_H:
	case ZM_FIT_V:
		{
			ibase::CSize clientSize = clientRect.GetSize();
			if (!m_fitArea.IsEmpty() && !clientSize.IsNull()){
				i2d::CVector2d fitSize = m_fitArea.GetSize();
				double scaleX = clientSize.GetX() / fitSize.GetX();
				double scaleY = clientSize.GetY() / fitSize.GetY();
				i2d::CVector2d deltaPos;
				double scale;
				if (zoom == ZM_FIT_H){
					scale = scaleX;
					deltaPos.SetX(-m_fitArea.GetLeft() * scale);
					istd::CIndex2d clientCenter = clientRect.GetCenter();
					i2d::CVector2d prevCenter = m_transform.GetClientPosition(clientCenter);
					deltaPos.SetY(clientCenter.GetY() - prevCenter.GetY() * scale);
				}
				else if (zoom == ZM_FIT_V){
					scale = scaleY;
					istd::CIndex2d clientCenter = clientRect.GetCenter();
					i2d::CVector2d prevCenter = m_transform.GetClientPosition(clientCenter);
					deltaPos.SetX(clientCenter.GetX() - prevCenter.GetX() * scale);
					deltaPos.SetY(-m_fitArea.GetTop() * scale);
				}
				else if (zoom == ZM_FIT_COVER){
					if (scaleX < scaleY){
						scale = scaleY;
						deltaPos.SetX((clientSize.GetX() - fitSize.GetX() * scale) * 0.5 - m_fitArea.GetLeft() * scale);
						deltaPos.SetY(-m_fitArea.GetTop() * scale);
					}
					else{
						scale = scaleX;
						deltaPos.SetX(-m_fitArea.GetLeft() * scale);
						deltaPos.SetY((clientSize.GetY() - fitSize.GetY() * scale) * 0.5 - m_fitArea.GetTop() * scale);
					}
				}
				else{
					if (scaleX < scaleY){
						scale = scaleX;
						deltaPos.SetX(-m_fitArea.GetLeft() * scale);
						deltaPos.SetY((clientSize.GetY() - fitSize.GetY() * scale) * 0.5 - m_fitArea.GetTop() * scale);
					}
					else{
						scale = scaleY;
						deltaPos.SetX((clientSize.GetX() - fitSize.GetX() * scale) * 0.5 - m_fitArea.GetLeft() * scale);
						deltaPos.SetY(-m_fitArea.GetTop() * scale);
					}
				}
				m_transform.Reset(deltaPos, 0, i2d::CVector2d(scale, scale));
			}
		}
		break;

	case ZM_FIT_UNPROP:
		{
			ibase::CSize clientSize = clientRect.GetSize();
			if (!m_fitArea.IsEmpty() && !clientSize.IsNull()){
				i2d::CVector2d fitSize = m_fitArea.GetSize();
				double scaleX = clientSize.GetX() / fitSize.GetX();
				double scaleY = clientSize.GetY() / fitSize.GetY();
				i2d::CVector2d deltaPos(-m_fitArea.GetLeft() * scaleX, -m_fitArea.GetTop() * scaleY);
				m_transform.Reset(deltaPos, 0, i2d::CVector2d(scaleX, scaleY));
			}
		}
		break;

	case ZM_RESET:
		m_transform.Reset();
		break;

	default:
		{
			double actualScale = m_transform.GetDeformMatrix().GetFrobeniusNorm() / ::sqrt(2.0);
			double scale = 1.0;
			if (zoom == ZM_ZOOM_IN){
				if (actualScale < 100){
					scale = 2;
				}
			}
			else if (zoom == ZM_ZOOM_OUT){
				if (actualScale > 0.01){
					scale = 0.5;
				}
			}
			i2d::CAffine2d zoomTransform;
			zoomTransform.Reset(i2d::CVector2d(0, 0), 0, scale);
			m_transform.Apply(zoomTransform);
			SetScreenPosition(center, screenCenter);
		}
		return;
	}

	UpdateAllShapes(CF_TRANSFORM);
}


void CViewBase::SetEditMode(int mode)
{
	m_editMode = mode;

	UpdateAllShapes(CF_EDIT_MODE);
}


void CViewBase::UpdateAllShapes(int changeFlag)
{
	i2d::CRect lastBoundingBox;
	if (m_isBoundingBoxValid){
		lastBoundingBox = m_boundingBox;
	}
	else{
		lastBoundingBox.Reset();
	}

	for (Layers::iterator iter = m_layers.begin(); iter != m_layers.end(); ++iter){
		IViewLayer* layerPtr = *iter;
		Q_ASSERT(layerPtr != NULL);
		layerPtr->UpdateAllShapes(changeFlag);
	}

	InvalidateBoundingBox();

	i2d::CRect boundingBox = GetBoundingBox();
	OnAreaInvalidated(lastBoundingBox, boundingBox);

	Update();
}


void CViewBase::SetScreenPosition(const i2d::CVector2d& client, istd::CIndex2d screen)
{
	const i2d::CAffine2d& invert = m_transform.GetInverted();
	const i2d::CVector2d& cp = m_transform.GetClientPosition(screen);
	iview::CScreenTransform transform = i2d::CAffine2d(invert.GetDeformMatrix(), invert.GetTranslation() + client - cp);
	SetTransform(transform.GetInverted());
}


void CViewBase::AddViewEventObserver(iview::IViewEventObserver* listenerPtr)
{
	QVector<iview::IViewEventObserver*>::iterator foundIter = qFind(m_viewListeners.begin(), m_viewListeners.end(), listenerPtr);
	if (foundIter == m_viewListeners.end()){
		m_viewListeners.push_back(listenerPtr);
	}
}


void CViewBase::RemoveViewEventObserver(iview::IViewEventObserver* listenerPtr)
{
	QVector<iview::IViewEventObserver*>::iterator foundIter = qFind(m_viewListeners.begin(), m_viewListeners.end(), listenerPtr);
	if (foundIter != m_viewListeners.end()){
		m_viewListeners.erase(foundIter);
	}
}


void CViewBase::InsertDefaultLayers()
{
	InsertLayer(&m_backgroundLayer, -1, IViewLayer::LT_BACKGROUND);
	InsertLayer(&m_inactiveLayer, -1, IViewLayer::LT_INACTIVE);
	InsertLayer(&m_activeLayer, -1, IViewLayer::LT_ACTIVE);
}


// reimplemented (iview::IShapeView)

void CViewBase::Update()
{
	const i2d::CRect& invalidatedBox = GetInvalidatedBox();
	i2d::CRect clientRect = GetClientRect();
	i2d::CRect realRect = clientRect.GetIntersection(invalidatedBox);
	if (!realRect.IsEmpty()){
		UpdateRectArea(realRect);
	}
}


int CViewBase::InsertLayer(IViewLayer* layerPtr, int index, int layerType)
{
	if (index >= 0){
		Layers::iterator iter = m_layers.begin();
		iter += index;
		m_layers.insert(iter, layerPtr);
	}
	else{
		index = int(m_layers.size());
		m_layers.push_back(layerPtr);
	}

	if (layerType == IViewLayer::LT_INACTIVE){
		m_inactiveLayerIndex = index;
	}
	else if (m_inactiveLayerIndex >= index){
		m_inactiveLayerIndex++;
	}

	if (layerType == IViewLayer::LT_BACKGROUND){
		m_backgroundLayerIndex = index;
	}
	else if (m_backgroundLayerIndex >= index){
		m_backgroundLayerIndex++;
	}

	if (layerType == IViewLayer::LT_ACTIVE){
		m_activeLayerIndex = index;
	}
	else if (m_activeLayerIndex >= index){
		m_activeLayerIndex++;
	}

	layerPtr->OnConnectView(this);

	i2d::CRect layerBoundingBox = layerPtr->GetBoundingBox();
	if (m_isBoundingBoxValid){
		m_boundingBox.Union(layerBoundingBox);
	}

	OnAreaInvalidated(i2d::CRect::GetEmpty(), layerPtr->GetBoundingBox());

	return index;
}


int CViewBase::GetLayerIndex(const IViewLayer& layer) const
{
	int index = 0;
	for (Layers::const_iterator iter = m_layers.begin(); iter != m_layers.end(); ++iter){
		if (*iter == &layer){
			return index;
		}

		++index;
	}

	return -1;
}


void CViewBase::RemoveLayer(int index)
{
	Layers::iterator iter = m_layers.begin() + index;
	m_layers.erase(iter);

	if (m_activeLayerIndex >= index){
		if (m_activeLayerIndex == index){
			m_activeLayerIndex = -1;
		}
		else{
			m_activeLayerIndex--;
		}
	}

	if (m_inactiveLayerIndex >= index){
		if (m_inactiveLayerIndex == index){
			m_inactiveLayerIndex = -1;
		}
		else{
			m_inactiveLayerIndex--;
		}
	}

	if (m_backgroundLayerIndex >= index){
		if (m_backgroundLayerIndex == index){
			m_backgroundLayerIndex = -1;
		}
		else{
			m_backgroundLayerIndex--;
		}
	}
}


bool CViewBase::ConnectShape(IShape* shapePtr)
{
	Q_ASSERT(shapePtr != NULL);

	if (GetLayersCount() <= 0){
		InsertDefaultLayers();
	}

	int layerType = shapePtr->GetLayerType();
	int layerIndex = -1;

	iview::IInteractiveShape* activeShapePtr = dynamic_cast<iview::IInteractiveShape*>(shapePtr);

	switch (layerType){
		case IViewLayer::LT_ACTIVE:
			if (activeShapePtr != NULL){
				layerIndex = m_activeLayerIndex;
			}
			else{
				layerIndex = m_inactiveLayerIndex;
			}

			break;
		
		case IViewLayer::LT_INACTIVE:
			layerIndex = m_inactiveLayerIndex;
			break;
		
		case IViewLayer::LT_BACKGROUND:
			layerIndex = m_backgroundLayerIndex;
			break;
		default:
			return false;
	}

	if (layerIndex < 0){
		return false;
	}

	Q_ASSERT(layerIndex < GetLayersCount());

	IViewLayer& layer = GetLayer(layerIndex);

	layer.ConnectShape(shapePtr);

	return true;
}


ISelectableLayer* CViewBase::GetFocusedLayerPtr() const
{
	return m_focusedLayerPtr;
}


void CViewBase::OnShapeFocused(IInteractiveShape* shapePtr, ISelectableLayer* layerPtr)
{
	Q_ASSERT(layerPtr != NULL);

	if (m_focusedLayerPtr != layerPtr){
		m_focusedLayerPtr = layerPtr;
	}

	CViewBase::OnShapeFocused(shapePtr);
}


void CViewBase::OnShapeDefocused(IInteractiveShape* shapePtr, ISelectableLayer* layerPtr)
{
	Q_ASSERT(layerPtr != NULL);

	if (m_focusedLayerPtr == layerPtr){
		m_focusedLayerPtr = NULL;
	}

	CViewBase::OnShapeDefocused(shapePtr);
}


// reimplemented (iview::ISelectable)

int CViewBase::GetSelectedShapesCount() const
{
	int selectedCount = 0;
	for (Layers::const_iterator iter = m_layers.begin(); iter != m_layers.end(); ++iter){
		const ISelectableLayer* layerPtr = dynamic_cast<const ISelectableLayer*>(*iter);
		if (layerPtr != NULL){
			selectedCount += layerPtr->GetSelectedShapesCount();
		}
	}

	return selectedCount;
}


void CViewBase::InsertSelectedShapes(SelectedShapes& result) const
{
	for (Layers::const_iterator iter = m_layers.begin(); iter != m_layers.end(); ++iter){
		ISelectable* selectablePtr = dynamic_cast<ISelectable*>(*iter);
		if (selectablePtr != NULL){
			selectablePtr->InsertSelectedShapes(result);
		}
	}
}


void CViewBase::DeselectAllShapes()
{
	for (Layers::iterator iter = m_layers.begin(); iter != m_layers.end(); ++iter){
		ISelectableLayer* layerPtr = dynamic_cast<ISelectableLayer*>(*iter);
		if (layerPtr != NULL){
			layerPtr->DeselectAllShapes();
		}
	}
}


int CViewBase::GetEditMode() const
{
	return m_editMode;
}


// reimplemented (iview::ITouchable)

ITouchable::TouchState CViewBase::IsTouched(istd::CIndex2d position) const
{
	int layersCont = m_layers.size();
	for (int i = layersCont - 1; i >= 0; --i){
		const IViewLayer* layerPtr = m_layers[i];
		if (layerPtr != NULL){
			TouchState touchState = layerPtr->IsTouched(position);
			if (touchState != TS_NONE){
				return touchState;
			}
		}
	}

	return TS_NONE;
}


QString CViewBase::GetShapeDescriptionAt(istd::CIndex2d position) const
{
	int layersCont = m_layers.size();
	for (int i = layersCont - 1; i >= 0; --i){
		const IViewLayer* layerPtr = m_layers[i];
		if (layerPtr != NULL){
			TouchState touchState = layerPtr->IsTouched(position);
			if (touchState != TS_NONE){
				return layerPtr->GetShapeDescriptionAt(position);
			}
		}
	}

	return "";
}


// reimplemented (iview::IShapeView)

void CViewBase::SetTransform(const i2d::CAffine2d& transform)
{
	if (m_transform != transform){
		m_transform = transform;

		UpdateAllShapes(CF_TRANSFORM);
	}
}


void CViewBase::UpdateMousePointer()
{
	SetMousePointer(CalcMousePointer(m_lastMousePosition));
}


void CViewBase::OnLayerInvalidated(const IViewLayer& layer, const i2d::CRect& prevArea, const i2d::CRect& newArea)
{
	Q_ASSERT(!m_layers.isEmpty());

	m_isBoundingBoxValid = false;

	for (int layerIndex = 0; layerIndex <= m_lastBackgroundLayerIndex; ++layerIndex){
		if (m_layers[layerIndex] == &layer){
			InvalidateBackground();
		}
	}

	OnAreaInvalidated(prevArea, newArea);
}


// reimplemented (iview::IDisplay)

void CViewBase::OnAreaInvalidated(const i2d::CRect& beforeBox, const i2d::CRect& afterBox)
{
	i2d::CRect boundingBox = GetBoundingBox();

	m_invalidatedBox.Union(afterBox);

	if (!boundingBox.IsEmpty()){
		if (			(beforeBox.GetLeft() <= boundingBox.GetLeft()) ||
						(beforeBox.GetTop() <= boundingBox.GetTop()) ||
						(beforeBox.GetRight() >= boundingBox.GetTop()) ||
						(beforeBox.GetBottom() >= boundingBox.GetTop()) ||
						(afterBox.GetLeft() <= boundingBox.GetLeft()) ||
						(afterBox.GetTop() <= boundingBox.GetTop()) ||
						(afterBox.GetRight() >= boundingBox.GetTop()) ||
						(afterBox.GetBottom() >= boundingBox.GetTop())){
			InvalidateBoundingBox();
		}
	}

	m_invalidatedBox.Union(beforeBox);

	InvalidateMouseShape();
}


// reimplemented (iview::IShapeObserver)

void CViewBase::OnChangeShape(IShape* shapePtr)
{
	for (Layers::iterator iter = m_layers.begin(); iter != m_layers.end(); ++iter){
		IViewLayer* layerPtr = *iter;
		Q_ASSERT(layerPtr != NULL);

		if (layerPtr->IsShapeConnected(shapePtr)){
			layerPtr->OnChangeShape(shapePtr);
		}
	}
}


bool CViewBase::DisconnectShape(IShape* shapePtr)
{
	for (Layers::iterator iter = m_layers.begin(); iter != m_layers.end(); ++iter){
		IViewLayer* layerPtr = *iter;
		Q_ASSERT(layerPtr != NULL);

		if (layerPtr->DisconnectShape(shapePtr)){
			if (iter == m_layers.begin()){
				InvalidateBackground();
			}

			return true;
		}
	}

	return false;
}


void CViewBase::OnShapeSelected(IInteractiveShape& shape, bool state)
{
	if (m_isSelectEventActive){
		for (int listenerIndex = 0; listenerIndex < int(m_viewListeners.size()); listenerIndex++){
			bool accepted = m_viewListeners.at(listenerIndex)->OnSelectChange(*this, m_lastMousePosition, shape, state);
			if (accepted){
				break;		
			}
		}
	}
}


// reimplemented (iview::IDraggable)

void CViewBase::BeginDrag(const istd::CIndex2d& position)
{
	for (Layers::iterator iter = m_layers.begin(); iter != m_layers.end(); ++iter){
		ISelectableLayer* layerPtr = dynamic_cast<ISelectableLayer*>(*iter);
		if (layerPtr != NULL){
			layerPtr->BeginDrag(position);
		}
	}

	m_viewMode = VM_DRAG;
}


void CViewBase::EndDrag()
{
	for (Layers::iterator iter = m_layers.begin(); iter != m_layers.end(); ++iter){
		ISelectableLayer* layerPtr = dynamic_cast<ISelectableLayer*>(*iter);
		if (layerPtr != NULL){
			layerPtr->EndDrag();
		}
	}

	m_viewMode = VM_NONE;
}


void CViewBase::SetDragPosition(const istd::CIndex2d& position)
{
	Q_ASSERT(m_viewMode == VM_DRAG);

	for (Layers::iterator iter = m_layers.begin(); iter != m_layers.end(); ++iter){
		ISelectableLayer* layerPtr = dynamic_cast<ISelectableLayer*>(*iter);
		if (layerPtr != NULL){
			layerPtr->SetDragPosition(position);
		}
	}
}


// protected methods	
	
void CViewBase::DrawLayers(QPainter& drawContext, int firstLayer, int lastLayer)
{
	Q_ASSERT((firstLayer >= 0) && (firstLayer < GetLayersCount()));
	Q_ASSERT((lastLayer >= 0) && (lastLayer < GetLayersCount()));

	bool drawFocused = false;

	for (int index = firstLayer; index <= lastLayer; ++index){
		IViewLayer& layer = GetLayer(index);
		layer.DrawShapes(drawContext);

		if (&layer == m_focusedLayerPtr){
			drawFocused = true;
		}
	}
	if (drawFocused){
		Q_ASSERT(m_focusedLayerPtr != NULL);

		m_focusedLayerPtr->DrawFocusedShape(drawContext);
	}
}


void CViewBase::DisconnectAllShapes()
{
	for (Layers::iterator iter = m_layers.begin(); iter != m_layers.end(); ++iter){
		IViewLayer* layerPtr = *iter;
		Q_ASSERT(layerPtr != NULL);

		layerPtr->DisconnectAllShapes();
	}
}


void CViewBase::InvalidateBackground()
{
	SetBackgroundBufferValid(false);
	if (!m_layers.isEmpty()){
		IViewLayer* backgroundLayerPtr = m_layers[0];
		Q_ASSERT(backgroundLayerPtr != NULL);

		const i2d::CRect& clientRect = backgroundLayerPtr->GetBoundingBox();
		OnAreaInvalidated(clientRect, i2d::CRect::GetEmpty());
	}
}


void CViewBase::InvalidateBoundingBox()
{
	m_isBoundingBoxValid = false;
}


i2d::CRect CViewBase::CalcBoundingBox() const
{
	i2d::CRect boundingBox = i2d::CRect::GetEmpty();

	for (Layers::const_iterator iter = m_layers.begin(); iter != m_layers.end(); ++iter){
		IViewLayer* layerPtr = *iter;
		Q_ASSERT(layerPtr != NULL);

		boundingBox.Union(layerPtr->GetBoundingBox());
	}

	return boundingBox;
}


void CViewBase::InvalidateMouseShape()
{
	CViewBase::m_isMouseShapeValid = false;
}


void CViewBase::CalcMouseShape() const
{
	m_mouseShapePtr = NULL;

	int layersCont = m_layers.size();
	for (int i = layersCont - 1; i >= 0; --i){
		ISelectableLayer* layerPtr = dynamic_cast<ISelectableLayer*>(m_layers[i]);
		if (layerPtr != NULL){
			IInteractiveShape* shapePtr;
			ITouchable::TouchState touchState = layerPtr->IsTouched(m_lastMousePosition, &shapePtr);
			if (touchState != TS_NONE){
				m_mouseShapePtr = shapePtr;
				return;
			}
		}
	}
}


bool CViewBase::OnMouseButton(istd::CIndex2d position, Qt::MouseButton buttonType, bool downFlag)
{
	if (m_isLastMouseButtonDown != downFlag){
		m_lastMousePosition = position;
		m_isLastMouseButtonDown = downFlag;

		m_isSelectEventActive = true;

		if (!downFlag){
			if (m_viewMode == VM_DRAG){
				EndDrag();

				m_isSelectEventActive = false;
				return true;
			}

			if (m_viewMode == VM_MOVE){
				m_viewMode = VM_NONE;

				m_isSelectEventActive = false;
				return true;
			}
		}

		if ((m_focusedLayerPtr != NULL) && (buttonType == Qt::LeftButton)){
			if (m_focusedLayerPtr->OnFocusedMouseButton(position, buttonType, downFlag)){
				m_isSelectEventActive = false;
				return true;
			}
		}

		int layersCont = m_layers.size();
		for (int i = layersCont - 1; i >= 0; --i){
			ISelectableLayer* layerPtr = dynamic_cast<ISelectableLayer*>(m_layers[i]);
			if (layerPtr != NULL){
				if (layerPtr->OnMouseButton(position, buttonType, downFlag)){
					m_isSelectEventActive = false;
					return true;
				}
			}
		}

		if (downFlag && (buttonType == Qt::LeftButton)){
			if ((m_keysState & Qt::ControlModifier) == 0){
				DeselectAllShapes();
				UpdateMousePointer();
				Update();
			}
			if (CanBeMoved()){
				m_viewMode = VM_MOVE;
			}
			else{
				m_viewMode = VM_NONE;
			}
			m_moveReference = m_transform.GetClientPosition(position);

			m_isSelectEventActive = false;
			return true;
		}

		m_isSelectEventActive = false;
	}

	return false;
}


bool CViewBase::OnMouseMove(istd::CIndex2d position)
{
	m_lastMousePosition = position;
	InvalidateMouseShape();

	bool result = false;

	if ((m_keysState & Qt::LeftButton) == 0){
		m_viewMode = VM_NONE;
	}
	else if (m_viewMode == VM_DRAG){
		SetDragPosition(position);
		Update();
		result = true;
	}
	else if (m_viewMode == VM_MOVE){
		SetScreenPosition(m_moveReference, position);
		result = true;
	}
	else if (m_focusedLayerPtr != NULL){
		result = m_focusedLayerPtr->OnFocusedMouseMove(position);
		if (result){
			Update();
		}
	}

	UpdateMousePointer();

	return result;
}


ISelectable::MousePointerMode CViewBase::CalcMousePointer(istd::CIndex2d position) const
{
	bool areSelected = false;

	int layersCont = m_layers.size();
	for (int i = layersCont - 1; i >= 0; --i){
		ISelectableLayer* layerPtr = dynamic_cast<ISelectableLayer*>(m_layers[i]);
		if (layerPtr != NULL){

			IInteractiveShape* shapePtr = NULL;

			ITouchable::TouchState touchState = layerPtr->IsTouched(position, &shapePtr);
			if (touchState != TS_NONE){
				ISelectable::MousePointerMode result;
				switch (touchState){
				case iview::IInteractiveShape::TS_TICKER:
					result = MPM_CROSS;
					break;

				case iview::IInteractiveShape::TS_INACTIVE:
					result = MPM_DEFAULT;
					break;

				case iview::IInteractiveShape::TS_DRAGGABLE:
					result = MPM_DRAG;
					break;

				default:
					result = MPM_DEFAULT;
					break;
				}
				Q_ASSERT(shapePtr != NULL);

				return result;
			}

			areSelected = areSelected || (layerPtr->GetSelectedShapesCount() > 0);
		}
	}

	MousePointerMode result = MPM_DEFAULT;

	if (areSelected){
		result = MPM_DESELECT;
	}
	else{
		if (CanBeMoved()){
			result = MPM_HAND;
		}
	}

	return result;
}


// protected methods

bool CViewBase::CanBeMoved() const
{
	return IsViewDraggable();
}


} // namespace iview


