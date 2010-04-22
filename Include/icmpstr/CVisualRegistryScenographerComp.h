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
			virtual public icmpstr::IElementSelectionInfo,
			virtual public ibase::ICommandsProvider
{
	Q_OBJECT

public:
	typedef iqt2d::TScenographerCompBase<
				imod::TSingleModelObserverBase<icomp::IRegistry> > BaseClass;

	I_BEGIN_COMPONENT(CVisualRegistryScenographerComp);
		I_REGISTER_INTERFACE(IElementSelectionInfo);
		I_REGISTER_INTERFACE(ibase::ICommandsProvider);
		I_ASSIGN(m_registryCodeSaverCompPtr, "RegistryCodeSaver", "Export registry to C++ code file", false, "RegistryCodeSaver");
		I_ASSIGN(m_registryPreviewCompPtr, "RegistryPreview", "Executes preview of the registry", false, "RegistryPreview");
		I_ASSIGN(m_envManagerCompPtr, "MetaInfoManager", "Allows access to component meta information", true, "MetaInfoManager");
		I_ASSIGN(m_quickHelpViewerCompPtr, "QuickHelpViewer", "Show help of selected component using its address", false, "HelpViewer");
		I_ASSIGN(m_mainWindowCompPtr, "MainWindow", "Access to main window command", false, "MainWindow");
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

	// reimplemented (icmpstr::IElementSelectionInfo)
	virtual icomp::IRegistry* GetSelectedRegistry() const;
	virtual iser::ISerializable* GetSelectedElement() const;
	virtual const std::string& GetSelectedElementName() const;
	virtual const icomp::CComponentAddress* GetSelectedElementAddress() const;

	// reimplemented (ibase::ICommandsProvider)
	virtual const ibase::IHierarchicalCommand* GetCommands() const;

	// reimplemented (icomp::IComponent)
	virtual void OnComponentCreated();

protected:
	void DoRetranslate();

	// reimplemented (iqt2d::TScenographerCompBase)
	virtual bool OnDropObject(const QMimeData& data, QGraphicsSceneDragDropEvent* eventPtr);
	virtual void UpdateScene(int updateFlags);

protected slots:
	void OnSelectionChanged();
	void OnRemoveComponent();
	void OnRenameComponent();
	void OnExportToCode();
	void OnExecute();
	void OnAbort();
	void OnAddNote();
	void OnRemoveNote();
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
	I_REF(IRegistryConsistInfo, m_consistInfoCompPtr);
	
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

	QFont m_elementNameFont;
	QFont m_elementDetailFont;

	std::string m_selectedElementId;
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


