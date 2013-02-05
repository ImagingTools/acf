#ifndef icmpstr_TObjectShapeBase_included
#define icmpstr_TObjectShapeBase_included


// Qt includes
#include <QtGui/QGraphicsSceneMouseEvent>

// ACF includes
#include "istd/TChangeNotifier.h"

#include "imod/TSingleModelObserverBase.h"

#include "i2d/IObject2d.h"

#include "icmpstr/TShapeBase.h"


namespace icmpstr
{


template <class GraphicsItemClass, class ObjectClass>
class TObjectShapeBase: 
			public TShapeBase<GraphicsItemClass>,
			public imod::TSingleModelObserverBase<ObjectClass>
{
public:
	typedef TShapeBase<GraphicsItemClass> BaseClass;
	typedef imod::TSingleModelObserverBase<ObjectClass> BaseClass2;

	TObjectShapeBase(bool isEditable = false, const ISceneProvider* providerPtr = NULL);

protected:
	// reimplemented (TShapeBase)
	virtual void OnSelectionChanged(bool isSelected);
	virtual void OnPositionChanged(const QPointF& position);

	// reimplemented (QGraphicsItem) 
	virtual QVariant itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant& value);
	virtual void mousePressEvent(QGraphicsSceneMouseEvent* eventPtr);
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* eventPtr);
	virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* eventPtr);

	// reimplemented (imod::IObserver)
	virtual void AfterUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr);

	// abstract methods
	virtual void UpdateGraphicsItem(const ObjectClass& object) = 0;

private:
	QPointF m_lastPosition;
	bool m_isObjectMoved;

	bool m_isShapeUpdateBlocked;

	istd::CChangeNotifier m_mousePressingNotifier;
};


// public methods

template <class GraphicsItemClass, class ObjectClass>
TObjectShapeBase<GraphicsItemClass, ObjectClass>::TObjectShapeBase(bool isEditable, const ISceneProvider* providerPtr)
:	BaseClass(isEditable, providerPtr),
	m_isShapeUpdateBlocked(false),
	m_mousePressingNotifier(NULL, i2d::IObject2d::CF_OBJECT_POSITION | istd::IChangeable::CF_MODEL)
{
	BaseClass::SetPen(BaseClass::InactiveColor, QPen(Qt::darkGreen, 0));
	BaseClass::SetPen(BaseClass::EditableColor, QPen(Qt::green, 0));
	BaseClass::SetPen(BaseClass::SelectedColor, QPen(Qt::yellow, 0));

	if (isEditable){
		BaseClass::setFlags(BaseClass::ItemIsMovable | BaseClass::ItemIsSelectable | BaseClass::ItemSendsGeometryChanges);
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


template <class GraphicsItemClass, class ObjectClass>
void TObjectShapeBase<GraphicsItemClass, ObjectClass>::OnPositionChanged(const QPointF& position)
{
	i2d::IObject2d* objectPtr = BaseClass2::GetObjectPtr();
	if (objectPtr != NULL){
		QPointF offset = position - m_lastPosition;

		objectPtr->MoveCenterTo(i2d::CVector2d(offset) + objectPtr->GetCenter());

		m_lastPosition = position;
		m_isObjectMoved = true;
	}

	BaseClass::OnPositionChanged(position);
}


template <class GraphicsItemClass, class ObjectClass>
QVariant TObjectShapeBase<GraphicsItemClass, ObjectClass>::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant& value)
{
	if (!m_isShapeUpdateBlocked){
		return BaseClass::itemChange(change, value);
	}

	return value;
}


template <class GraphicsItemClass, class ObjectClass>
void TObjectShapeBase<GraphicsItemClass, ObjectClass>::mousePressEvent(QGraphicsSceneMouseEvent* eventPtr)
{
	BaseClass::mousePressEvent(eventPtr);

	if (BaseClass::IsEditable() && (eventPtr->button() == Qt::LeftButton)){
		m_lastPosition = BaseClass::pos();
		m_isObjectMoved = false;

		m_mousePressingNotifier.SetPtr(BaseClass2::GetObjectPtr());
	}
}


template <class GraphicsItemClass, class ObjectClass>
void TObjectShapeBase<GraphicsItemClass, ObjectClass>::mouseReleaseEvent(QGraphicsSceneMouseEvent* eventPtr)
{
	BaseClass::mouseReleaseEvent(eventPtr);

	if (m_isObjectMoved){
		m_mousePressingNotifier.Reset();
	}
	else{
		m_mousePressingNotifier.Abort();
	}
}


template <class GraphicsItemClass, class ObjectClass>
void TObjectShapeBase<GraphicsItemClass, ObjectClass>::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* eventPtr)
{
	BaseClass::mouseDoubleClickEvent(eventPtr);

	m_mousePressingNotifier.Reset();
}


// reimplemented (imod::IObserver)

template <class GraphicsItemClass, class ObjectClass>
void TObjectShapeBase<GraphicsItemClass, ObjectClass>::AfterUpdate(imod::IModel* /*modelPtr*/, int /*updateFlags*/, istd::IPolymorphic* /*updateParamsPtr*/)
{
	Q_ASSERT(!m_isShapeUpdateBlocked);

	ObjectClass* objectPtr = BaseClass2::GetObjectPtr();
	if (objectPtr != NULL){
		m_isShapeUpdateBlocked = true;

		UpdateGraphicsItem(*objectPtr);

		m_isShapeUpdateBlocked = false;

		BaseClass::update();
	}
}


} // namespace icmpstr


#endif // !icmpstr_TObjectShapeBase_included


