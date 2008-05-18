#ifndef CComponentConnector_included
#define CComponentConnector_included


#include <QGraphicsItem>


class CComponentView;


class CComponentConnector: public QGraphicsItem
{
public:
	typedef QGraphicsItem BaseClass;

	enum GeometricParams
	{
		GP_RADIUS = 4,
		GP_RADIUS2 = 8,
		GP_OFFSET = 12
	};

    CComponentConnector(	CComponentView *sourceComponent, 
							CComponentView *destComponent, 
							QGraphicsItem *parent = NULL, 
							QGraphicsScene *scene = NULL);
    
	virtual ~CComponentConnector();

    CComponentView* GetSourceComponent() const;
    void SetSourceComponent(CComponentView* sourceComponent);

    CComponentView* GetDestinationComponent() const;
    void SetDestinationComponent(CComponentView* destComponent);

    void Adjust();

	// reimplemented (QGraphicsItem)
    virtual int type() const;
	virtual QPainterPath shape() const;
    
protected:
	// reimplemented (QGraphicsItem)
    virtual QRectF boundingRect() const;
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent* event);
	virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent* event);

private:
    CComponentView* m_sourceComponent;
	CComponentView* m_destComponent;

	QPolygonF m_connectionLine;
	QPointF m_touchPoint;
};


#endif // !CComponentConnector_included
