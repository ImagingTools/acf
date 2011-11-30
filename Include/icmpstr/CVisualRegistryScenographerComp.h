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
#include "iser/CArchiveTag.h"

#include "imod/TSingleModelObserverBase.h"
#include "imod/TModelWrap.h"

#include "icomp/IRegistry.h"
#include "icomp/IComponentEnvironmentManager.h"

#include "ibase/ICommandsProvider.h"

#include "idoc/IHelpViewer.h"
#include "idoc/IDocumentManager.h"

#include "iqtgui/TGuiComponentBase.h"
#include "iqtgui/TGuiObserverWrap.h"
#include "iqtgui/CHierarchicalCommand.h"

#include "iqt2d/TScenographerCompBase.h"

#include "icmpstr/IRegistryPreview.h"
#include "icmpstr/IRegistryConsistInfo.h"
#include "icmpstr/IElementSelectionInfo.h"


namespace icmpstr
{


class CRegistryElementShape;
class CVisualRegistryElement;
class CVisualRegistryComp;


class CVisualRegistryScenographerComp:
			public QObject,
			public iqt2d::TScenographerCompBase<
						imod::TSingleModelObserverBase<icomp::IRegistry> >,
			virtual public ibase::ICommandsProvider
{
	Q_OBJECT

public:
	typedef iqt2d::TScenographerCompBase<
				imod::TSingleModelObserverBase<icomp::IRegistry> > BaseClass;

	I_BEGIN_COMPONENT(CVisualRegistryScenographerComp);
		I_REGISTER_INTERFACE(ibase::ICommandsProvider);
		I_REGISTER_SUBELEMENT(SelectionInfo);
		I_REGISTER_SUBELEMENT_INTERFACE(SelectionInfo, IElementSelectionInfo, ExtractSelectionInterface);
		I_REGISTER_SUBELEMENT_INTERFACE(SelectionInfo, imod::IModel, ExtractSelectionInterfaceModel);
		I_REGISTER_SUBELEMENT_INTERFACE(SelectionInfo, istd::IChangeable, ExtractSelectionInterfaceChangeable);
		I_ASSIGN(m_registryCodeSaverCompPtr, "RegistryCodeSaver", "Export registry to C++ code file", false, "RegistryCodeSaver");
		I_ASSIGN(m_registryPreviewCompPtr, "RegistryPreview", "Executes preview of the registry", false, "RegistryPreview");
		I_ASSIGN(m_envManagerCompPtr, "MetaInfoManager", "Allows access to component meta information", true, "MetaInfoManager");
		I_ASSIGN_TO(m_envManagerModelCompPtr, m_envManagerCompPtr, false);
		I_ASSIGN(m_quickHelpViewerCompPtr, "QuickHelpGui", "Show help of selected component using its address", false, "QuickHelpGui");
		I_ASSIGN(m_documentManagerCompPtr, "DocumentManager", "Document manager allowing to load files on double click", false, "DocumentManager");
		I_ASSIGN(m_consistInfoCompPtr, "ConsistencyInfo", "Allows to check consistency of registries and attributes", false, "ConsistencyInfo");
	I_END_COMPONENT;

	CVisualRegistryScenographerComp();

	const QFont& GetElementNameFont() const;
	const QFont& GetElementDetailFont() const;
	
	const icomp::IComponentEnvironmentManager* GetEnvironmentManager() const;
	const IRegistryConsistInfo* GetRegistryConsistInfo() const;

	/**
		Try to open the composite component's registry in a new window. 
		If the element is a composite component the function returns \c true, otherwise a \c false.
	*/
	bool TryOpenComponent(const CVisualRegistryElement& registryElement) const;

	// reimplemented (ibase::ICommandsProvider)
	virtual const ibase::IHierarchicalCommand* GetCommands() const;

protected:
	enum GroupId
	{
		GI_COMPONENT = 0x5430,
		GI_EMBEDDED_REGISTRY,
		GI_CODEGEN,
		GI_PREVIEW,
		GI_EDIT
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
	icomp::IRegistryElement* TryCreateComponent(
				const std::string elementId,
				const icomp::CComponentAddress& address,
				const i2d::CVector2d& position);
	void ConnectReferences(const std::string& componentRole);
	void UpdateComponentSelection();

	void DoRetranslate();

	// reimplemented (iqt2d::TScenographerCompBase)
	virtual bool OnDropObject(const QMimeData& data, QGraphicsSceneDragDropEvent* eventPtr);
	virtual void UpdateScene(int updateFlags);

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

	// static methods
	static IElementSelectionInfo* ExtractSelectionInterface(CVisualRegistryScenographerComp& component);
	static imod::IModel* ExtractSelectionInterfaceModel(CVisualRegistryScenographerComp& component);
	static istd::IChangeable* ExtractSelectionInterfaceChangeable(CVisualRegistryScenographerComp& component);

protected Q_SLOTS:
	void OnSelectionChanged();

	void OnCutCommand();
	void OnCopyCommand();
	void OnPasteCommand();
	void OnRemoveComponent();
	void OnRenameComponent();
	void InsertEmbeddedComponent();
	void ToEmbeddedComponent();
	void OnExportToCode();
	void OnExecute();
	void OnAbort();
	void OnAddNote();
	void OnRemoveNote();
	void OnExecutionTimerTick();

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
	I_REF(IRegistryConsistInfo, m_consistInfoCompPtr);
	
	iqtgui::CHierarchicalCommand m_registryCommand;
	iqtgui::CHierarchicalCommand m_editMenu;
	iqtgui::CHierarchicalCommand m_cutCommand;
	iqtgui::CHierarchicalCommand m_copyCommand;
	iqtgui::CHierarchicalCommand m_pasteCommand;
	iqtgui::CHierarchicalCommand m_removeComponentCommand;
	iqtgui::CHierarchicalCommand m_registryMenu;
	iqtgui::CHierarchicalCommand m_renameComponentCommand;
	iqtgui::CHierarchicalCommand m_insertEmbeddedRegistryCommand;
	iqtgui::CHierarchicalCommand m_toEmbeddedRegistryCommand;
	iqtgui::CHierarchicalCommand m_exportToCodeCommand;
	iqtgui::CHierarchicalCommand m_executeRegistryCommand;
	iqtgui::CHierarchicalCommand m_abortRegistryCommand;
	iqtgui::CHierarchicalCommand m_addNoteCommand;
	iqtgui::CHierarchicalCommand m_removeNoteCommand;

	QTimer m_executionObserverTimer;

	QFont m_elementNameFont;
	QFont m_elementDetailFont;

	typedef std::set<std::string> ElementIds;
	ElementIds m_selectedElementIds;

	EnvironmentObserver m_environmentObserver;

	imod::TModelWrap<SelectionInfoImpl> m_selectionInfo;

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
	return m_consistInfoCompPtr.GetPtr();
}


} // namespace icmpstr


#endif // !icmpstr_CVisualRegistryScenographerComp_included


