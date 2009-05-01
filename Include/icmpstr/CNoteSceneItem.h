#ifndef CNoteSceneItem_included
#define CNoteSceneItem_included


#include <QGraphicsItemGroup>


namespace icmpstr
{


class CComponentSceneItem;
class CRegistryViewComp;


class CNoteSceneItem: public QGraphicsItemGroup
{
public:
	typedef QGraphicsItemGroup BaseClass;

    CNoteSceneItem(
				const CRegistryViewComp* registryViewPtr,
				CComponentSceneItem *parentComponent, 
				QGraphicsItem *parent = NULL, 
				QGraphicsScene *scene = NULL);
    
	virtual ~CNoteSceneItem();

    CComponentSceneItem* GetParentComponent() const;
    void SetParentComponent(CComponentSceneItem* parentComponentPtr);

    void Adjust();

	// reimplemented (QGraphicsItem)
    virtual int type() const;
	virtual QPainterPath shape() const;
    
protected:
	// reimplemented (QGraphicsItem)
    virtual QRectF boundingRect() const;
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);

private:
	const CRegistryViewComp& m_registryView;

    CComponentSceneItem* m_parentComponent;

	QPolygonF m_connectionLine;
	QPointF m_touchPoint;
};


} // namespace icmpstr


#endif // !CNoteSceneItem_included

