#ifndef icmpstr_CRegistryView_included
#define icmpstr_CRegistryView_included


// STL includes
#include <string>

// Qt includes
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsSceneDragDropEvent>
#include <QGraphicsRectItem>
#include <QMimeData>

#include "istd/TChangeNotifier.h"

#include "icomp/IComponentEnvironmentManager.h"
#include "icomp/IRegistry.h"

#include "idoc/IMainWindowCommands.h"


namespace icmpstr
{


class CComponentSceneItem;


class CRegistryView: public QGraphicsView
{
	Q_OBJECT

public:
	typedef QGraphicsView BaseClass;
	typedef QList<CComponentSceneItem*> ComponentViewList;

	CRegistryView(QWidget* parent);

	void CreateConnector(
				CComponentSceneItem& sourceView,
				const std::string& referenceComponentId,
				const std::string& attributeId,
				bool isFactory = false);

	CComponentSceneItem* CreateComponentView(
				icomp::IRegistry* registryPtr,
				const icomp::IRegistry::ElementInfo* elementInfoPtr,
				const std::string& role);

	CComponentSceneItem* GetSelectedComponent() const;
	void SetSelectedComponent(CComponentSceneItem* selectedComponentPtr);

	const icomp::IComponentEnvironmentManager* GetPackagesManager() const;

	void RemoveSelectedComponent();
	void RemoveAllConnectors();

	ComponentViewList GetComponentViews() const;

	void SetCenterOn(const QString& componentName);
	void OnDropDataEvent(const QMimeData& data, QGraphicsSceneDragDropEvent* eventPtr);
	void UpdateCompositeGeometry();
	void ResetScene();
	void ScaleView(double scaleFactor);
	QIcon GetIcon(const icomp::CComponentAddress& address) const;

	void Init(const icomp::IComponentEnvironmentManager* managerPtr, icomp::IRegistry* registryPtr, idoc::IMainWindowCommands* commandsPtr);

	virtual idoc::IMainWindowCommands* GetMainWindowCommands() const;

	// static methods
	static double GetGrid();

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
	const icomp::IComponentEnvironmentManager* m_environmentManagerPtr;
	idoc::IMainWindowCommands* m_mainWindowCommandsPtr;

	istd::CChangeNotifier m_mousePressingNotifier;
};


// inline methods

inline const icomp::IComponentEnvironmentManager* CRegistryView::GetPackagesManager() const
{
	return m_environmentManagerPtr;
}


// static methods

inline double CRegistryView::GetGrid()
{
	return 25;
}


} // namespace icmpstr


#endif // !icmpstr_CRegistryView_included


