#ifndef icmpstr_CVisualRegistryScenographerComp_included
#define icmpstr_CVisualRegistryScenographerComp_included


// Qt includes
#include <QtCore/QObject>
#include <QtCore/QTimer>

// ACF includes
#include "istd/TPointerVector.h"
#include "iser/IFileLoader.h"
#include "imod/CMultiModelDispatcherBase.h"
#include "icomp/IRegistry.h"
#include "icomp/IComponentEnvironmentManager.h"
#include "ibase/ICommandsProvider.h"
#include "idoc/IHelpViewer.h"
#include "idoc/IDocumentManager.h"
#include "iqtgui/TDesignerGuiObserverCompBase.h"
#include "iqtgui/CHierarchicalCommand.h"
#include "iqtgui/IDropConsumer.h"
#include "icmpstr/IRegistryPreview.h"
#include "icmpstr/IRegistryConsistInfo.h"
#include "icmpstr/IElementSelectionInfo.h"
#include "icmpstr/ISceneProvider.h"

#include "Generated/ui_CVisualRegistryScenographerComp.h"


namespace icmpstr
{

class CRegistryElementShape;
class CVisualRegistryElement;
class CVisualRegistry;


class CVisualRegistryScenographerComp:
			public iqtgui::TDesignerGuiObserverCompBase<Ui::CVisualRegistryScenographerComp, icomp::IRegistry>,
			virtual public iqtgui::IDropConsumer,
			virtual public ibase::ICommandsProvider,
			protected imod::CMultiModelDispatcherBase
{
	Q_OBJECT

public:
	iqtgui::TDesignerGuiObserverCompBase<Ui::CVisualRegistryScenographerComp, icomp::IRegistry> BaseClass2;

	I_BEGIN_COMPONENT(CVisualRegistryScenographerComp);
		I_REGISTER_INTERFACE(ibase::ICommandsProvider);
		I_REGISTER_INTERFACE(iqtgui::IDropConsumer);
		I_REGISTER_INTERFACE(imod::IObserver);
		I_REGISTER_INTERFACE(iqtgui::IGuiObject);
		I_REGISTER_SUBELEMENT(SelectionInfo);
		I_REGISTER_SUBELEMENT_INTERFACE(SelectionInfo, IElementSelectionInfo, ExtractSelectionInterface);
		I_REGISTER_SUBELEMENT_INTERFACE(SelectionInfo, imod::IModel, ExtractSelectionInterfaceModel);
		I_REGISTER_SUBELEMENT_INTERFACE(SelectionInfo, istd::IChangeable, ExtractSelectionInterfaceChangeable);
		I_ASSIGN(m_sceneProviderCompPtr, "SceneProvider", "Display view where graphical objects will be shown", true, "SceneProvider");
		I_ASSIGN_TO(m_sceneProviderGuiCompPtr, m_sceneProviderCompPtr, true);
		I_ASSIGN(m_registryCodeSaverCompPtr, "RegistryCodeSaver", "Export registry to C++ code file", false, "RegistryCodeSaver");
		I_ASSIGN(m_registryPreviewCompPtr, "RegistryPreview", "Executes preview of the registry", false, "RegistryPreview");
		I_ASSIGN(m_envManagerCompPtr, "MetaInfoManager", "Allows access to component meta information", true, "MetaInfoManager");
		I_ASSIGN_TO(m_envManagerModelCompPtr, m_envManagerCompPtr, false);
		I_ASSIGN(m_quickHelpViewerCompPtr, "QuickHelpGui", "Show help of selected component using its address", false, "QuickHelpGui");
		I_ASSIGN(m_documentManagerCompPtr, "DocumentManager", "Document manager allowing to load files on double click", false, "DocumentManager");
		I_ASSIGN(m_consistencyInfoCompPtr, "ConsistencyInfo", "Allows to check consistency of registries and attributes", false, "ConsistencyInfo");
		I_ASSIGN(m_registryTopologyGuiCompPtr, "RegistryTopologyGui", "GUI for showing the registry component topology", false, "RegistryTopologyGui");
		I_ASSIGN_TO(m_registryObserverCompPtr, m_registryTopologyGuiCompPtr, false);
		I_ASSIGN(m_registryValidationStatusCompPtr, "RegistryValidationStatus", "Visual status of registry validation", false, "RegistryValidationStatus");
		I_ASSIGN_TO(m_registryValidationStatusModelCompPtr, m_registryValidationStatusCompPtr, false);
	I_END_COMPONENT;

	CVisualRegistryScenographerComp();

	// font getters used by CRegistryElementShape
	const QFont& GetElementNameFont() const;
	const QFont& GetElementDetailFont() const;
	
	const icomp::IComponentEnvironmentManager* GetEnvironmentManager() const;
	const IRegistryConsistInfo* GetRegistryConsistInfo() const;

	/**
		Try to open the composite component's registry in a new window. 
		If the element is a composite component the function returns \c true, otherwise a \c false.
	*/
	bool TryOpenComponent(const CVisualRegistryElement& registryElement);

	/**
		Specify the flags that will be ignored by the update logic of the scene.
	 */
	void SetIgnoreChanges(int ignoreUpdateFlags);

	/**
		Set accepted mime types.
		For this types drag'n'drop functionality is enabled.
	 */
	void SetAcceptedMimeTypes(const QStringList& mimeTypes);

	// reimplemented (ibase::ICommandsProvider)
	virtual const ibase::IHierarchicalCommand* GetCommands() const;

	// reimplemented (iqtgui::IDropConsumer)
	virtual QStringList GetAcceptedMimeIds() const;
	virtual void OnDropFinished(const QMimeData& mimeData, QEvent* eventPtr);

	// reimplemented (imod::IObserver)
	virtual void AfterUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr);

protected:


	enum GroupId
	{
		GI_COMPONENT = 0x5430,
		GI_EMBEDDED_REGISTRY,
		GI_CODEGEN,
		GI_PREVIEW,
		GI_EDIT,
		GI_TOOLS
	};


	class EnvironmentObserver: public imod::TSingleModelObserverBase<icomp::IComponentEnvironmentManager>
	{
	public:
		EnvironmentObserver(CVisualRegistryScenographerComp* parentPtr);

	protected:
		// reimplemented (imod::TSingleModelObserverBase)
		virtual void OnUpdate(int updateFlags, istd::IPolymorphic* updateParamsPtr);

	private:
		CVisualRegistryScenographerComp& m_parent;
	};

	/**
		Get root or embedded registry that is currently selected in the editor.
	*/
	icomp::IRegistry* GetSelectedRegistry() const;

	/**
		Create instance of shape representing some element.
		The shape will be automatically connected to element using model/observer pattern.
		\sa imod.
	*/
	QGraphicsItem* AddShapeToScene(iser::ISerializable* elementPtr) const;
	void AddConnectorsToScene();
	void AddConnector(
				CRegistryElementShape& sourceShape,
				const QByteArray& referenceComponentId,
				const QByteArray& attributeId,
				bool isFactory = false);

	/**
		Used on drop/paste action
	*/
	icomp::IRegistryElement* TryCreateComponent(
				const QByteArray& elementId,
				const icomp::CComponentAddress& address,
				const i2d::CVector2d& position);

	/**
	 Used by TryCreateComponent() when an existing component is pasted (?) to 
	 update attributes referencing it, in all registry elements.
	 \param componentRole registry element id
	 */
	void ConnectReferences(const QByteArray& componentRole);

	/**
		Update component selection and related menu actions.
	*/
	void UpdateComponentSelection();

	void DoRetranslate();

	// reimplemented (icmpstr::TScenographerCompBase)
	virtual bool OnDropObject(const QMimeData& data, QGraphicsSceneDragDropEvent* eventPtr);
	virtual void UpdateScene(int updateFlags);

	// reimplemented (imod::CMultiModelDispatcherBase)
	virtual void OnModelChanged(int modelId, int changeFlags, istd::IPolymorphic* updateParamsPtr);

	// reimplemented (imod::CSingleModelObserverBase)
	virtual bool OnAttached(imod::IModel* modelPtr);
	virtual bool OnDetached(imod::IModel* modelPtr);

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

	// static methods
	static IElementSelectionInfo* ExtractSelectionInterface(CVisualRegistryScenographerComp& component);
	static imod::IModel* ExtractSelectionInterfaceModel(CVisualRegistryScenographerComp& component);
	static istd::IChangeable* ExtractSelectionInterfaceChangeable(CVisualRegistryScenographerComp& component);

	protected
Q_SLOTS:
	void OnSelectionChanged();

	void OnCutCommand();
	void OnCopyCommand();
	void OnPasteCommand();
	void OnRemoveComponent();
	void OnRenameComponent();
	void NewEmbeddedComponent();
	void ToEmbeddedComponent();
	void RemoveEmbeddedComponent();
	void OnExportToCode();
	void OnExecute();
	void OnAbort();
	void OnExecutionTimerTick();
	void OnShowRegistryTopology();

	/**
	 Selects (checks) a button representing an embedded composition
	 */
	void OnEmbeddedComponentButtonClicked();
	/**
	 Clear and fill the embedded registry button box (used when the number of registry elements changes)
	 */
	void UpdateEmbeddedRegistryButtons();
	/** 
	 Update the scene to display currently selected embedded composition
	*/
	void UpdateEmbeddedRegistryView(const QByteArray& id);

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();
	virtual void OnGuiDestroyed();

private:
	class SelectionInfoImpl: virtual public IElementSelectionInfo
	{
	public:
		void SetParent(CVisualRegistryScenographerComp* parentPtr);

		// reimplemented (icmpstr::IElementSelectionInfo)
		virtual icomp::IRegistry* GetSelectedRegistry() const;
		virtual Elements GetSelectedElements() const;

	private:
		CVisualRegistryScenographerComp* m_parentPtr;
	};

	friend class imod::TModelWrap<SelectionInfoImpl>;

	I_REF(iser::IFileLoader, m_registryCodeSaverCompPtr);
	I_REF(IRegistryPreview, m_registryPreviewCompPtr);
	I_REF(icomp::IComponentEnvironmentManager, m_envManagerCompPtr);
	I_REF(imod::IModel, m_envManagerModelCompPtr);
	I_REF(idoc::IHelpViewer, m_quickHelpViewerCompPtr);
	I_REF(idoc::IDocumentManager, m_documentManagerCompPtr);
	I_REF(IRegistryConsistInfo, m_consistencyInfoCompPtr);
	I_REF(iqtgui::IGuiObject, m_registryTopologyGuiCompPtr);
	I_REF(imod::IObserver, m_registryObserverCompPtr);
	I_REF(iqtgui::IVisualStatusProvider, m_registryValidationStatusCompPtr);
	I_REF(imod::IModel, m_registryValidationStatusModelCompPtr);
	I_REF(ISceneProvider, m_sceneProviderCompPtr);
	I_REF(iqtgui::IGuiObject, m_sceneProviderGuiCompPtr);
	
	iqtgui::CHierarchicalCommand m_registryCommand;
	iqtgui::CHierarchicalCommand m_editMenu;
	iqtgui::CHierarchicalCommand m_cutCommand;
	iqtgui::CHierarchicalCommand m_copyCommand;
	iqtgui::CHierarchicalCommand m_pasteCommand;
	iqtgui::CHierarchicalCommand m_removeComponentCommand;
	iqtgui::CHierarchicalCommand m_registryMenu;
	iqtgui::CHierarchicalCommand m_renameComponentCommand;
	iqtgui::CHierarchicalCommand m_newEmbeddedRegistryCommand;
	iqtgui::CHierarchicalCommand m_toEmbeddedRegistryCommand;
	iqtgui::CHierarchicalCommand m_removeEmbeddedRegistryCommand;
	iqtgui::CHierarchicalCommand m_exportToCodeCommand;
	iqtgui::CHierarchicalCommand m_executeRegistryCommand;
	iqtgui::CHierarchicalCommand m_abortRegistryCommand;
	iqtgui::CHierarchicalCommand m_showRegistryTopologyCommand;

	QTimer m_executionObserverTimer;

	QFont m_elementNameFont;
	QFont m_elementDetailFont;

	typedef QSet<QByteArray> ElementIds;
	ElementIds m_selectedElementIds;

	EnvironmentObserver m_environmentObserver;

	imod::TModelWrap<SelectionInfoImpl> m_selectionInfo;

	QByteArray m_embeddedRegistryId;
	int m_ignoreUpdateFlags;
	QGraphicsScene* m_scenePtr;
	bool m_isUpdating;
	QStringList m_acceptedMimeTypes;

	istd::TPointerVector<QPushButton> m_embeddedButtons;

	// static attributes
	static iser::CArchiveTag s_elementsListTag;
	static iser::CArchiveTag s_elementTag;
	static iser::CArchiveTag s_elementIdTag;
	static iser::CArchiveTag s_elementAddressTag;
	static iser::CArchiveTag s_elementCenterTag;
};


// inline methods


inline const icomp::IComponentEnvironmentManager* CVisualRegistryScenographerComp::GetEnvironmentManager() const
{
	return m_envManagerCompPtr.GetPtr();
}


inline const IRegistryConsistInfo* CVisualRegistryScenographerComp::GetRegistryConsistInfo() const
{
	return m_consistencyInfoCompPtr.GetPtr();
}


} // namespace icmpstr


#endif // !icmpstr_CVisualRegistryScenographerComp_included


