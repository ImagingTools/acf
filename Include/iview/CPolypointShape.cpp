#include "iview/CPolypointShape.h"


// Qt includes
#include <QtCore/QVector>
#include <QtGui/QPainter>

// ACF includes
#include "imod/IModel.h"
#include "i2d/CVector2d.h"
#include "i2d/CPolypoint.h"



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


// reimplemented (iview::TShapeBase)

i2d::CRect CPolypointShape::CalcBoundingBox() const
{
	I_ASSERT(IsDisplayConnected());

	const imod::IModel* modelPtr = GetModelPtr();
	if (modelPtr != NULL){
		const i2d::CPolypoint& polypoint = *dynamic_cast<const i2d::CPolypoint*>(modelPtr);
		I_ASSERT(&polypoint != NULL);

        const IColorShema& colorShema = GetColorShema();
		const QVector<i2d::CVector2d>& points = polypoint.GetPoints();

		if (!points.isEmpty()){
			const iview::CScreenTransform& transform = GetLogToScreenTransform();

			istd::CIndex2d sp = transform.GetScreenPosition(points[0]);

			i2d::CRect boundingBox(sp, sp);

			int pointsCount = points.size();
			for (int pointIndex = 1; pointIndex < pointsCount; ++pointIndex){
				sp = transform.GetScreenPosition(points[pointIndex]);

				boundingBox.Union(sp);
			}
			const i2d::CRect& tickerBox = colorShema.GetTickerBox(IColorShema::TT_INACTIVE);

			return boundingBox.GetExpanded(tickerBox);
		}
	}

	return i2d::CRect();
}


// reimplemented (iview::IShape)

void CPolypointShape::Draw(QPainter& drawContext) const
{
	I_ASSERT(IsDisplayConnected());

	const iview::CScreenTransform& transform = GetLogToScreenTransform();

	const imod::IModel* modelPtr = GetModelPtr();
	if (modelPtr != NULL){
		const i2d::CPolypoint& polypoint = *dynamic_cast<const i2d::CPolypoint*>(modelPtr);
		I_ASSERT(&polypoint != NULL);

        const IColorShema& colorShema = GetColorShema();
		const i2d::CRect& clientArea = GetClientRect();

		const QVector<i2d::CVector2d>& points = polypoint.GetPoints();

		int pointsCount = points.size();
		for (int pointIndex = 0; pointIndex < pointsCount; ++pointIndex){
			istd::CIndex2d sp = transform.GetScreenPosition(points[pointIndex]);
			if (clientArea.IsInside(sp)){
				if (m_isSmallTickersMode){
					colorShema.DrawTicker(drawContext, sp, IColorShema::TT_INACTIVE | IColorShema::TT_SMALL);
				}
				else{
					colorShema.DrawTicker(drawContext, sp, IColorShema::TT_INACTIVE);
				}
			}
		}
	}
}


// reimplemented (imod::IObserver)

bool CPolypointShape::OnAttached(imod::IModel* modelPtr)
{
	I_ASSERT(dynamic_cast<i2d::CPolypoint*>(modelPtr) != NULL);

	return BaseClass::OnAttached(modelPtr);
}


} // namespace iview



