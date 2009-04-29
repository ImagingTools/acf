#ifndef CComponentConnector_included
#define CComponentConnector_included


#include <QGraphicsItem>


namespace icmpstr
{


class CComponentView;
class CRegistryView;


class CComponentConnector: public QGraphicsItem
{
public:
	typedef QGraphicsItem BaseClass;

	enum ConnectFlags
	{
		CF_FACTORY = 0x0001,
		CF_EMBEDDED  =0x0002
	};

	enum GeometricParams
	{
		GP_RADIUS = 4,
		GP_RADIUS2 = 8,
		GP_OFFSET = 12
	};

    CComponentConnector(
				const CRegistryView* registryViewPtr,
				CComponentView* sourceComponent, 
				CComponentView* destComponent,
				int connectFlags = 0,
				QGraphicsItem* parent = NULL);
    
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
	const CRegistryView& m_registryView;

    CComponentView* m_sourceComponent;
	CComponentView* m_destComponent;

	int m_connectFlags;

	QPolygonF m_connectionLine;
	QPointF m_touchPoint;
};


} // namespace icmpstr


#endif // !CComponentConnector_included


