#include "iview/CViewLayer.h"


// Qt includes
#include <QtGui/QPainter>


// ACF includes
#include "iqt/iqt.h"

#include "iview/IShape.h"
#include "iview/IShapeView.h"


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
		I_ASSERT(layerIndex >= 0);

		m_viewPtr->RemoveLayer(layerIndex);
	}
}


CViewLayer::Shapes CViewLayer::GetAllShapes() const
{
	return m_shapes.keys();
}


// reimplemented (iview::IViewLayer)

void CViewLayer::OnConnectView(IShapeView* viewPtr)
{
	I_ASSERT(m_viewPtr == NULL);
	I_ASSERT(viewPtr != NULL);

	m_viewPtr = viewPtr;
}


void CViewLayer::OnDisconnectView(IShapeView* I_IF_DEBUG(viewPtr))
{
	I_ASSERT(m_viewPtr == viewPtr);
	I_ASSERT(viewPtr != NULL);

	m_viewPtr = NULL;
}


IShapeView* CViewLayer::GetViewPtr() const
{
	return m_viewPtr;
}


bool CViewLayer::IsShapeConnected(IShape* shapePtr)
{
	return (m_shapes.find(shapePtr) != m_shapes.end());
}


bool CViewLayer::ConnectShape(IShape* shapePtr)
{
	shapePtr->OnConnectDisplay(this);
	shapePtr->OnDisplayChange(CF_ALL);

	i2d::CRect boundingBox = shapePtr->GetBoundingBox();
	I_ASSERT(boundingBox.IsValid());

	m_shapes[shapePtr] = boundingBox;

	OnAreaInvalidated(i2d::CRect::GetEmpty(), boundingBox);

	return true;
}


int CViewLayer::GetShapesCount() const
{
	return int(m_shapes.size());
}


void CViewLayer::UpdateAllShapes(int changeFlag)
{
	m_boundingBox = RecalcAllShapes(changeFlag);
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
		for (ShapeMap::iterator iter = m_shapes.begin(); iter != m_shapes.end(); ++iter){
			iview::IShape* shapePtr = iter.key();
			if (shapePtr->IsVisible()){
				const i2d::CRect& boundingBox = iter.value();
				i2d::CRect updateRect = iqt::GetCRect(drawContext.clipRegion().boundingRect());

				if (!updateRect.IsOutside(boundingBox)){
					shapePtr->Draw(drawContext);
				}
			}
		}
	}
}


inline bool CViewLayer::IsVisible() const
{
	return m_isVisible;
}


inline void CViewLayer::SetVisible(bool state)
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
	I_ASSERT(m_viewPtr != NULL);

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
	I_ASSERT(m_viewPtr != NULL);

	return m_viewPtr->GetClientRect();
}


const IColorShema& CViewLayer::GetColorShema() const
{
	I_ASSERT(m_viewPtr != NULL);

	return m_viewPtr->GetColorShema();
}


void CViewLayer::OnAreaInvalidated(const i2d::CRect& prevArea, const i2d::CRect& newArea)
{
	I_ASSERT(!m_isBoundingBoxValid || m_boundingBox.IsInside(prevArea));

	if (m_isBoundingBoxValid){
		I_ASSERT(!m_isBoundingBoxValid || m_boundingBox.IsInside(prevArea));

		if (prevArea.IsEmpty() || (
						(m_boundingBox.GetLeft() < prevArea.GetLeft()) &&
						(m_boundingBox.GetRight() > prevArea.GetRight()) &&
						(m_boundingBox.GetTop() < prevArea.GetTop()) &&
						(m_boundingBox.GetBottom() > prevArea.GetBottom()))){
			m_boundingBox.Union(newArea);
		}
		else{
			m_isBoundingBoxValid = false;
		}
		I_ASSERT(!m_isBoundingBoxValid || m_boundingBox.IsInside(newArea));
	}

	if (m_viewPtr != NULL){
		m_viewPtr->OnLayerInvalidated(*this, prevArea, newArea);
	}
}


// reimplemented (iview::IShapeObserver)

void CViewLayer::OnChangeShape(IShape* shapePtr)
{
	I_ASSERT(shapePtr != NULL);

	ShapeMap::iterator iter = m_shapes.find(shapePtr);
	I_ASSERT(iter != m_shapes.end());

	OnChangeShapeElement(iter);
}


bool CViewLayer::DisconnectShape(IShape* shapePtr)
{
	I_ASSERT(shapePtr != NULL);

	ShapeMap::iterator iter = m_shapes.find(shapePtr);
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
		for (ShapeMap::iterator iter = m_shapes.begin(); iter != m_shapes.end(); ++iter){
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

	return ITouchable::TS_NONE;
}


QString CViewLayer::GetShapeDescriptionAt(istd::CIndex2d position) const
{
	if (IsVisible()){
		for (ShapeMap::iterator iter = m_shapes.begin(); iter != m_shapes.end(); ++iter){
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

	return "";
}


// protected methods

bool CViewLayer::OnChangeShapeElement(ShapeMap::Iterator elementIter)
{
	I_ASSERT(elementIter.key() != NULL);
	I_ASSERT(GetBoundingBox().IsInside(elementIter.value()));

	const i2d::CRect oldBoundingBox = elementIter.value();
	const iview::IShape* shapePtr = elementIter.key();
	I_ASSERT(shapePtr != NULL);

	elementIter.value() = shapePtr->GetBoundingBox();

	OnAreaInvalidated(oldBoundingBox, elementIter.value());

	return true;
}


void CViewLayer::DisconnectShapeElement(ShapeMap& map, ShapeMap::iterator iter)
{
	iview::IShape* shapePtr = iter.key();
	const i2d::CRect boundingBox = iter.value();
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


i2d::CRect CViewLayer::RecalcAllShapes(int changeFlag)
{
	i2d::CRect boundingBox = i2d::CRect::GetEmpty();

	if (IsVisible()){
		for (ShapeMap::iterator iter = m_shapes.begin(); iter != m_shapes.end(); ++iter){
			IShape* shapePtr = iter.key();
			i2d::CRect& shapeBox = iter.value();

			if (shapePtr->OnDisplayChange(changeFlag)){
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

	for (ShapeMap::ConstIterator iter = m_shapes.begin(); iter != m_shapes.end(); ++iter){
		const i2d::CRect& shapeBox = iter.value();
		
		boundingBox.Union(shapeBox);
	}

	return boundingBox;
}


} // namespace iview


