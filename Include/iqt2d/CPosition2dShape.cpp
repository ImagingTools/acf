#include "iqt2d/CPosition2dShape.h"


#include <QPen>
#include <QBrush>

#include "istd/TChangeNotifier.h"


namespace iqt2d
{


// public methods

CPosition2dShape::CPosition2dShape(bool isEditable, const ISceneProvider* providerPtr)
:	BaseClass(NULL, providerPtr),
	m_ignoreUpdate(false)
{
	SetEditable(isEditable);
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

void CPosition2dShape::OnPositionChanged(const QPointF& position)
{
	i2d::CPosition2d* positionPtr = GetObjectPtr();
	if ((positionPtr != NULL) && !m_ignoreUpdate){
		m_ignoreUpdate = true;

		positionPtr->SetCenter(iqt::GetCVector2d(position));

		m_ignoreUpdate = false;
	}
}


} // namespace iqt2d


