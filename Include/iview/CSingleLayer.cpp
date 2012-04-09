#include "iview/CSingleLayer.h"



// Qt includes
#include <QtGui/QPainter>


// ACF includes
#include "iqt/iqt.h"



#include "iview/IShape.h"
#include "iview/IShapeView.h"


namespace iview
{


CSingleLayer::CSingleLayer()
:	m_shapePtr(NULL)
{
}


CSingleLayer::~CSingleLayer()
{
	DisconnectAllShapes();
}


// reimplemented (iview::ILayer)

i2d::CRect CSingleLayer::GetBoundingBox() const
{
	if (m_shapePtr != NULL){
		return m_boundingBox;
	}

	return i2d::CRect::GetEmpty();
}


bool CSingleLayer::ConnectShape(IShape* shapePtr)
{
	I_ASSERT(m_shapePtr == NULL);
	if (m_shapePtr != NULL){
		return false;
	}

	shapePtr->OnConnectDisplay(this);
	shapePtr->OnDisplayChange(CF_ALL);

	m_shapePtr = shapePtr;
	m_boundingBox = shapePtr->GetBoundingBox();

	OnAreaInvalidated(i2d::CRect::GetEmpty(), m_boundingBox);

	return true;
}


int CSingleLayer::GetShapesCount() const
{
	return m_shapePtr != NULL ? 1 : 0;
}


void CSingleLayer::UpdateAllShapes(int changeFlag)
{
	if (m_shapePtr != NULL){
		if (m_shapePtr->OnDisplayChange(changeFlag)){
			m_boundingBox = m_shapePtr->GetBoundingBox();
		}
	}
}


void CSingleLayer::DisconnectAllShapes()
{
	if (m_shapePtr != NULL){
		i2d::CRect oldBoundingBox = m_boundingBox;
		m_boundingBox.Reset();

		OnAreaInvalidated(oldBoundingBox, i2d::CRect::GetEmpty());

		m_shapePtr->OnDisconnectDisplay(this);

		m_shapePtr = NULL;
	}
}


void CSingleLayer::DrawShapes(QPainter& drawContext)
{
	if (IsVisible() && (m_shapePtr != NULL) && m_shapePtr->IsVisible()){
		i2d::CRect updateRect = iqt::GetCRect(drawContext.clipRegion().boundingRect());

		if (!updateRect.IsOutside(m_boundingBox)){
			m_shapePtr->Draw(drawContext);
		}
	}
}


// reimplemented (iview::IShapeObserver)

void CSingleLayer::OnChangeShape(IShape* I_IF_DEBUG(shapePtr))
{
	I_ASSERT(shapePtr != NULL);
	I_ASSERT(m_shapePtr == shapePtr);

	i2d::CRect oldBoundingBox = m_boundingBox;
	m_boundingBox = m_shapePtr->GetBoundingBox();

	OnAreaInvalidated(oldBoundingBox, m_boundingBox);
}


void CSingleLayer::DisconnectShape(IShape* shapePtr)
{
	I_ASSERT(shapePtr != NULL);

	if (m_shapePtr == shapePtr){
		i2d::CRect oldBoundingBox = m_boundingBox;
		m_boundingBox.Reset();

		shapePtr->OnDisconnectDisplay(this);
		m_shapePtr = NULL;

		OnAreaInvalidated(oldBoundingBox, m_boundingBox);
	}
}


} // namespace iview



