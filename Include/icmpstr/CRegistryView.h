#ifndef CRegistryView_included
#define CRegistryView_included


// STL includes
#include <string>

// Qt includes
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsSceneDragDropEvent>
#include <QMimeData>

#include "istd/TChangeNotifier.h"

#include "icomp/IRegistriesManager.h"

#include "icmpstr/CComponentSceneItem.h"


namespace icmpstr
{


class CRegistryView: public QGraphicsView
{
	Q_OBJECT

public:
	typedef QGraphicsView BaseClass;
	typedef QList<CComponentSceneItem*> ComponentViewList;

	CRegistryView(QWidget* parent = NULL);

	void CreateConnector(CComponentSceneItem& sourceView, const std::string& referenceComponentId, bool isFactory = false);

	CComponentSceneItem* CreateComponentView(
				icomp::IRegistry* registryPtr,
				const icomp::IRegistry::ElementInfo* elementInfoPtr,
				const std::string& role);

	static double GetGrid();

	CComponentSceneItem* GetSelectedComponent() const;
	void SetSelectedComponent(CComponentSceneItem* selectedComponentPtr);

	void RemoveSelectedComponent();
	void RemoveAllConnectors();

	ComponentViewList GetComponentViews() const;

	void SetCenterOn(const QString& componentName);
	void OnDropDataEvent(const QMimeData& data, QGraphicsSceneDragDropEvent* eventPtr);
	void UpdateCompositeGeometry();
	void ResetScene();
	void ScaleView(double scaleFactor);
	QIcon GetIcon(const icomp::CComponentAddress& address) const;

	void Init(const icomp::IRegistriesManager* managerPtr, icomp::IRegistry* registryPtr);

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
		virtual void mousePressEvent(QGraphicsSceneMouseEvent* mouseEvent);
		virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* mouseEvent);

	private:
		CRegistryView& m_parent;
	};

private:
	CCompositeItem m_compositeItem;
	CRegistryScene* m_scenePtr;
	CComponentSceneItem* m_selectedComponentPtr;
	icomp::IRegistry* m_registryPtr;
	const icomp::IRegistriesManager* m_packagesManagerPtr;

	istd::CChangeNotifier m_mousePressingNotifier;
};


// inline methods

inline double CRegistryView::GetGrid()
{
	return 25;
}


} // namespace icmpstr


#endif // !CRegistryView_included


