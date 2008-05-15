#ifndef CComponentConnector_included
#define CComponentConnector_included


#include <QGraphicsItem>


class CComponentView;


class CComponentConnector: public QGraphicsItem
{
public:
    enum { 
		Type = UserType + 2 
	};

    CComponentConnector(CComponentView *sourceComponent, CComponentView *destComponent);
    ~CComponentConnector();

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
    QPointF m_sourcePoint;
    QPointF m_destPoint;
    qreal m_arrowSize;
};


#endif // !CComponentConnector_included
