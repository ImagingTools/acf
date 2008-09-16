#ifndef CComponentNoteView_included
#define CComponentNoteView_included


#include <QGraphicsItemGroup>


class CComponentView;
class CRegistryViewComp;


class CComponentNoteView: public QGraphicsItemGroup
{
public:
	typedef QGraphicsItemGroup BaseClass;

    CComponentNoteView(
				const CRegistryViewComp* registryViewPtr,
				CComponentView *parentComponent, 
				QGraphicsItem *parent = NULL, 
				QGraphicsScene *scene = NULL);
    
	virtual ~CComponentNoteView();

    CComponentView* GetParentComponent() const;
    void SetParentComponent(CComponentView* parentComponentPtr);

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

    CComponentView* m_parentComponent;

	QPolygonF m_connectionLine;
	QPointF m_touchPoint;
};


#endif // !CComponentNoteView_included

