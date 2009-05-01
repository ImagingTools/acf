#ifndef CRegistryViewComp_included
#define CRegistryViewComp_included


// Qt includes
#include <QTimer>


// ACF includes
#include "iser/IFileLoader.h"

#include "imod/TSingleModelObserverBase.h"

#include "icomp/IComponentStaticInfo.h"
#include "icomp/IRegistryElement.h"
#include "icomp/CRegistry.h"

#include "idoc/ICommandsProvider.h"
#include "idoc/IHelpViewer.h"

#include "iqtgui/TGuiComponentBase.h"
#include "iqtgui/TGuiObserverWrap.h"
#include "iqtgui/CHierarchicalCommand.h"

#include "icmpstr/IRegistryPreview.h"
#include "icmpstr/CRegistryView.h"


namespace icmpstr
{


class CRegistryViewComp:
			public iqtgui::TGuiObserverWrap<
						iqtgui::TGuiComponentBase<CRegistryView>, 
						imod::TSingleModelObserverBase<icomp::IRegistry> >,
			virtual public imod::TModelWrap<IElementSelectionInfo>,
			virtual public idoc::ICommandsProvider
{
	Q_OBJECT

public:
	typedef iqtgui::TGuiObserverWrap<
				iqtgui::TGuiComponentBase<CRegistryView>, 
				imod::TSingleModelObserverBase<icomp::IRegistry> > BaseClass;

	I_BEGIN_COMPONENT(CRegistryViewComp);
		I_REGISTER_INTERFACE(idoc::ICommandsProvider);
		I_REGISTER_INTERFACE(imod::IObserver);
		I_ASSIGN_MULTI_0(m_registryElementObserversCompPtr, "RegistryElementObservers", "Registry element observers", false);
		I_ASSIGN(m_registryCodeSaverCompPtr, "RegistryCodeSaver", "Export registry to C++ code file", false, "RegistryCodeSaver");
		I_ASSIGN(m_registryPreviewCompPtr, "RegistryPreview", "Executes preview of the registry", false, "RegistryPreview");
		I_ASSIGN(m_packagesManagerCompPtr, "PackagesManager", "Packages manager used to provide icon paths", true, "PackagesManager");
		I_ASSIGN(m_quickHelpViewerCompPtr, "QuickHelpViewer", "Show help of selected component using its address", false, "HelpViewer");
	I_END_COMPONENT;

	CRegistryViewComp();

	bool TryCreateComponent(const icomp::CComponentAddress& address, const i2d::CVector2d& position);

	// reimplemented (idoc::ICommandsProvider)
	virtual const idoc::IHierarchicalCommand* GetCommands() const;

	// reimplemented (icmpstr::IElementSelectionInfo)
	virtual icomp::IRegistry* GetSelectedRegistry() const;
	virtual iser::ISerializable* GetSelectedElement() const;
	virtual const std::string& GetSelectedElementName() const;
	virtual QIcon GetSelectedElementIcon() const;
	virtual const icomp::CComponentAddress* GetSelectedElementAddress() const;

	// reimplemented (imod::IModelEditor)
	virtual void UpdateEditor(int updateFlags = 0);
	virtual void UpdateModel() const;

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();
	virtual void OnGuiDestroyed();
	virtual void OnRetranslate();

	// reimplemented (icomp::IComponent)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

protected slots:
	void OnComponentViewSelected(CComponentSceneItem* view, bool selected);
	void OnComponentPositionChanged(CComponentSceneItem* view, const QPointF& newPosition);
	void OnRemoveComponent();
	void OnRenameComponent();
	void OnProperties();
	void OnExportToCode();
	void OnExecute();
	void OnAbort();
	void OnAddNote();
	void OnRemoveNote();
	bool ProcessDroppedData(const QMimeData& data, QGraphicsSceneDragDropEvent* eventPtr);
	void UpdateConnectors();
	void OnExecutionTimerTick();

protected:
	enum GroupId
	{
		GI_COMPONENT = 0x5430,
		GI_CODEGEN,
		GI_PREVIEW
	};

	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void OnGuiModelAttached();
	virtual void OnGuiModelDetached();

private:
	void ConnectReferences(const QString& componentRole);

	I_MULTIREF(imod::IObserver, m_registryElementObserversCompPtr);
	I_REF(iser::IFileLoader, m_registryCodeSaverCompPtr);
	I_REF(IRegistryPreview, m_registryPreviewCompPtr);
	I_REF(icomp::IRegistriesManager, m_packagesManagerCompPtr);
	I_REF(idoc::IHelpViewer, m_quickHelpViewerCompPtr);

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
};


} // namespace icmpstr


#endif // !CRegistryViewComp_included


