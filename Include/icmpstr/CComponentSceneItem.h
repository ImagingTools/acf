#ifndef icmpstr_CComponentSceneItem_included
#define icmpstr_CComponentSceneItem_included


// Qt includes
#include <QGraphicsRectItem>
#include <QMouseEvent>
#include <QResizeEvent>
#include <QPaintEvent>
#include <QIcon>


#include "imod/TModelWrap.h"
#include "imod/TSingleModelObserverBase.h"

#include "icomp/IRegistry.h"
#include "icomp/IMetaInfoManager.h"


namespace icmpstr
{


class CComponentConnector;
class CRegistryView;


class CComponentSceneItem:
			public QObject,
			public QGraphicsRectItem
{
	Q_OBJECT

public:
	typedef imod::TSingleModelObserverBase<icomp::IRegistry> BaseObserverClass;

	CComponentSceneItem(
				const CRegistryView* registryViewPtr,
				icomp::IRegistry* registryPtr,
				const icomp::IRegistry::ElementInfo* elementInfoPtr,
				const std::string& componentName, 
				QGraphicsItem* parent = NULL);

	virtual ~CComponentSceneItem();

	/**
		Get registry element info associated with this object.
	*/
	const icomp::IRegistry::ElementInfo& GetElementInfo() const;
	/**
		Set registry element info associated with this object.
		\param	elementInfoPtr	pointer to element info object. It cannot be NULL.
	*/
	void SetElementInfo(const icomp::IRegistry::ElementInfo* elementInfoPtr);
	const std::string& GetComponentName() const;

	void AddConnector(CComponentConnector* connector);
	void RemoveConnector(const CComponentConnector* connector);
	void RemoveAllConnectors();

	QRectF GetInnerRect() const;

signals:
	void selectionChanged(CComponentSceneItem*, bool);
	void positionChanged(CComponentSceneItem*, const QPointF& point);

protected:
	QRect CalculateRect() const;

	void CalcExportedInteraces();

	// reimplemented (QGraphicsRectItem)
	virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0);
	virtual QVariant itemChange(GraphicsItemChange change, const QVariant& value);
	virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* eventPtr);

	class RegistryObserver: public imod::TSingleModelObserverBase<icomp::IRegistry>
	{
	public:
		RegistryObserver(CComponentSceneItem* parentPtr);

	protected:
		// reimplemented (imod::CSingleModelObserverBase)
		virtual void OnUpdate(int updateFlags, istd::IPolymorphic* updateParamsPtr);

	private:
		CComponentSceneItem& m_parent;
	};

private:
	const CRegistryView& m_registryView;

	RegistryObserver m_registryObserver;

	icomp::IRegistry& m_registry;
	std::string m_componentName;
	const icomp::IRegistry::ElementInfo* m_elementInfoPtr;

	mutable int m_componentLabelHeight;
	mutable int m_componentIdHeight;

	const int m_componentLabelFontSize;
	const int m_componentIdFontSize;

	QList<CComponentConnector*> m_connectors;

	QStringList m_exportedInterfacesList;

	QPixmap m_image;
};


} // namespace icmpstr


#endif // icmpstr_CComponentSceneItem_included


