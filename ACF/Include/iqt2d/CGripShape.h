#ifndef iqt2d_CGripShape_included
#define iqt2d_CGripShape_included


#include <QGraphicsRectItem>

#include "iqt2d/iqt2d.h"


namespace iqt2d
{


class CGripShape: public QObject, public QGraphicsEllipseItem
{
	Q_OBJECT

public:
	typedef QGraphicsEllipseItem BaseClass;

	CGripShape(QGraphicsItem* parentPtr = NULL);

signals:
	void PositionChanged(const i2d::CVector2d& position);

protected:
	virtual void OnPositionChanged(const i2d::CVector2d& position);

	// reimplemented (QGraphicsItem) 
	virtual QVariant itemChange(GraphicsItemChange change, const QVariant& value);
};


} // namespace iqt2d


#endif // !iqt2d_CGripShape_included


