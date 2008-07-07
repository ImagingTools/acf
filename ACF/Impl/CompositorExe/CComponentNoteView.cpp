#include <QPainter>
#include <math.h>


#include "i2d/CVector2d.h"

#include "CComponentView.h"
#include "CComponentNoteView.h"
#include "CRegistryViewComp.h"


// public methods

CComponentNoteView::CComponentNoteView(
			const CRegistryViewComp* registryViewPtr,
			CComponentView* parentComponent, 
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


CComponentNoteView::~CComponentNoteView()
{
}


CComponentView *CComponentNoteView::GetParentComponent() const
{
	return m_parentComponent;
}


void CComponentNoteView::SetParentComponent(CComponentView* parentComponent)
{
	m_parentComponent = parentComponent;
	if (parentComponent != NULL){
//		m_parentComponent->AddConnector(this);

		Adjust();
	}

	update();
}


void CComponentNoteView::Adjust()
{
	if (m_parentComponent == NULL){
		return;
	}

	prepareGeometryChange();

	QRectF sourceRect = mapFromItem(m_parentComponent, m_parentComponent->GetInnerRect()).boundingRect();
}


// reimplemented (QGraphicsItem)

int CComponentNoteView::type() const
{ 
	return Type; 
}


QPainterPath CComponentNoteView::shape() const
{
	QPainterPath path;
	path.addRect(boundingRect());

	return path;
}


// protected members

// reimplemented (QGraphicsItem)

QRectF CComponentNoteView::boundingRect() const
{
	if ((m_parentComponent == NULL) || (m_connectionLine.size() < 2)){
		return QRectF();
	}

	double penWidth = 1.0;

	QRectF rect = m_connectionLine.boundingRect();
	rect.unite(QRectF(m_touchPoint, QSize(1, 1)));

	double extra = 1;
	return rect.adjusted(-extra, -extra, extra, extra);
}


void CComponentNoteView::paint(QPainter *painter, const QStyleOptionGraphicsItem*, QWidget*)
{
}

