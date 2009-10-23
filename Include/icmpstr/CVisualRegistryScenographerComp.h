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
#include "icomp/IRegistriesManager.h"
#include "icomp/CComponentBase.h"

#include "idoc/ICommandsProvider.h"
#include "idoc/IHelpViewer.h"
#include "idoc/IMainWindowCommands.h"

#include "iqtgui/TGuiComponentBase.h"
#include "iqtgui/TGuiObserverWrap.h"
#include "iqtgui/CHierarchicalCommand.h"

#include "iqt2d/ISceneProvider.h"

#include "icmpstr/IRegistryPreview.h"
#include "icmpstr/IElementSelectionInfo.h"


namespace icmpstr
{


class CRegistryElementShape;
class CVisualRegistryElement;


class CVisualRegistryScenographerComp:
			public QObject,
			public icomp::CComponentBase,
			public imod::TSingleModelObserverBase<icomp::IRegistry>,
			virtual public imod::TModelWrap<IElementSelectionInfo>,
			virtual public idoc::ICommandsProvider
{
	Q_OBJECT

public:
	typedef icomp::CComponentBase BaseClass;
	typedef imod::TSingleModelObserverBase<icomp::IRegistry> BaseClass2;

	I_BEGIN_COMPONENT(CVisualRegistryScenographerComp);
		I_REGISTER_INTERFACE(idoc::ICommandsProvider);
		I_REGISTER_INTERFACE(imod::IObserver);
		I_REGISTER_INTERFACE(imod::IModel);
		I_ASSIGN(m_registryCodeSaverCompPtr, "RegistryCodeSaver", "Export registry to C++ code file", false, "RegistryCodeSaver");
		I_ASSIGN(m_registryPreviewCompPtr, "RegistryPreview", "Executes preview of the registry", false, "RegistryPreview");
		I_ASSIGN(m_packagesManagerCompPtr, "PackagesManager", "Packages manager used to provide icon paths", true, "PackagesManager");
		I_ASSIGN(m_quickHelpViewerCompPtr, "QuickHelpViewer", "Show help of selected component using its address", false, "HelpViewer");
		I_ASSIGN(m_mainWindowCompPtr, "MainWindow", "Access to main window command", false, "MainWindow");
		I_ASSIGN(m_sceneProviderCompPtr, "SceneProvider", "Display view where the registry will be shown", true, "SceneProvider");
	I_END_COMPONENT;

	CVisualRegistryScenographerComp();

	const QFont& GetElementNameFont() const;
	const QFont& GetElementDetailFont() const;
	const QIcon* GetIcon(const icomp::CComponentAddress& address) const;

	double GetGrid() const{return 25;}	// TODO: replace it with some geometrical info concept

	// reimplemented (idoc::ICommandsProvider)
	virtual const idoc::IHierarchicalCommand* GetCommands() const;

	// reimplemented (icmpstr::IElementSelectionInfo)
	virtual icomp::IRegistry* GetSelectedRegistry() const;
	virtual iser::ISerializable* GetSelectedElement() const;
	virtual const std::string& GetSelectedElementName() const;
	virtual const QIcon* GetSelectedElementIcon() const;
	virtual const icomp::CComponentAddress* GetSelectedElementAddress() const;

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
	void OnProperties();
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

private:
	I_REF(iser::IFileLoader, m_registryCodeSaverCompPtr);
	I_REF(IRegistryPreview, m_registryPreviewCompPtr);
	I_REF(icomp::IRegistriesManager, m_packagesManagerCompPtr);
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
	iqtgui::CHierarchicalCommand m_propertiesCommand;

	QTimer m_executionObserverTimer;

	QGraphicsScene* m_scenePtr;

	typedef istd::TDelPtr<QIcon> IconPtr;
	typedef std::map<icomp::CComponentAddress, IconPtr> IconMap;
	mutable IconMap m_iconMap;

	QFont m_elementNameFont;
	QFont m_elementDetailFont;
};


} // namespace icmpstr


#endif // !icmpstr_CVisualRegistryScenographerComp_included


