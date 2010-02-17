#ifndef icmpstr_CVisualRegistryScenographerComp_included
#define icmpstr_CVisualRegistryScenographerComp_included


// STL includes
#include <map>

// Qt includes
#include <QObject>
#include <QTimer>
#include <QGraphicsView>
#include <QGraphicsScene>


// ACF includes
#include "iser/IFileLoader.h"

#include "imod/TSingleModelObserverBase.h"
#include "imod/TModelWrap.h"

#include "icomp/IRegistry.h"
#include "icomp/IComponentEnvironmentManager.h"
#include "icomp/CComponentBase.h"

#include "ibase/ICommandsProvider.h"
#include "idoc/IHelpViewer.h"
#include "idoc/IMainWindowCommands.h"

#include "iqtgui/IDropConsumer.h"
#include "iqtgui/TGuiComponentBase.h"
#include "iqtgui/TGuiObserverWrap.h"
#include "iqtgui/CHierarchicalCommand.h"

#include "iqt2d/ISceneProvider.h"

#include "icmpstr/IRegistryPreview.h"


namespace icmpstr
{


class CRegistryElementShape;
class CVisualRegistryElement;
class CVisualRegistryComp;


class CVisualRegistryScenographerComp:
			public QObject,
			public icomp::CComponentBase,
			public imod::TSingleModelObserverBase<icomp::IRegistry>,
			virtual public ibase::ICommandsProvider,
			virtual public iqtgui::IDropConsumer
{
	Q_OBJECT

public:
	typedef icomp::CComponentBase BaseClass;
	typedef imod::TSingleModelObserverBase<icomp::IRegistry> BaseClass2;

	I_BEGIN_COMPONENT(CVisualRegistryScenographerComp);
		I_REGISTER_INTERFACE(iqtgui::IDropConsumer);
		I_REGISTER_INTERFACE(ibase::ICommandsProvider);
		I_REGISTER_INTERFACE(imod::IObserver);
		I_ASSIGN(m_registryCodeSaverCompPtr, "RegistryCodeSaver", "Export registry to C++ code file", false, "RegistryCodeSaver");
		I_ASSIGN(m_registryPreviewCompPtr, "RegistryPreview", "Executes preview of the registry", false, "RegistryPreview");
		I_ASSIGN(m_envManagerCompPtr, "MetaInfoManager", "Allows access to component meta information", true, "MetaInfoManager");
		I_ASSIGN(m_quickHelpViewerCompPtr, "QuickHelpViewer", "Show help of selected component using its address", false, "HelpViewer");
		I_ASSIGN(m_mainWindowCompPtr, "MainWindow", "Access to main window command", false, "MainWindow");
		I_ASSIGN(m_sceneProviderCompPtr, "SceneProvider", "Display view where the registry will be shown", true, "SceneProvider");
	I_END_COMPONENT;

	CVisualRegistryScenographerComp();

	const QFont& GetElementNameFont() const;
	const QFont& GetElementDetailFont() const;
	
	const icomp::IComponentEnvironmentManager* GetEnvironmentManager() const;

	double GetGrid() const{return 25;}	// TODO: replace it with some geometrical info concept

	// reimplemented (iqtgui::IDropConsumer)
	virtual QStringList GetAcceptedMimeIds() const;
	virtual void OnDropFinished(const QMimeData& data, QEvent* eventPtr);

	// reimplemented (ibase::ICommandsProvider)
	virtual const ibase::IHierarchicalCommand* GetCommands() const;

	// reimplemented (imod::CSingleModelObserverBase)
	virtual void OnUpdate(int updateFlags, istd::IPolymorphic* updateParamsPtr);

	// reimplemented (icomp::IComponent)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

protected:
	void DoRetranslate();

protected slots:
	void OnSelectionChanged();
	void OnRemoveComponent();
	void OnRenameComponent();
	void OnExportToCode();
	void OnExecute();
	void OnAbort();
	void OnAddNote();
	void OnRemoveNote();
	bool OnDroppedData(const QMimeData& data, QGraphicsSceneDragDropEvent* eventPtr);
	void OnExecutionTimerTick();

protected:
	enum GroupId
	{
		GI_COMPONENT = 0x5430,
		GI_CODEGEN,
		GI_PREVIEW
	};

	/**
		Create instance of shape representing some element.
		The shape will be automatically conncted to element using model/observer pattern.
		\sa imod.
	*/
	QGraphicsItem* AddShapeToScene(iser::ISerializable* elementPtr) const;
	void AddConnectorsToScene();
	void AddConnector(
				CRegistryElementShape& sourceShape,
				const std::string& referenceComponentId,
				const std::string& attributeId,
				bool isFactory = false);
	bool TryCreateComponent(const icomp::CComponentAddress& address, const i2d::CVector2d& position);
	void ConnectReferences(const QString& componentRole);
	void UpdateComponentSelection();

private:
	I_REF(iser::IFileLoader, m_registryCodeSaverCompPtr);
	I_REF(IRegistryPreview, m_registryPreviewCompPtr);
	I_REF(icomp::IComponentEnvironmentManager, m_envManagerCompPtr);
	I_REF(idoc::IHelpViewer, m_quickHelpViewerCompPtr);
	I_REF(idoc::IMainWindowCommands, m_mainWindowCompPtr);
	I_REF(iqt2d::ISceneProvider, m_sceneProviderCompPtr);

	iqtgui::CHierarchicalCommand m_registryCommand;
	iqtgui::CHierarchicalCommand m_registryMenu;
	iqtgui::CHierarchicalCommand m_removeComponentCommand;
	iqtgui::CHierarchicalCommand m_renameComponentCommand;
	iqtgui::CHierarchicalCommand m_exportToCodeCommand;
	iqtgui::CHierarchicalCommand m_executeRegistryCommand;
	iqtgui::CHierarchicalCommand m_abortRegistryCommand;
	iqtgui::CHierarchicalCommand m_addNoteCommand;
	iqtgui::CHierarchicalCommand m_removeNoteCommand;

	QTimer m_executionObserverTimer;

	QGraphicsScene* m_scenePtr;

	QFont m_elementNameFont;
	QFont m_elementDetailFont;

	bool m_isUpdating;
};


// inline methods

inline const icomp::IComponentEnvironmentManager* CVisualRegistryScenographerComp::GetEnvironmentManager() const
{
	return m_envManagerCompPtr.GetPtr();
}


} // namespace icmpstr


#endif // !icmpstr_CVisualRegistryScenographerComp_included


