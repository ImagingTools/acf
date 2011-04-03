#ifndef icmpstr_CGraphicsConnectorItem_included
#define icmpstr_CGraphicsConnectorItem_included


// Qt includes
#include <QObject>
#include <QGraphicsItem>


// ACF includes
#include "iqt2d/ISceneProvider.h"


namespace icmpstr
{


class CComponentSceneItem;
class CRegistryElementShape;


class CGraphicsConnectorItem:
			public QObject,
			public QGraphicsItem
{
	Q_OBJECT
	Q_INTERFACES(QGraphicsItem)

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
				const iqt2d::ISceneProvider& sceneProvider,
				int connectFlags = 0,
				QGraphicsItem* parent = NULL);

	void InitEnds(CRegistryElementShape* sourceShapePtr, CRegistryElementShape* destShapePtr);
    
	// reimplemented (QGraphicsItem)
    virtual int type() const;
	virtual QPainterPath shape() const;
	virtual bool contains(const QPointF& point) const;

protected:
    void Adjust();

	// reimplemented (QGraphicsItem)
    virtual QRectF boundingRect() const;
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);

protected slots:
	void OnSourceRectMoved(const QRectF& rect);
	void OnDestRectMoved(const QRectF& rect);
	void OnSourceSelected(bool state);
	void OnDestSelected(bool state);

private:
	const iqt2d::ISceneProvider& m_sceneProvider;

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


