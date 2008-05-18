#ifndef CComponentView_included
#define CComponentView_included


#include <QGraphicsRectItem>
#include <QMouseEvent>
#include <QResizeEvent>
#include <QPaintEvent>


#include "icomp/IRegistry.h"


class CComponentConnector;


class CComponentView: public QObject, public QGraphicsRectItem
{
	Q_OBJECT

public:
	CComponentView( 
				const icomp::IRegistry* registryPtr,
				const icomp::IRegistry::ElementInfo* elementInfoPtr, 
				const QString& componentName, 
				QGraphicsItem* parent = NULL, 
				QGraphicsScene * scene = NULL);

	virtual ~CComponentView();

	const icomp::IRegistry::ElementInfo& GetElementInfo() const;
	QString GetComponentName() const;

	void AddConnector(CComponentConnector* connector);
	void RemoveConnector(const CComponentConnector* connector);
	void RemoveAllConnectors();

	QRectF GetInnerRect() const;

protected slots:
	void OnExportChanged(bool state);
	void OnRename();

signals:
	void selectionChanged(CComponentView*, bool);
	void exportChanged(CComponentView*, bool);
	void positionChanged(CComponentView*, const QPoint& point);

protected:
	QRect CalculateRect() const;

	void CalcExportedInteraces();

	// reimplemented QGraphicsRectItem
	virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent* event);
	virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0);
	virtual QVariant itemChange(GraphicsItemChange change, const QVariant& value);

private:
	const icomp::IRegistry& m_registry;
	const icomp::IRegistry::ElementInfo& m_elementInfo;
	QString m_componentName;
	QMenu* m_contextMenu;

	mutable int m_componentLabelHeight;
	mutable int m_componentIdHeight;

	const int m_componentLabelFontSize;
	const int m_componentIdFontSize;
	const int m_gridSize;

	QList<CComponentConnector*> m_connectors;

	QStringList m_exportedInterfacesList;
};


#endif // CComponentView_included

