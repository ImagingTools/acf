#ifndef CComponentView_included
#define CComponentView_included


#include <QGraphicsRectItem>
#include <QMouseEvent>
#include <QResizeEvent>
#include <QPaintEvent>


#include "iser/ISerializable.h"

#include "icomp/IRegistry.h"


class CComponentConnector;


class CComponentView: public QObject, public QGraphicsRectItem, public iser::ISerializable
{
	Q_OBJECT

public:
	typedef icomp::IRegistry::ElementInfo ComponentData;

	CComponentView( const ComponentData& componentRef, 
					const QString& componentName, 
					QGraphicsItem* parent = NULL, 
					QGraphicsScene * scene = NULL);

	virtual ~CComponentView();

	const ComponentData* GetComponent() const;
	QString GetComponentName() const;

	void AddConnector(CComponentConnector* connector);
	void RemoveConnector(const CComponentConnector* connector);
	void RemoveAllConnectors();

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

protected slots:
	void OnExportChanged(bool state);
	void OnRename();

signals:
	void selectionChanged(CComponentView*, bool);
	void exportChanged(CComponentView*, bool);

protected:
	QRectF GetInnerRect() const;
	QRect CalculateRect() const; 

	// reimplemented QGraphicsRectItem
	virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent* event);
	virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0);
	virtual QVariant itemChange(GraphicsItemChange change, const QVariant& value);

private:
	const ComponentData& m_componentRef;
	QString m_componentName;
	QMenu* m_contextMenu;

	mutable int m_componentLabelHeight;
	mutable int m_componentIdHeight;

	const int m_componentLabelFontSize;
	const int m_componentIdFontSize;
	const int m_gridSize;

	QList<CComponentConnector*> m_connectors;
};


#endif // CComponentView_included

