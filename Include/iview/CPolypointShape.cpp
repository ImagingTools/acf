#include <iview/CPolypointShape.h>


// Qt includes
#include <QtCore/QVector>
#include <QtGui/QPainter>

// ACF includes
#include <imod/IModel.h>

#include <i2d/CVector2d.h>
#include <i2d/CPolypoint.h>

#include <iview/IColorSchema.h>
#include <iview/CScreenTransform.h>


namespace iview
{


// constructors

CPolypointShape::CPolypointShape()
{
	m_isSmallTickersMode = false;
}


bool CPolypointShape::IsSmallTickersMode() const
{
	return m_isSmallTickersMode;
}


void CPolypointShape::SetSmallTickersMode(bool state)
{
	if (m_isSmallTickersMode != state){
		m_isSmallTickersMode = state;
		Invalidate();
	}
}


// reimplemented (iview::IShape)

void CPolypointShape::Draw(QPainter& drawContext) const
{
	Q_ASSERT(IsDisplayConnected());

	const imod::IModel* modelPtr = GetObservedModel();
	if (modelPtr != NULL){
		const i2d::CPolypoint& polypoint = *dynamic_cast<const i2d::CPolypoint*>(modelPtr);
		Q_ASSERT(&polypoint != NULL);

		const IColorSchema& colorSchema = GetColorSchema();
		const i2d::CRect& clientArea = GetClientRect();

		int pointsCount = polypoint.GetNodesCount();
		for (int pointIndex = 0; pointIndex < pointsCount; ++pointIndex){
			istd::CIndex2d sp = GetScreenPosition(polypoint.GetNodePos(pointIndex)).ToIndex2d();
			if (clientArea.IsInside(sp)){
				if (m_isSmallTickersMode){
					colorSchema.DrawTicker(drawContext, sp, IColorSchema::TT_INACTIVE | IColorSchema::TT_SMALL);
				}
				else{
					colorSchema.DrawTicker(drawContext, sp, IColorSchema::TT_INACTIVE);
				}
			}
		}
	}
}


// reimplemented (imod::IObserver)

bool CPolypointShape::OnModelAttached(imod::IModel* modelPtr, istd::IChangeable::ChangeSet& changeMask)
{
	Q_ASSERT(dynamic_cast<i2d::CPolypoint*>(modelPtr) != NULL);

	return BaseClass::OnModelAttached(modelPtr, changeMask);
}


// protected methods

// reimplemented (iview::CShapeBase)

i2d::CRect CPolypointShape::CalcBoundingBox() const
{
	Q_ASSERT(IsDisplayConnected());

	const imod::IModel* modelPtr = GetObservedModel();
	if (modelPtr != NULL){
		const i2d::CPolypoint& polypoint = *dynamic_cast<const i2d::CPolypoint*>(modelPtr);
		Q_ASSERT(&polypoint != NULL);

		const IColorSchema& colorSchema = GetColorSchema();
		int pointsCount = polypoint.GetNodesCount();

		if (pointsCount > 0){
			istd::CIndex2d sp = GetScreenPosition(polypoint.GetNodePos(0)).ToIndex2d();

			i2d::CRect boundingBox(sp, sp);

			for (int pointIndex = 1; pointIndex < pointsCount; ++pointIndex){
				sp = GetScreenPosition(polypoint.GetNodePos(pointIndex)).ToIndex2d();

				boundingBox.Union(sp);
			}
			const i2d::CRect& tickerBox = colorSchema.GetTickerBox(IColorSchema::TT_INACTIVE);

			return boundingBox.GetExpanded(tickerBox);
		}
	}

	return i2d::CRect::GetEmpty();
}


} // namespace iview



