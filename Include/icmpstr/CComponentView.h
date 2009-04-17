#ifndef CComponentView_included
#define CComponentView_included


// Qt includes
#include <QGraphicsRectItem>
#include <QMouseEvent>
#include <QResizeEvent>
#include <QPaintEvent>
#include <QIcon>


#include "icomp/IRegistry.h"

#include "icmpstr/icmpstr.h"


namespace icmpstr
{


class CComponentConnector;
class CRegistryView;


class CComponentView: public QObject, public QGraphicsRectItem
{
	Q_OBJECT

public:
	CComponentView(
				const CRegistryView* registryViewPtr,
				const icomp::IRegistry* registryPtr,
				const icomp::IRegistry::ElementInfo* elementInfoPtr, 
				const std::string& componentName, 
				QGraphicsItem* parent = NULL);

	virtual ~CComponentView();

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
	void selectionChanged(CComponentView*, bool);
	void positionChanged(CComponentView*, const QPoint& point);

protected:
	QRect CalculateRect() const;

	void CalcExportedInteraces();

	// reimplemented (QGraphicsRectItem)
	virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0);
	virtual QVariant itemChange(GraphicsItemChange change, const QVariant& value);

private:
	const CRegistryView& m_registryView;

	const icomp::IRegistry& m_registry;
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


#endif // CComponentView_included

