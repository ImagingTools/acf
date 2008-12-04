#ifndef iqt2d_TObjectShapeBase_included
#define iqt2d_TObjectShapeBase_included


// Qt includes
#include <QGraphicsSceneMouseEvent>

#include "istd/TChangeNotifier.h"

#include "imod/TSingleModelObserverBase.h"

#include "iqt2d/TShapeBase.h"


namespace iqt2d
{


template <class GraphicsItemClass, class ObjectClass>
class TObjectShapeBase: 
			public TShapeBase<GraphicsItemClass>,
			public imod::TSingleModelObserverBase<ObjectClass>
{
public:
	typedef TShapeBase<GraphicsItemClass> BaseClass;
	typedef imod::TSingleModelObserverBase<ObjectClass> BaseClass2;

	TObjectShapeBase(bool isEditable = false, QGraphicsItem* parentPtr = NULL);

protected:
	// reimplemented (TShapeBase<GraphicsItemClass, ObjectClass>) 
	virtual void OnSelectionChanged(bool isSelected);

	// reimplemented (QGraphicsItem) 
	virtual QVariant itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant& value);
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* eventPtr);

private:
	QPointF m_lastPosition;
};


// public methods

template <class GraphicsItemClass, class ObjectClass>
TObjectShapeBase<GraphicsItemClass, ObjectClass>::TObjectShapeBase(bool isEditable, QGraphicsItem* parentPtr)
:	BaseClass(isEditable, parentPtr)
{
	BaseClass::SetPen(BaseClass::InactiveColor, QPen(Qt::darkGreen, 0));
	BaseClass::SetPen(BaseClass::EditableColor, QPen(Qt::green, 0));
	BaseClass::SetPen(BaseClass::SelectedColor, QPen(Qt::yellow, 0));

	if (isEditable){
		BaseClass::setFlags(BaseClass::ItemIsMovable | BaseClass::ItemIsSelectable);
		BaseClass::setCursor(QCursor(Qt::ArrowCursor)); 

		BaseClass::SwitchColorSheme(BaseClass::EditableColor);
	}
	else{
		BaseClass::SwitchColorSheme(BaseClass::InactiveColor);
	}
}


// protected methods
	
// reimplemented (TShapeBase<GraphicsItemClass, ObjectClass>) 

template <class GraphicsItemClass, class ObjectClass>
void TObjectShapeBase<GraphicsItemClass, ObjectClass>::OnSelectionChanged(bool isSelected)
{
	if (isSelected){
		BaseClass::SwitchColorSheme(BaseClass::SelectedColor);
	}
	else{
		if (BaseClass::IsEditable()){
			BaseClass::SwitchColorSheme(BaseClass::EditableColor);
		}
		else{
			BaseClass::SwitchColorSheme(BaseClass::InactiveColor);		
		}
	}
}


// reimplemented (QGraphicsItem) 

template <class GraphicsItemClass, class ObjectClass>
QVariant TObjectShapeBase<GraphicsItemClass, ObjectClass>::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant& value)
{
	if (change == BaseClass::ItemPositionChange){
		i2d::IObject2d* objectPtr = BaseClass2::GetObjectPtr();
		if ((objectPtr != NULL) && (change == BaseClass::ItemPositionChange)){
			QPointF newPosition = value.toPointF();
			QPointF offset = newPosition - m_lastPosition;

			istd::CChangeNotifier changePtr(objectPtr);

			objectPtr->MoveTo(iqt::GetCVector2d(offset) + objectPtr->GetCenter());

			m_lastPosition = newPosition;

			return BaseClass::pos();
		}
	}

	return BaseClass::itemChange(change, value);
}


template <class GraphicsItemClass, class ObjectClass>
void TObjectShapeBase<GraphicsItemClass, ObjectClass>::mouseReleaseEvent(QGraphicsSceneMouseEvent* eventPtr)
{
	BaseClass::mouseReleaseEvent(eventPtr);

	if (eventPtr->button() == Qt::LeftButton){
		m_lastPosition = QPointF(0, 0);
	}
}


} // namespace iqt2d


#endif // !iqt2d_TObjectShapeBase_included


