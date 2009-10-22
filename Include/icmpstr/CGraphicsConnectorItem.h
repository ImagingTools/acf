#ifndef icmpstr_CGraphicsConnectorItem_included
#define icmpstr_CGraphicsConnectorItem_included


#include <QObject>
#include <QGraphicsItem>


namespace icmpstr
{


class CComponentSceneItem;
class CRegistryGuiComp;


class CGraphicsConnectorItem:
			public QObject,
			public QGraphicsItem
{
	Q_OBJECT

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

    CGraphicsConnectorItem(
				const CRegistryGuiComp* registryViewPtr,
				int connectFlags = 0,
				QGraphicsItem* parent = NULL);
    
    void Adjust();

	// reimplemented (QGraphicsItem)
    virtual int type() const;
	virtual QPainterPath shape() const;
	virtual bool contains(const QPointF& point) const;

public slots:
	void OnSourceRectMoved(const QRectF& rect);
	void OnDestRectMoved(const QRectF& rect);
	void OnSourceSelected(bool state);
	void OnDestSelected(bool state);
    
protected:
	// reimplemented (QGraphicsItem)
    virtual QRectF boundingRect() const;
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);
//    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent* event);
//	virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent* event);

private:
	const CRegistryGuiComp& m_registryView;

	int m_connectFlags;

	QPolygonF m_connectionLine;
	QPointF m_touchPoint;

	QRectF m_sourceRect;
	QRectF m_destRect;
	bool m_isSourceSelected;
	bool m_isDestSelected;
};


} // namespace icmpstr


#endif // !icmpstr_CGraphicsConnectorItem_included


