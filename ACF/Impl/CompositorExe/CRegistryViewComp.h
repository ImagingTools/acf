#ifndef CRegistryViewComp_included
#define CRegistryViewComp_included


#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsSceneDragDropEvent>
#include <QGraphicsItemGroup>
#include <QDrag>
#include <QMimeData>
#include <QDropEvent>


#include "imod/TSingleModelObserverBase.h"

#include "icomp/IComponentStaticInfo.h"
#include "icomp/IRegistryElement.h"
#include "icomp/CRegistry.h"

#include "idoc/ICommandsProvider.h"

#include "iqt/TGuiComponentBase.h"
#include "iqt/TGuiObserverWrap.h"
#include "iqt/CHierarchicalCommand.h"

#include "CComponentView.h"
#include "CStaticComponentInfo.h"


class CRegistryViewComp:	public iqt::TGuiObserverWrap<iqt::TGuiComponentBase<QGraphicsView>, 
								imod::TSingleModelObserverBase<icomp::IRegistry> >,
							public iser::ISerializable,
							public idoc::ICommandsProvider
{
	Q_OBJECT

public:

	typedef iqt::TGuiObserverWrap<iqt::TGuiComponentBase<QGraphicsView>, 
				imod::TSingleModelObserverBase<icomp::IRegistry> > BaseClass;

	I_BEGIN_COMPONENT(CRegistryViewComp)
		I_REGISTER_INTERFACE(idoc::ICommandsProvider)
		I_ASSIGN_MULTI_0(m_registryElementObserversCompPtr, "RegistryElementObservers", "Registry element observers", false)
	I_END_COMPONENT

	CRegistryViewComp();

	void OnAddComponent(const CStaticComponentInfo& componentInfo);
	void OnAddComponent(const CStaticComponentInfo& componentInfo, const QString& componentRole);

	QStringList GetComponentsForDependency(const QString& dependecySource) const;

	// reimplemented (idoc::ICommandsProvider)
	virtual const idoc::IHierarchicalCommand* GetCommands() const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

	// reimplemented (imod::IModelEditor)
	virtual void UpdateEditor();
	virtual void UpdateModel() const;

	// reimplemented (icomp::IComponent)
	virtual void OnGuiCreated();

public slots:
	void SetCenterOn(const QString& componentRole);
	void UpdateConnections();

protected slots:
	void OnComponentViewSelected(CComponentView* view, bool selected);
	void OnExportChanged(CComponentView* view, bool export);
	void OnRemoveComponent();

private:
	void ScaleView(double scaleFactor);
	void CreateConnector(CComponentView& sourceView, const std::string& referenceComponentId);
	CComponentView* CreateComponentView(const icomp::IRegistry::ElementInfo& componentRef, const QString& role);

	virtual void OnWheelEvent(QWheelEvent *event);
	virtual void OnDragEnterEvent(QDragEnterEvent *event);
	virtual void OnDropEvent(QDropEvent *event);

protected:
	class CCompositeItem: public QGraphicsRectItem
	{
	protected:
		// reimplemented (QGraphicsRectItem)
		virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0); 
		virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value);
	};
	
	class CRegistryScene: public QGraphicsScene
	{
	public:
		CRegistryScene(CRegistryViewComp& parent);

	protected:
		// reimplemented (QGraphicsScene)
		virtual void drawBackground(QPainter* painter, const QRectF & rect);
		virtual void dragEnterEvent(QGraphicsSceneDragDropEvent * event);
		virtual void dropEvent(QGraphicsSceneDragDropEvent * event);
		virtual void dragMoveEvent(QGraphicsSceneDragDropEvent * event);

	private:
		CRegistryViewComp& m_parent;
	};

private:
	I_MULTIREF(imod::IObserver, m_registryElementObserversCompPtr);

	CRegistryScene* m_scenePtr;
	CCompositeItem m_compositeItem;
	CComponentView* m_selectedComponentPtr;

	iqt::CHierarchicalCommand m_registryCommand;
};


#endif // !CRegistryViewComp_included