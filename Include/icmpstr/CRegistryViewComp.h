#ifndef icmpstr_CRegistryViewComp_included
#define icmpstr_CRegistryViewComp_included


// Qt includes
#include <QTimer>


// ACF includes
#include "iser/IFileLoader.h"

#include "imod/TSingleModelObserverBase.h"

#include "icomp/IComponentStaticInfo.h"
#include "icomp/IRegistryElement.h"
#include "icomp/CRegistry.h"

#include "ibase/ICommandsProvider.h"
#include "idoc/IHelpViewer.h"
#include "idoc/IMainWindowCommands.h"

#include "iqtgui/TGuiComponentBase.h"
#include "iqtgui/TGuiObserverWrap.h"
#include "iqtgui/CHierarchicalCommand.h"

#include "iqtdoc/IPrintable.h"

#include "icmpstr/IRegistryPreview.h"
#include "icmpstr/IElementSelectionInfo.h"
#include "icmpstr/IRegistryConsistInfo.h"
#include "icmpstr/CRegistryView.h"


namespace icmpstr
{


class CRegistryViewComp:
			public iqtgui::TGuiObserverWrap<
						iqtgui::TGuiComponentBase<CRegistryView>, 
						imod::TSingleModelObserverBase<icomp::IRegistry> >,
			virtual public imod::TModelWrap<IElementSelectionInfo>,
			virtual public iqtdoc::IPrintable,
			virtual public ibase::ICommandsProvider
{
	Q_OBJECT

public:
	typedef iqtgui::TGuiObserverWrap<
				iqtgui::TGuiComponentBase<CRegistryView>, 
				imod::TSingleModelObserverBase<icomp::IRegistry> > BaseClass;

	I_BEGIN_COMPONENT(CRegistryViewComp);
		I_REGISTER_INTERFACE(ibase::ICommandsProvider);
		I_REGISTER_INTERFACE(iqtdoc::IPrintable);
		I_REGISTER_INTERFACE(imod::IObserver);
		I_ASSIGN_MULTI_0(m_registryElementObserversCompPtr, "RegistryElementObservers", "Registry element observers", false);
		I_ASSIGN(m_envManagerCompPtr, "MetaInfoManager", "Allows access to component meta information", true, "MetaInfoManager");
		I_ASSIGN(m_registryCodeSaverCompPtr, "RegistryCodeSaver", "Export registry to C++ code file", false, "RegistryCodeSaver");
		I_ASSIGN(m_registryPreviewCompPtr, "RegistryPreview", "Executes preview of the registry", false, "RegistryPreview");
		I_ASSIGN(m_quickHelpViewerCompPtr, "QuickHelpViewer", "Show help of selected component using its address", false, "HelpViewer");
		I_ASSIGN(m_mainWindowCompPtr, "MainWindow", "Access to main window command", false, "MainWindow");
		I_ASSIGN(m_consistInfoCompPtr, "ConsistencyInfo", "Allows to check consistency of registries and attributes", false, "ConsistencyInfo");
	I_END_COMPONENT;

	CRegistryViewComp();

	bool TryCreateComponent(const icomp::CComponentAddress& address, const i2d::CVector2d& position);

	// reimplemented (iqtdoc::IPrintable)
	virtual void Print(QPrinter* printerPtr) const;

	// reimplemented (ibase::ICommandsProvider)
	virtual const ibase::IHierarchicalCommand* GetCommands() const;

	// reimplemented (icmpstr::IElementSelectionInfo)
	virtual icomp::IRegistry* GetSelectedRegistry() const;
	virtual iser::ISerializable* GetSelectedElement() const;
	virtual const std::string& GetSelectedElementName() const;
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
	I_REF(icomp::IComponentEnvironmentManager, m_envManagerCompPtr);
	I_REF(iser::IFileLoader, m_registryCodeSaverCompPtr);
	I_REF(IRegistryPreview, m_registryPreviewCompPtr);
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

	QIcon m_selectedIcon;

	QTimer m_executionObserverTimer;
};


} // namespace icmpstr


#endif // !icmpstr_CRegistryViewComp_included


