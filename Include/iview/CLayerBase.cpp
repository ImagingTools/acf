#include "iview/CLayerBase.h"


// Qt includes
#include <QtGui/QPainter>


// ACF includes
#include "iqt/iqt.h"

#include "iview/IShape.h"
#include "iview/IShapeView.h"


namespace iview
{


CLayerBase::CLayerBase()
{
}


CLayerBase::~CLayerBase()
{
	DisconnectAllShapes();
}


// reimplemented (iview::TLayerBaseEx)

void CLayerBase::CalcBoundingBox(i2d::CRect& result) const
{
	result.Reset();

	for (ShapeMap::const_iterator iter = m_shapes.begin(); iter != m_shapes.end(); ++iter){
		const i2d::CRect& boundingBox = iter.value();
		
		result.Union(boundingBox);
	}
}


// reimplemented (iview::ILayer)

bool CLayerBase::IsShapeConnected(IShape* shapePtr)
{
	return (m_shapes.find(shapePtr) != m_shapes.end());
}


void CLayerBase::ConnectShape(IShape* shapePtr)
{
	shapePtr->OnConnectDisplay(this);
	shapePtr->OnDisplayChange(CF_ALL);

	i2d::CRect boundingBox = shapePtr->GetBoundingBox();
	m_shapes[shapePtr] = boundingBox;

	OnAreaInvalidated(i2d::CRect::GetEmpty(), boundingBox);
}


int CLayerBase::GetShapesCount() const
{
	return int(m_shapes.size());
}


void CLayerBase::UpdateAllShapes(int changeFlag)
{
	i2d::CRect& boundingBox = GetBoundingBoxRef();
	boundingBox.Reset();

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
	
	SetBoundingBoxValid();
}


void CLayerBase::DisconnectAllShapes()
{
	while (!m_shapes.empty()){
		DisconnectShapeElement(m_shapes, m_shapes.begin());
	}
}


void CLayerBase::DrawShapes(QPainter& drawContext)
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


// reimplemented (iview::IShapeObserver)

void CLayerBase::OnChangeShape(IShape* shapePtr)
{
	I_ASSERT(shapePtr != NULL);

	ShapeMap::iterator iter = m_shapes.find(shapePtr);
	I_ASSERT(iter != m_shapes.end());

	OnChangeShapeElement(iter);
}


void CLayerBase::DisconnectShape(IShape* shapePtr)
{
	I_ASSERT(shapePtr != NULL);

	ShapeMap::iterator iter = m_shapes.find(shapePtr);
	if (iter != m_shapes.end()){
		DisconnectShapeElement(m_shapes, iter);
	}
}


} // namespace iview


