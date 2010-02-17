#ifndef iqt2d_TShapeBase_included
#define iqt2d_TShapeBase_included


#include <QObject>
#include <QBrush>
#include <QPen>
#include <QVariant>
#include <QMap>
#include <QPointF>
#include <QGraphicsItem>
#include <QStyle>
#include <QStyleOptionGraphicsItem>


#include "iqt2d/ISceneProvider.h"


namespace iqt2d
{


template <class GraphicsItemClass>
class TShapeBase: public QObject, public GraphicsItemClass
{
public:
	typedef GraphicsItemClass BaseClass;

	enum ColorSheme
	{
		/**
			Color for a selected shape
		*/
		SelectedColor,

		/**
			Color for a shape, that can be edited on the graphics view
		*/
		EditableColor,

		/**
			Color for a shape, that can not be edited on the graphics view
		*/
		InactiveColor,

		/**
			Standard shape color.
		*/
		DefaultColor = InactiveColor
	};

	TShapeBase(bool isEditable = false, const ISceneProvider* providerPtr = NULL);

	virtual void SetPen(int colorSheme, const QPen& pen);
	virtual QPen GetPen(int colorSheme) const;
	
	virtual void SetBrush(int colorSheme, const QBrush& pen);
	virtual QBrush GetBrush(int colorSheme) const;

	virtual void SwitchColorSheme(int colorSheme);

	bool IsEditable() const;
	void SetEditable(bool isEditable);

	const ISceneProvider* GetSceneProvider() const;
	
protected:
	/**
		Map position from local graphics item coordinates.
	*/
	i2d::CVector2d GetPosFromLocal(const QPointF& position) const;
	/**
		Map position from local graphics item coordinates.
	*/
	QPointF GetLocalFromPos(const i2d::CVector2d& position) const;

	virtual void OnSelectionChanged(bool isSelected);
	virtual void OnPositionChanged(const QPointF& position);

	// reimplemented (QGraphicsItem) 
	virtual QVariant itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant& value);
	virtual void hoverEnterEvent(QGraphicsSceneHoverEvent* event);
	virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent* event);
	virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = NULL);

private:
	typedef QPair<QPen, QBrush> ColorShemeInfo;
	typedef QMap<int, ColorShemeInfo> ColorShemeMap;

	bool m_isEditable;
	const ISceneProvider* m_providerPtr;

	ColorShemeMap m_colorShemeMap;
};


// public methods

template <class GraphicsItemClass>
TShapeBase<GraphicsItemClass>::TShapeBase(bool isEditable, const ISceneProvider* providerPtr)
:	m_isEditable(isEditable),
	m_providerPtr(providerPtr)
{
	BaseClass::setAcceptsHoverEvents(true);
}


template <class GraphicsItemClass>
void TShapeBase<GraphicsItemClass>::SetPen(int colorSheme, const QPen& pen)
{
	ColorShemeInfo info;
	
	if (m_colorShemeMap.contains(colorSheme)){
		info = m_colorShemeMap.value(colorSheme);
	}

	info.first = pen;
	m_colorShemeMap[colorSheme] = info;
}


template <class GraphicsItemClass>
QPen TShapeBase<GraphicsItemClass>::GetPen(int colorSheme) const
{
	if (m_colorShemeMap.contains(colorSheme)){
		return m_colorShemeMap.value(colorSheme).first;
	}

	return QPen();
}
	

template <class GraphicsItemClass>
void TShapeBase<GraphicsItemClass>::SetBrush(int colorSheme, const QBrush& brush)
{
	ColorShemeInfo info;
	
	if (m_colorShemeMap.contains(colorSheme)){
		info = m_colorShemeMap.value(colorSheme);
	}

	info.second = brush;
	m_colorShemeMap[colorSheme] = info;
}


template <class GraphicsItemClass>
QBrush TShapeBase<GraphicsItemClass>::GetBrush(int colorSheme) const
{
	if (m_colorShemeMap.contains(colorSheme)){
		return m_colorShemeMap.value(colorSheme).second;
	}

	return QBrush();
}


template <class GraphicsItemClass>
void TShapeBase<GraphicsItemClass>::SwitchColorSheme(int colorSheme)
{
	QAbstractGraphicsShapeItem* itemPtr = dynamic_cast<QAbstractGraphicsShapeItem*>(this);
	if (itemPtr != NULL){
		QBrush brush = GetBrush(colorSheme);
		itemPtr->setBrush(brush);

		QPen pen = GetPen(colorSheme);
		itemPtr->setPen(pen);
	}
	else{
		QGraphicsLineItem* linePtr = dynamic_cast<QGraphicsLineItem*>(this);
		if (linePtr != NULL){
			QPen pen = GetPen(colorSheme);
			linePtr->setPen(pen);
		}
	}
}


template <class GraphicsItemClass>
bool TShapeBase<GraphicsItemClass>::IsEditable() const
{
	return m_isEditable;
}


template <class GraphicsItemClass>
void TShapeBase<GraphicsItemClass>::SetEditable(bool isEditable)
{
	m_isEditable = isEditable;
	if (isEditable){
		BaseClass::setFlags(BaseClass::flags() | BaseClass::ItemIsMovable | BaseClass::ItemIsSelectable | BaseClass::ItemSendsGeometryChanges);
	}
	else{
		BaseClass::setFlags(BaseClass::flags() & ~(BaseClass::ItemIsMovable | BaseClass::ItemIsSelectable));
	}
}


template <class GraphicsItemClass>
const ISceneProvider* TShapeBase<GraphicsItemClass>::GetSceneProvider() const
{
	return m_providerPtr;
}


// protected methods
	
template <class GraphicsItemClass>
i2d::CVector2d TShapeBase<GraphicsItemClass>::GetPosFromLocal(const QPointF& position) const
{
	return iqt::GetCVector2d(mapToScene(position));
}


template <class GraphicsItemClass>
QPointF TShapeBase<GraphicsItemClass>::GetLocalFromPos(const i2d::CVector2d& position) const
{
	return mapFromScene(iqt::GetQPointF(position));
}


template <class GraphicsItemClass>
void TShapeBase<GraphicsItemClass>::OnSelectionChanged(bool isSelected)
{
	if (isSelected){
		SwitchColorSheme(SelectedColor);
	}
	else{
		SwitchColorSheme(DefaultColor);
	}
}


template <class GraphicsItemClass>
void TShapeBase<GraphicsItemClass>::OnPositionChanged(const QPointF& /*position*/)
{
}


// reimplemented (QGraphicsItem) 

template <class GraphicsItemClass>
QVariant TShapeBase<GraphicsItemClass>::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant& value)
{
	if (change == BaseClass::ItemPositionHasChanged){
		QPointF position = value.toPointF();

		if (m_providerPtr != NULL){
			double distance;
			if (m_providerPtr->GetSceneAlignment(distance)){
				position.setX(::floor(position.x() / distance) * distance);
				position.setY(::floor(position.y() / distance) * distance);
			}
		}

		OnPositionChanged(position);

		return position;
	}

	if (change == BaseClass::ItemSelectedChange){
		OnSelectionChanged(value.toBool());
	}

	return BaseClass::itemChange(change, value);
}


template <class GraphicsItemClass>
void TShapeBase<GraphicsItemClass>::hoverEnterEvent(QGraphicsSceneHoverEvent* /*event*/)
{
	if (m_isEditable){
		OnSelectionChanged(true);
	}
}


template <class GraphicsItemClass>
void TShapeBase<GraphicsItemClass>::hoverLeaveEvent(QGraphicsSceneHoverEvent* /*event*/)
{
	if (m_isEditable){
		OnSelectionChanged(false);
	}
}


template <class GraphicsItemClass>
void TShapeBase<GraphicsItemClass>::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	// remove selected flag to prevent the drawing of the cruel rubber band:

	if ((option != NULL) && ((option->state & QStyle::State_Selected) != 0)){
		QStyleOptionGraphicsItem newOption = *option;

		newOption.state &= ~QStyle::State_Selected;

		BaseClass::paint(painter, &newOption, widget);
	}
	else{
		BaseClass::paint(painter, option, widget);
	}
}


// specialized implementations

template <>
inline void TShapeBase<QGraphicsItem>::paint(QPainter* /*painter*/, const QStyleOptionGraphicsItem* /*option*/, QWidget* /*widget*/)
{
}


template <>
inline void TShapeBase<QAbstractGraphicsShapeItem>::paint(QPainter* /*painter*/, const QStyleOptionGraphicsItem* /*option*/, QWidget* /*widget*/)
{
}


} // namespace iqt2d


#endif // !iqt2d_TShapeBase_included


