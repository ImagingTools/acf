#include <iview/CStarShape.h>


// ACF includes
#include <imod/IModel.h>
#include <iview/IColorSchema.h>
#include <iview/CScreenTransform.h>


namespace iview
{



// reimplemented (iview::IVisualizable)

void CStarShape::Draw(QPainter& drawContext) const
{
	Q_ASSERT(IsDisplayConnected());

	const i2d::CPosition2d* pinPtr = dynamic_cast<const i2d::CPosition2d*>(GetObservedModel());
	if (pinPtr != NULL){
		istd::CIndex2d sp = GetScreenPosition(pinPtr->GetPosition()).ToIndex2d();

		drawContext.setPen(QPen(Qt::green, 3));

		drawContext.drawLine(sp.GetX() - 5, sp.GetY() - 5, sp.GetX() + 5, sp.GetY() + 5);
		drawContext.drawLine(sp.GetX() + 5, sp.GetY() - 5, sp.GetX() - 5, sp.GetY() + 5);
	}
}



// protected methods

// reimplemented (iview::CShapeBase)

i2d::CRect CStarShape::CalcBoundingBox() const
{
	Q_ASSERT(IsDisplayConnected());

	const i2d::CPosition2d* pinPtr = dynamic_cast<const i2d::CPosition2d*>(GetObservedModel());
	if (pinPtr != NULL){
		istd::CIndex2d sp = GetScreenPosition(pinPtr->GetPosition()).ToIndex2d();

		return i2d::CRect(sp.GetX() - 5, sp.GetY() - 5, sp.GetX() + 5, sp.GetY() + 5);
	}

	return i2d::CRect::GetEmpty();
}


} // namespace iview



