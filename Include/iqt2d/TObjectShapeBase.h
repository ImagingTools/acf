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

	TObjectShapeBase(bool isEditable = false, const ISceneProvider* providerPtr = NULL);

protected:
	// reimplemented (TShapeBase)
	virtual void OnSelectionChanged(bool isSelected);
	virtual void OnPositionChanged(const QPointF& position);
};


// public methods

template <class GraphicsItemClass, class ObjectClass>
TObjectShapeBase<GraphicsItemClass, ObjectClass>::TObjectShapeBase(bool isEditable, const ISceneProvider* providerPtr)
:	BaseClass(isEditable, providerPtr)
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


template <class GraphicsItemClass, class ObjectClass>
void TObjectShapeBase<GraphicsItemClass, ObjectClass>::OnPositionChanged(const QPointF& position)
{
	i2d::IObject2d* objectPtr = BaseClass2::GetObjectPtr();
	if (objectPtr != NULL){
		istd::CChangeNotifier changePtr(objectPtr, i2d::IObject2d::CF_OBJECT_POSITION | istd::IChangeable::CF_MODEL);

		objectPtr->MoveTo(iqt::GetCVector2d(position));
	}

	BaseClass::OnPositionChanged(position);
}


} // namespace iqt2d


#endif // !iqt2d_TObjectShapeBase_included


