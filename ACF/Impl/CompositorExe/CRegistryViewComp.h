#ifndef CRegistryViewComp_included
#define CRegistryViewComp_included


#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsSceneDragDropEvent>
#include <QGraphicsItemGroup>
#include <QDrag>
#include <QMimeData>
#include <QDropEvent>


#include "iser/IFileLoader.h"

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
							public idoc::ICommandsProvider
{
	Q_OBJECT

public:
	typedef iqt::TGuiObserverWrap<iqt::TGuiComponentBase<QGraphicsView>, 
				imod::TSingleModelObserverBase<icomp::IRegistry> > BaseClass;

	I_BEGIN_COMPONENT(CRegistryViewComp)
		I_REGISTER_INTERFACE(idoc::ICommandsProvider)
		I_ASSIGN_MULTI_0(m_registryElementObserversCompPtr, "RegistryElementObservers", "Registry element observers", false)
		I_ASSIGN(m_registryCodeSaverCompPtr, "RegistryCodeSaver", "Export registry to C++ code file", false, "RegistryCodeSaver")
	I_END_COMPONENT;

	CRegistryViewComp();

	void OnAddComponent(const CStaticComponentInfo& componentInfo);
	void OnAddComponent(const CStaticComponentInfo& componentInfo, const QString& componentRole);

	QStringList GetComponentsForDependency(const QString& dependecySource) const;

	double GetGrid() const;

	// reimplemented (idoc::ICommandsProvider)
	virtual const idoc::IHierarchicalCommand* GetCommands() const;

	// reimplemented (imod::IModelEditor)
	virtual void UpdateEditor();
	virtual void UpdateModel() const;

	// reimplemented (iqt::CGuiComponentBase)
	virtual void OnGuiCreated();

public slots:
	void SetCenterOn(const QString& componentRole);
	void UpdateConnectors();

protected slots:
	void OnComponentViewSelected(CComponentView* view, bool selected);
	void OnExportChanged(CComponentView* view, bool export);
	void OnComponentPositionChanged(CComponentView* view, const QPoint& newPosition);
	void OnRemoveComponent();
	void OnExportToCode();

private:
	void ResetScene();
	void ScaleView(double scaleFactor);
	void CreateConnector(CComponentView& sourceView, const std::string& referenceComponentId);
	CComponentView* CreateComponentView(
				const icomp::IRegistry* registryPtr,
				const icomp::IRegistry::ElementInfo* elementInfoPtr,
				const std::string& role);

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
		virtual void keyPressEvent(QKeyEvent* keyEvent);
		virtual void wheelEvent(QGraphicsSceneWheelEvent* event);
		virtual void drawBackground(QPainter* painter, const QRectF & rect);
		virtual void dragEnterEvent(QGraphicsSceneDragDropEvent * event);
		virtual void dropEvent(QGraphicsSceneDragDropEvent * event);

	private:
		CRegistryViewComp& m_parent;
	};

private:
	I_MULTIREF(imod::IObserver, m_registryElementObserversCompPtr);
	I_REF(iser::IFileLoader, m_registryCodeSaverCompPtr);

	CRegistryScene* m_scenePtr;
	CCompositeItem m_compositeItem;
	CComponentView* m_selectedComponentPtr;

	iqt::CHierarchicalCommand m_registryCommand;
	iqt::CHierarchicalCommand m_removeComponentCommand;
};


// inline methods

inline double CRegistryViewComp::GetGrid() const
{
	return 25;
}


#endif // !CRegistryViewComp_included