#include <iview/CViewLayer.h>


// Qt includes
#include <QtGui/QPainter>


// ACF includes
#include <iqt/iqt.h>

#include <iview/IShape.h>
#include <iview/IShapeView.h>


namespace iview
{


CViewLayer::CViewLayer()
{
	m_viewPtr = NULL;
	m_isVisible = true;
	m_boundingBox.Reset();
	m_isBoundingBoxValid = true;
}


CViewLayer::~CViewLayer()
{
	DisconnectAllShapes();

	if (m_viewPtr != NULL){
		int layerIndex = m_viewPtr->GetLayerIndex(*this);
		Q_ASSERT(layerIndex >= 0);

		m_viewPtr->RemoveLayer(layerIndex);
	}
}


// reimplemented (iview::IViewLayer)

void CViewLayer::OnConnectView(IShapeView* viewPtr)
{
	Q_ASSERT(m_viewPtr == NULL);
	Q_ASSERT(viewPtr != NULL);

	m_viewPtr = viewPtr;
}


void CViewLayer::OnDisconnectView(IShapeView* I_IF_DEBUG(viewPtr))
{
	I_IF_DEBUG(Q_ASSERT(m_viewPtr == viewPtr));
	I_IF_DEBUG(Q_ASSERT(viewPtr != NULL));

	m_viewPtr = NULL;
}


IShapeView* CViewLayer::GetViewPtr() const
{
	return m_viewPtr;
}


bool CViewLayer::IsShapeConnected(IShape* shapePtr)
{
	for (ShapeList::iterator it = m_shapes.begin(); it != m_shapes.end(); ++it){
		if (it->shapePtr == shapePtr){
			return true;
		}
	}

	return false;
}


bool CViewLayer::ConnectShape(IShape* shapePtr)
{
	shapePtr->OnConnectDisplay(this);

	static const istd::IChangeable::ChangeSet changeSet(istd::IChangeable::CF_ALL_DATA);
	shapePtr->OnDisplayChange(changeSet);

	i2d::CRect boundingBox = shapePtr->GetBoundingBox();
	Q_ASSERT(boundingBox.IsValid());

	m_shapes.push_back(ShapeWithBoundingBox(shapePtr, boundingBox));

	OnAreaInvalidated(i2d::CRect::GetEmpty(), boundingBox);

	return true;
}


int CViewLayer::GetShapesCount() const
{
	return int(m_shapes.size());
}


QList<iview::IShape*> CViewLayer::GetShapes() const
{
	QList<iview::IShape*> result;
	for (ShapeList::const_iterator it = m_shapes.begin(); it != m_shapes.end(); ++it){
		result.push_back(it->shapePtr);
	}
	return result;
}


void CViewLayer::UpdateAllShapes(const istd::IChangeable::ChangeSet& changeSet)
{
	m_boundingBox = RecalcAllShapes(changeSet);
	m_isBoundingBoxValid = true;
}


void CViewLayer::DisconnectAllShapes()
{
	while (!m_shapes.isEmpty()){
		DisconnectShapeElement(m_shapes, m_shapes.begin());
	}
}


void CViewLayer::DrawShapes(QPainter& drawContext)
{
	if (IsVisible()){
		for (ShapeList::iterator iter = m_shapes.begin(); iter != m_shapes.end(); ++iter){
			iview::IShape* shapePtr = iter->shapePtr;
			if (shapePtr->IsVisible()){
				const i2d::CRect& boundingBox = iter->box;
				i2d::CRect updateRect = iqt::GetCRect(drawContext.clipRegion().boundingRect());

				if (!updateRect.IsOutside(boundingBox)){
					shapePtr->Draw(drawContext);
				}
			}
		}
	}
}


void CViewLayer::SetVisible(bool state)
{
	if (m_isVisible != state){
		m_isBoundingBoxValid = false;
		m_isVisible = state;

		if (m_isVisible){
			OnAreaInvalidated(i2d::CRect::GetEmpty(), GetBoundingBox());
		}
		else{
			OnAreaInvalidated(GetBoundingBox(), i2d::CRect::GetEmpty());
		}
	}
}


// reimplemented (iview::IDisplay)

IDisplay* CViewLayer::GetParentDisplayPtr() const
{
	return m_viewPtr;
}


const CScreenTransform& CViewLayer::GetTransform() const
{
	Q_ASSERT(m_viewPtr != NULL);

	return m_viewPtr->GetTransform();
}


i2d::CRect CViewLayer::GetBoundingBox() const
{
	if (!m_isBoundingBoxValid){
		m_boundingBox = CalcBoundingBox();

		m_isBoundingBoxValid = true;
	}
	
	return m_boundingBox;
}


i2d::CRect CViewLayer::GetClientRect() const
{
	Q_ASSERT(m_viewPtr != NULL);

	return m_viewPtr->GetClientRect();
}


const IColorSchema& CViewLayer::GetColorSchema() const
{
	Q_ASSERT(m_viewPtr != NULL);

	return m_viewPtr->GetColorSchema();
}


void CViewLayer::OnAreaInvalidated(const i2d::CRect& prevArea, const i2d::CRect& newArea)
{
	Q_ASSERT(!m_isBoundingBoxValid || m_boundingBox.IsInside(prevArea));

	if (m_isBoundingBoxValid){
		if (newArea.IsInside(prevArea) || (
					(prevArea.GetLeft() > m_boundingBox.GetLeft()) &&
					(prevArea.GetRight() < m_boundingBox.GetRight()) &&
					(prevArea.GetTop() > m_boundingBox.GetTop()) &&
					(prevArea.GetBottom() < m_boundingBox.GetBottom()))){
			m_boundingBox.Union(newArea);
		}
		else{
			m_isBoundingBoxValid = false;
		}
	}

	Q_ASSERT(!m_isBoundingBoxValid || m_boundingBox.IsInside(newArea));

	if (m_viewPtr != NULL){
		m_viewPtr->OnLayerInvalidated(*this, prevArea, newArea);
	}
}


// reimplemented (iview::IShapeObserver)

void CViewLayer::OnChangeShape(IShape* shapePtr)
{
	Q_ASSERT(shapePtr != NULL);

	ShapeList::iterator iter = m_shapes.begin();
	for (; iter != m_shapes.end(); ++iter){
		if (iter->shapePtr == shapePtr){
			break;
		}
	}

	Q_ASSERT(iter != m_shapes.end());

	OnChangeShapeElement(iter);
}


bool CViewLayer::DisconnectShape(IShape* shapePtr)
{
	Q_ASSERT(shapePtr != NULL);

	ShapeList::iterator iter = m_shapes.begin();
	for (; iter != m_shapes.end(); ++iter){
		if (iter->shapePtr == shapePtr){
			break;
		}
	}

	if (iter != m_shapes.end()){
		DisconnectShapeElement(m_shapes, iter);

		return true;
	}

	return false;
}


// reimplemented (iview::ITouchable)

ITouchable::TouchState CViewLayer::IsTouched(istd::CIndex2d position) const
{
	if (IsVisible()){
		for (ShapeList::const_iterator iter = m_shapes.begin(); iter != m_shapes.end(); ++iter){
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

	return ITouchable::TS_NONE;
}


QString CViewLayer::GetShapeDescriptionAt(istd::CIndex2d position) const
{
	if (IsVisible()){
		for (ShapeList::const_iterator iter = m_shapes.begin(); iter != m_shapes.end(); ++iter){
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

	return "";
}


QString CViewLayer::GetToolTipAt(istd::CIndex2d position) const
{
	if (IsVisible()) {
		for (ShapeList::const_iterator iter = m_shapes.begin(); iter != m_shapes.end(); ++iter) {
			const iview::IShape* shapePtr = iter->shapePtr;
			const i2d::CRect& boundingBox = iter->box;
			if (boundingBox.IsInside(position) && shapePtr->IsVisible()) {
				if (shapePtr->IsInside(position)) {
					auto toolTip = shapePtr->GetToolTipAt(position);
					if (toolTip.size())
						return toolTip;
				}
			}
		}
	}

	return "";
}


// protected methods

bool CViewLayer::OnChangeShapeElement(ShapeList::Iterator elementIter)
{
	Q_ASSERT(elementIter->shapePtr != NULL);
	Q_ASSERT(GetBoundingBox().IsInside(elementIter->box));

	const i2d::CRect oldBoundingBox = elementIter->box;
	const iview::IShape* shapePtr = elementIter->shapePtr;
	Q_ASSERT(shapePtr != NULL);

	elementIter->box = shapePtr->GetBoundingBox();

	OnAreaInvalidated(oldBoundingBox, elementIter->box);

	return true;
}


void CViewLayer::DisconnectShapeElement(ShapeList& map, ShapeList::iterator iter)
{
	iview::IShape* shapePtr = iter->shapePtr;
	const i2d::CRect boundingBox = iter->box;
	map.erase(iter);

	OnAreaInvalidated(i2d::CRect::GetEmpty(), boundingBox);

	shapePtr->OnDisconnectDisplay(this);
}


void CViewLayer::InvalidateBoundingBox()
{
	m_isBoundingBoxValid = false;
}


i2d::CRect& CViewLayer::GetBoundingBoxRef() const
{
	return m_boundingBox;
}


void CViewLayer::SetBoundingBoxValid() const
{
	m_isBoundingBoxValid = true;
}


i2d::CRect CViewLayer::RecalcAllShapes(const istd::IChangeable::ChangeSet& changeSet)
{
	i2d::CRect boundingBox = i2d::CRect::GetEmpty();

	if (IsVisible()){
		for (ShapeList::iterator iter = m_shapes.begin(); iter != m_shapes.end(); ++iter){
			IShape* shapePtr = iter->shapePtr;
			i2d::CRect& shapeBox = iter->box;

			if (shapePtr->OnDisplayChange(changeSet)){
				shapeBox = shapePtr->GetBoundingBox();
			}
			boundingBox.Union(shapeBox);
		}
	}

	return boundingBox;
}


i2d::CRect CViewLayer::CalcBoundingBox() const
{
	i2d::CRect boundingBox = i2d::CRect::GetEmpty();

	for (ShapeList::ConstIterator iter = m_shapes.begin(); iter != m_shapes.end(); ++iter){
		const i2d::CRect& shapeBox = iter->box;
		
		boundingBox.Union(shapeBox);
	}

	return boundingBox;
}


} // namespace iview


