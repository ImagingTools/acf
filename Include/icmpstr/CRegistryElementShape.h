#ifndef icmpstr_CRegistryElementShape_included
#define icmpstr_CRegistryElementShape_included


// Qt includes
#include <QGraphicsItem>

#include "imod/TSingleModelObserverBase.h"

#include "iqt2d/TObjectShapeBase.h"

#include "icmpstr/CVisualRegistryElement.h"


namespace icmpstr
{


class CVisualRegistryScenographerComp;


/**
	Visualization of geometrical registry elements.
*/
class CRegistryElementShape: public iqt2d::TObjectShapeBase<QGraphicsItem, CVisualRegistryElement>
{
	Q_OBJECT

public:
	typedef iqt2d::TObjectShapeBase<QGraphicsItem, CVisualRegistryElement> BaseClass;

	CRegistryElementShape(const CVisualRegistryScenographerComp* registryViewPtr, const iqt2d::ISceneProvider* providerPtr = NULL);

	QRectF GetViewRect() const;

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

	void CalcExportedInteraces(const CVisualRegistryElement& element);

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

	// reimplemented (TShapeBase)
	virtual void OnSelectionChanged(bool isSelected);

private:
	const CVisualRegistryScenographerComp& m_registryView;

	RegistryObserver m_registryObserver;

	QRectF m_realBox;

	QStringList m_exportedInterfacesList;

	QPointF m_lastClickPosition;
};


} // namespace icmpstr


#endif // !icmpstr_CRegistryElementShape_included


