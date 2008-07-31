#include "iqt2d/CPosition2dShape.h"


#include <QPen>
#include <QBrush>

#include "istd/TChangeNotifier.h"

#include "iqt/CSignalBlocker.h"


namespace iqt2d
{


// public methods

CPosition2dShape::CPosition2dShape()
:	m_ignoreUpdate(false)
{
	setFlags(ItemIsMovable | ItemIsSelectable);
}


// reimplemented (imod::IObserver)

void CPosition2dShape::AfterUpdate(imod::IModel* /*modelPtr*/, int /*updateFlags*/, istd::IPolymorphic* /*updateParamsPtr*/)
{
	i2d::CPosition2d* positionPtr = GetObjectPtr();
	if ((positionPtr != NULL) && !m_ignoreUpdate){
		setPos((iqt::GetQPointF(positionPtr->GetCenter())));
	}
}


// protected methods

// reimplemented (CGripShape) 

void CPosition2dShape::OnPositionChanged(const i2d::CVector2d& position)
{
	i2d::CPosition2d* positionPtr = GetObjectPtr();
	if ((positionPtr != NULL) && !m_ignoreUpdate){
		m_ignoreUpdate = true;

		positionPtr->SetCenter(position);

		m_ignoreUpdate = false;
	}
}


} // namespace iqt2d


