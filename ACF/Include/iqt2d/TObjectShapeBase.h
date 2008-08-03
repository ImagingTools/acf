#ifndef iqt2d_TObjectShapeBase_included
#define iqt2d_TObjectShapeBase_included


#include "istd/TChangeNotifier.h"

#include "imod/TSingleModelObserverBase.h"

#include "iqt2d/TShapeBase.h"


namespace iqt2d
{


template <class GraphicsItemClass>
class TObjectShapeBase: 
			public TShapeBase<GraphicsItemClass>,
			public imod::TSingleModelObserverBase<i2d::IObject2d>
{
public:
	typedef TShapeBase<GraphicsItemClass> BaseClass;
	typedef imod::TSingleModelObserverBase<i2d::IObject2d> BaseClass2;

	TObjectShapeBase(bool isEditable = false, QGraphicsItem* parentPtr = NULL);

	void SetEditable(bool isEditable);
	bool IsEditable() const;

protected:
	// reimplemented (TShapeBase<GraphicsItemClass>) 
	virtual void OnSelectionChanged(bool isSelected);

	// reimplemented (QGraphicsItem) 
	virtual QVariant itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant& value);
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);

private:
	QPointF m_lastPosition;
	bool m_isEditable;
};


// public methods

template <class GraphicsItemClass>
TObjectShapeBase<GraphicsItemClass>::TObjectShapeBase(bool isEditable, QGraphicsItem* parentPtr)
	:BaseClass(parentPtr),
	m_isEditable(isEditable)
{
	if (isEditable){
		setFlags(ItemIsMovable | ItemIsSelectable);
		setCursor(QCursor(Qt::ArrowCursor)); 
	}
}


template <class GraphicsItemClass>
void TObjectShapeBase<GraphicsItemClass>::SetEditable(bool isEditable)
{
	m_isEditable = isEditable;
	if (isEditable){
		setFlags(ItemIsMovable | ItemIsSelectable);
	}
	else{
		setFlags(flags() & ~(ItemIsMovable | ItemIsSelectable));
	}
}


template <class GraphicsItemClass>
bool TObjectShapeBase<GraphicsItemClass>::IsEditable() const
{
	return m_isEditable;
}

// protected methods
	
// reimplemented (TShapeBase<GraphicsItemClass>) 

template <class GraphicsItemClass>
void TObjectShapeBase<GraphicsItemClass>::OnSelectionChanged(bool isSelected)
{
	if (isSelected){
		SwitchColorSheme(SelectedColor);
	}
	else{
		if (IsEditable()){
			SwitchColorSheme(EditableColor);
		}
		else{
			SwitchColorSheme(InactiveColor);		
		}
	}
}


// reimplemented (QGraphicsItem) 

template <class GraphicsItemClass>
QVariant TObjectShapeBase<GraphicsItemClass>::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant& value)
{
	if (change == BaseClass::ItemPositionChange){
		i2d::IObject2d* objectPtr = GetObjectPtr();
		if (objectPtr != NULL && change == ItemPositionChange){
			QPointF newPosition = value.toPointF();
			QPointF offset = newPosition - m_lastPosition;

			istd::CChangeNotifier changePtr(objectPtr);

			objectPtr->MoveTo(iqt::GetCVector2d(offset) + objectPtr->GetCenter());

			m_lastPosition = newPosition;

			OnPositionChanged(newPosition);

			return pos();
		}
	}

	return BaseClass::itemChange(change, value);
}


template <class GraphicsItemClass>
void TObjectShapeBase<GraphicsItemClass>::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
	BaseClass::mouseReleaseEvent(event);

	m_lastPosition = QPointF(0, 0);
}


} // namespace iqt2d


#endif // !iqt2d_TObjectShapeBase_included


