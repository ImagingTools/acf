#ifndef CRegistryViewComp_included
#define CRegistryViewComp_included


#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsSceneDragDropEvent>
#include <QGraphicsItemGroup>
#include <QDrag>
#include <QMimeData>
#include <QTimer>


#include "iser/IFileLoader.h"

#include "imod/TSingleModelObserverBase.h"

#include "icomp/IComponentStaticInfo.h"
#include "icomp/IRegistryElement.h"
#include "icomp/CRegistry.h"

#include "idoc/ICommandsProvider.h"

#include "iqtgui/TGuiComponentBase.h"
#include "iqtgui/TGuiObserverWrap.h"
#include "iqtgui/CHierarchicalCommand.h"

#include "IRegistryPreview.h"
#include "CComponentView.h"
#include "CComponentNoteView.h"


class CRegistryViewComp:
			public iqtgui::TGuiObserverWrap<
						iqtgui::TGuiComponentBase<QGraphicsView>, 
						imod::TSingleModelObserverBase<icomp::IRegistry> >,
			public idoc::ICommandsProvider
{
	Q_OBJECT

public:
	typedef iqtgui::TGuiObserverWrap<
				iqtgui::TGuiComponentBase<QGraphicsView>, 
				imod::TSingleModelObserverBase<icomp::IRegistry> > BaseClass;

	I_BEGIN_COMPONENT(CRegistryViewComp)
		I_REGISTER_INTERFACE(idoc::ICommandsProvider)
		I_REGISTER_INTERFACE(imod::IObserver)
		I_ASSIGN_MULTI_0(m_registryElementObserversCompPtr, "RegistryElementObservers", "Registry element observers", false)
		I_ASSIGN(m_registryCodeSaverCompPtr, "RegistryCodeSaver", "Export registry to C++ code file", false, "RegistryCodeSaver")
		I_ASSIGN(m_registryPreviewCompPtr, "RegistryPreview", "Executes preview of the registry", false, "RegistryPreview")
	I_END_COMPONENT;

	CRegistryViewComp();

	bool TryCreateComponent(const icomp::CComponentAddress& address, const i2d::CVector2d& position);
	static double GetGrid();

	// reimplemented (idoc::ICommandsProvider)
	virtual const idoc::IHierarchicalCommand* GetCommands() const;

	// reimplemented (imod::IModelEditor)
	virtual void UpdateEditor();
	virtual void UpdateModel() const;

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();
	virtual void OnRetranslate();

public slots:
	void SetCenterOn(const std::string& componentRole);
	void UpdateConnectors();
	void OnExecutionTimerTick();

protected slots:
	void OnComponentViewSelected(CComponentView* view, bool selected);
	void OnComponentPositionChanged(CComponentView* view, const QPoint& newPosition);
	void OnRemoveComponent();
	void OnRenameComponent();
	void OnExportInterface();
	void OnExportComponent();
	void OnExportToCode();
	void OnExecute();
	void OnAbort();
	void OnAddNote();
	void OnRemoveNote();

private:
	void ResetScene();
	void ScaleView(double scaleFactor);
	void CreateConnector(CComponentView& sourceView, const std::string& referenceComponentId);
	CComponentView* CreateComponentView(
				const icomp::IRegistry* registryPtr,
				const icomp::IRegistry::ElementInfo* elementInfoPtr,
				const std::string& role);

	bool ProcessDroppedData(const QMimeData& data, QGraphicsSceneDragDropEvent* event);
	void ConnectReferences(const QString& componentRole);
	bool HasExportedInterfaces(const CComponentView& componentView) const;
	void UpdateExportInterfaceCommand();

protected:
	enum GroupId
	{
		GI_COMPONENT = 0x5430,
		GI_CODEGEN,
		GI_PREVIEW
	};

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
		virtual void dragMoveEvent(QGraphicsSceneDragDropEvent * event);

	private:
		CRegistryViewComp& m_parent;
	};

private:
	I_MULTIREF(imod::IObserver, m_registryElementObserversCompPtr);
	I_REF(iser::IFileLoader, m_registryCodeSaverCompPtr);
	I_REF(IRegistryPreview, m_registryPreviewCompPtr);

	CRegistryScene* m_scenePtr;
	CCompositeItem m_compositeItem;
	CComponentView* m_selectedComponentPtr;

	iqtgui::CHierarchicalCommand m_registryCommand;
	iqtgui::CHierarchicalCommand m_registryMenu;
	iqtgui::CHierarchicalCommand m_removeComponentCommand;
	iqtgui::CHierarchicalCommand m_renameComponentCommand;
	iqtgui::CHierarchicalCommand m_exportToCodeCommand;
	iqtgui::CHierarchicalCommand m_executeRegistryCommand;
	iqtgui::CHierarchicalCommand m_abortRegistryCommand;
	iqtgui::CHierarchicalCommand m_addNoteCommand;
	iqtgui::CHierarchicalCommand m_removeNoteCommand;
	iqtgui::CHierarchicalCommand m_exportComponentCommand;
	iqtgui::CHierarchicalCommand m_exportInterfaceCommand;

	QTimer m_executionObserverTimer;
};


// inline methods

inline double CRegistryViewComp::GetGrid()
{
	return 25;
}


#endif // !CRegistryViewComp_included


