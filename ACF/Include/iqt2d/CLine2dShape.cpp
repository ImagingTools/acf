#include "iqt2d/CLine2dShape.h"


#include <QStyleOptionGraphicsItem>
#include <QPainter>


namespace iqt2d
{


// public methods

// reimplemented (imod::IObserver)

void CLine2dShape::AfterUpdate(imod::IModel* /*modelPtr*/, int /*updateFlags*/, istd::IPolymorphic* /*updateParamsPtr*/)
{
	i2d::CLine2d* linePtr = GetObjectPtr();
	if (linePtr != NULL){
		setPen(QPen(Qt::green, 4));
		setLine(iqt::GetQLineF(*linePtr));
	}
}


} // namespace iqt2d


