#ifndef icmpstr_CRegistryElementShape_included
#define icmpstr_CRegistryElementShape_included


// Qt includes
#include <QGraphicsItem>
#include <QMouseEvent>
#include <QIcon>

#include "imod/TSingleModelObserverBase.h"

#include "iqt2d/TObjectShapeBase.h"

#include "icmpstr/CGeometricalRegistryElement.h"
#include "icmpstr/CRegistryView.h"


namespace icmpstr
{


class CRegistryElementShape: public iqt2d::TObjectShapeBase<QGraphicsItem, CGeometricalRegistryElement>
{
public:
	typedef iqt2d::TObjectShapeBase<QGraphicsItem, CGeometricalRegistryElement> BaseClass;

	CRegistryElementShape(const CRegistryView* registryViewPtr);

	// reimplemented (QGraphicsItem)
	virtual QRectF boundingRect() const;
	virtual void paint(QPainter* painterPtr, const QStyleOptionGraphicsItem* stylePtr, QWidget* widgetPtr);
	virtual bool contains(const QPointF& point) const;

	// reimplemented (imod::IObserver)
	virtual bool OnAttached(imod::IModel* modelPtr);
	virtual void AfterUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr);

protected:
	void CalcExportedInteraces();

	// reimplemented (QGraphicsItem)
    virtual void mousePressEvent(QGraphicsSceneMouseEvent* eventPtr);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* eventPtr);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* eventPtr);

	class RegistryObserver: public imod::TSingleModelObserverBase<icomp::IRegistry>
	{
	public:
		typedef imod::TSingleModelObserverBase<icomp::IRegistry> BaseClass;

		RegistryObserver(CRegistryElementShape* parentPtr);

	protected:
		// reimplemented (imod::CSingleModelObserverBase)
		virtual void OnUpdate(int updateFlags, istd::IPolymorphic* updateParamsPtr);

	private:
		CRegistryElementShape& m_parent;
	};

private:
	const CRegistryView& m_registryView;

	RegistryObserver m_registryObserver;

	QRectF m_boundingBox;
	QPixmap m_image;
	int m_componentLabelHeight;
	int m_componentIdHeight;
	QStringList m_exportedInterfacesList;

	QPointF m_lastClickPosition;
};


} // namespace icmpstr


#endif // !icmpstr_CRegistryElementShape_included


