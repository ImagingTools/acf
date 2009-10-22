#ifndef icmpstr_CRegistryElementShape_included
#define icmpstr_CRegistryElementShape_included


// Qt includes
#include <QAbstractGraphicsShapeItem>

#include "imod/TSingleModelObserverBase.h"

#include "iqt2d/TObjectShapeBase.h"

#include "icmpstr/CGeometricalRegistryElement.h"


namespace icmpstr
{


class CRegistryGuiComp;


/**
	Visualization of geometrical registry elements.
*/
class CRegistryElementShape: public iqt2d::TObjectShapeBase<QAbstractGraphicsShapeItem, CGeometricalRegistryElement>
{
	Q_OBJECT

public:
	typedef iqt2d::TObjectShapeBase<QAbstractGraphicsShapeItem, CGeometricalRegistryElement> BaseClass;

	CRegistryElementShape(const CRegistryGuiComp* registryViewPtr);

	// reimplemented (QGraphicsItem)
	virtual QRectF boundingRect() const;
	virtual void paint(QPainter* painterPtr, const QStyleOptionGraphicsItem* stylePtr, QWidget* widgetPtr);
	virtual bool contains(const QPointF& point) const;
	virtual bool collidesWithPath(const QPainterPath& path, Qt::ItemSelectionMode mode = Qt::IntersectsItemShape) const;

	// reimplemented (imod::IObserver)
	virtual bool OnAttached(imod::IModel* modelPtr);
	virtual void AfterUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr);

signals:
	void RectChanged(QRectF rect);
	void SelectionChanged(bool state);

protected:
	enum
	{
		SIDE_OFFSET = 3,
		SHADOW_OFFSET = 10
	};

	void CalcExportedInteraces(const CGeometricalRegistryElement& element);

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
	const CRegistryGuiComp& m_registryView;

	RegistryObserver m_registryObserver;

	QRectF m_realBox;

	QStringList m_exportedInterfacesList;

	QPointF m_lastClickPosition;
};


} // namespace icmpstr


#endif // !icmpstr_CRegistryElementShape_included


