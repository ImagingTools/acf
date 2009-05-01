#include "icmpstr/CNoteSceneItem.h"


// STL includes
#include <cmath>

// Qt includes
#include <QPainter>

#include "i2d/CVector2d.h"

#include "icmpstr/CComponentSceneItem.h"
#include "icmpstr/CRegistryViewComp.h"


namespace icmpstr
{


// public methods

CNoteSceneItem::CNoteSceneItem(
			const CRegistryViewComp* registryViewPtr,
			CComponentSceneItem* parentComponent, 
			QGraphicsItem *parent, 
			QGraphicsScene *scene)
:	BaseClass(parent, scene),
	m_registryView(*registryViewPtr)
{
	I_ASSERT(registryViewPtr != NULL);

	setAcceptedMouseButtons(0);

	m_parentComponent = NULL;

	SetParentComponent(parentComponent);
	
	setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
}


CNoteSceneItem::~CNoteSceneItem()
{
}


CComponentSceneItem *CNoteSceneItem::GetParentComponent() const
{
	return m_parentComponent;
}


void CNoteSceneItem::SetParentComponent(CComponentSceneItem* parentComponent)
{
	m_parentComponent = parentComponent;
	if (parentComponent != NULL){
//		m_parentComponent->AddConnector(this);

		Adjust();
	}

	update();
}


void CNoteSceneItem::Adjust()
{
	if (m_parentComponent == NULL){
		return;
	}

	prepareGeometryChange();

	QRectF sourceRect = mapFromItem(m_parentComponent, m_parentComponent->GetInnerRect()).boundingRect();
}


// reimplemented (QGraphicsItem)

int CNoteSceneItem::type() const
{ 
	return Type; 
}


QPainterPath CNoteSceneItem::shape() const
{
	QPainterPath path;
	path.addRect(boundingRect());

	return path;
}


// protected members

// reimplemented (QGraphicsItem)

QRectF CNoteSceneItem::boundingRect() const
{
	if ((m_parentComponent == NULL) || (m_connectionLine.size() < 2)){
		return QRectF();
	}

	QRectF rect = m_connectionLine.boundingRect();
	rect.unite(QRectF(m_touchPoint, QSize(1, 1)));

	double extra = 1;
	return rect.adjusted(-extra, -extra, extra, extra);
}


void CNoteSceneItem::paint(QPainter* /*painter*/, const QStyleOptionGraphicsItem*, QWidget*)
{
}


} // namespace icmpstr


