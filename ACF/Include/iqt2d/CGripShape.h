#ifndef iqt2d_CGripShape_included
#define iqt2d_CGripShape_included


#include "iqt2d/TShapeBase.h"


namespace iqt2d
{


class CGripShape: public TShapeBase<QGraphicsEllipseItem>
{
	Q_OBJECT

public:
	typedef TShapeBase<QGraphicsEllipseItem> BaseClass;

	CGripShape(QGraphicsItem* parentPtr = NULL);

signals:
	void PositionChanged(const QPointF& position);

protected:
	// reimplemented (TShapeBase<QGraphicsEllipseItem>)
	virtual void OnPositionChanged(const QPointF& position);
	virtual void OnSelectionChanged(bool isSelected);

private:
	QGraphicsSimpleTextItem m_labelItem;
};


} // namespace iqt2d


#endif // !iqt2d_CGripShape_included


