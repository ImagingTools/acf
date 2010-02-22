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
class CRegistryElementShape: public iqt2d::TObjectShapeBase<QGraphicsRectItem, CVisualRegistryElement>
{
	Q_OBJECT

public:
	typedef iqt2d::TObjectShapeBase<QGraphicsRectItem, CVisualRegistryElement> BaseClass;

	CRegistryElementShape(
				const CVisualRegistryScenographerComp* registryViewPtr,
				const iqt2d::ISceneProvider* providerPtr = NULL);

	QRectF GetViewRect() const;

	/**
		Check consistency, if it is changed do update.
	*/
	void CheckConsistency();

	// reimplemented (QGraphicsItem)
	virtual QRectF boundingRect() const;
	virtual void paint(QPainter* painterPtr, const QStyleOptionGraphicsItem* stylePtr, QWidget* widgetPtr);
	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* eventPtr);
	virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* eventPtr);

	// reimplemented (imod::IObserver)
	virtual bool OnAttached(imod::IModel* modelPtr);

signals:
	void RectChanged(QRectF rect);
	void SelectionChanged(bool state);

protected:
	enum
	{
		SIDE_OFFSET = 4,
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

	// reimplemented (iqt2d::TObjectShapeBase)
	void UpdateGraphicsItem(const CVisualRegistryElement& element);

	// reimplemented (TShapeBase)
	virtual void OnSelectionChanged(bool isSelected);

private:
	const CVisualRegistryScenographerComp& m_registryView;

	RegistryObserver m_registryObserver;

	QStringList m_exportedInterfacesList;

	QPointF m_lastClickPosition;

	QIcon m_icon;
	bool m_isConsistent;
};


} // namespace icmpstr


#endif // !icmpstr_CRegistryElementShape_included


