#ifndef CRegistryView_included
#define CRegistryView_included


// STL includes
#include <string>

// Qt includes
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsSceneDragDropEvent>
#include <QMimeData>


// Compositor includes
#include "icmpstr/CComponentView.h"


namespace icmpstr
{


class CRegistryView: public QGraphicsView
{
	Q_OBJECT

public:
	typedef QGraphicsView BaseClass;
	typedef QList<CComponentView*> ComponentViewList;

	CRegistryView(QWidget* parent = NULL);

	void CreateConnector(CComponentView& sourceView, const std::string& referenceComponentId);

	CComponentView* CreateComponentView(
				const icomp::IRegistry* registryPtr,
				const icomp::IRegistry::ElementInfo* elementInfoPtr,
				const std::string& role);

	static double GetGrid();

	const CComponentView* GetSelectedComponent() const;
	void SetSelectedComponent(CComponentView* selectedComponentPtr);

	void RemoveSelectedComponent();
	void RemoveAllConnectors();

	ComponentViewList GetComponentViews() const;

	void SetCenterOn(const QString& componentName);
	void OnDropDataEvent(const QMimeData& data, QGraphicsSceneDragDropEvent* eventPtr);
	void UpdateCompositeGeometry();
	void ResetScene();
	void ScaleView(double scaleFactor);

signals:
	void DropDataEventEntered(const QMimeData& data, QGraphicsSceneDragDropEvent* eventPtr);

protected:
	// reimplemented (QWidget)
	virtual void resizeEvent(QResizeEvent* eventPtr);

public:
	class CCompositeItem: public QGraphicsRectItem
	{
	protected:
		// reimplemented (QGraphicsRectItem)
		virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0); 
		virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value);
		virtual QRectF boundingRect() const;
	};

	class CRegistryScene: public QGraphicsScene
	{
	public:
		typedef QGraphicsScene BaseClass;

		CRegistryScene(CRegistryView& parent);

	protected:
		// reimplemented (QGraphicsScene)
		virtual void keyPressEvent(QKeyEvent* keyEvent);
		virtual void wheelEvent(QGraphicsSceneWheelEvent* eventPtr);
		virtual void drawBackground(QPainter* painter, const QRectF & rect);
		virtual void dragEnterEvent(QGraphicsSceneDragDropEvent * eventPtr);
		virtual void dropEvent(QGraphicsSceneDragDropEvent * eventPtr);
		virtual void dragMoveEvent(QGraphicsSceneDragDropEvent * eventPtr);

	private:
		CRegistryView& m_parent;
	};

private:
	CCompositeItem m_compositeItem;
	CRegistryScene* m_scenePtr;
	CComponentView* m_selectedComponentPtr;
};


// inline methods

inline double CRegistryView::GetGrid()
{
	return 25;
}


} // namespace icmpstr


#endif // !CRegistryView_included


