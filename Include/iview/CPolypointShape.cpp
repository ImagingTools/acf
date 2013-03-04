#include "iview/CPolypointShape.h"


// Qt includes
#include <QtCore/QVector>
#include <QtGui/QPainter>

// ACF includes
#include "imod/IModel.h"

#include "i2d/CVector2d.h"
#include "i2d/CPolypoint.h"

#include "iview/IColorSchema.h"
#include "iview/CScreenTransform.h"


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
		Invalidate(CS_CONSOLE);
	}
}


// reimplemented (iview::IShape)

void CPolypointShape::Draw(QPainter& drawContext) const
{
	Q_ASSERT(IsDisplayConnected());

	const imod::IModel* modelPtr = GetModelPtr();
	if (modelPtr != NULL){
		const i2d::CPolypoint& polypoint = *dynamic_cast<const i2d::CPolypoint*>(modelPtr);
		Q_ASSERT(&polypoint != NULL);

		const IColorSchema& colorSchema = GetColorSchema();
		const i2d::CRect& clientArea = GetClientRect();

		const QVector<i2d::CVector2d>& points = polypoint.GetPoints();

		int pointsCount = points.size();
		for (int pointIndex = 0; pointIndex < pointsCount; ++pointIndex){
			istd::CIndex2d sp = GetScreenPosition(points[pointIndex]).ToIndex2d();
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

bool CPolypointShape::OnAttached(imod::IModel* modelPtr)
{
	Q_ASSERT(dynamic_cast<i2d::CPolypoint*>(modelPtr) != NULL);

	return BaseClass::OnAttached(modelPtr);
}


// protected methods

// reimplemented (iview::CShapeBase)

i2d::CRect CPolypointShape::CalcBoundingBox() const
{
	Q_ASSERT(IsDisplayConnected());

	const imod::IModel* modelPtr = GetModelPtr();
	if (modelPtr != NULL){
		const i2d::CPolypoint& polypoint = *dynamic_cast<const i2d::CPolypoint*>(modelPtr);
		Q_ASSERT(&polypoint != NULL);

		const IColorSchema& colorSchema = GetColorSchema();
		const QVector<i2d::CVector2d>& points = polypoint.GetPoints();

		if (!points.isEmpty()){
			istd::CIndex2d sp = GetScreenPosition(points[0]).ToIndex2d();

			i2d::CRect boundingBox(sp, sp);

			int pointsCount = points.size();
			for (int pointIndex = 1; pointIndex < pointsCount; ++pointIndex){
				sp = GetScreenPosition(points[pointIndex]).ToIndex2d();

				boundingBox.Union(sp);
			}
			const i2d::CRect& tickerBox = colorSchema.GetTickerBox(IColorSchema::TT_INACTIVE);

			return boundingBox.GetExpanded(tickerBox);
		}
	}

	return i2d::CRect::GetEmpty();
}


} // namespace iview



