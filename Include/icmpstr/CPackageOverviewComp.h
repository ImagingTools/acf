#ifndef icmpstr_CPackageOverviewComp_included
#define icmpstr_CPackageOverviewComp_included


// Qt includes
#include <QtCore/QDir>
#include <QtGui/QTreeWidget>

// ACF includes
#include "istd/TDelPtr.h"

#include "icomp/IComponentEnvironmentManager.h"
#include "icomp/CComponentAddress.h"

#include "ibase/ICommandsProvider.h"

#include "ifile/IFileNameParam.h"

#include "idoc/IHelpViewer.h"
#include "idoc/IDocumentManager.h"

#include "iqtgui/TDesignerGuiObserverCompBase.h"
#include "iqtgui/CHierarchicalCommand.h"

#include "icmpstr/IRegistryConsistInfo.h"
#include "icmpstr/IAttributeSelectionObserver.h"
#include "icmpstr/IElementSelectionInfo.h"

#include "Generated/ui_CPackageOverviewComp.h"


namespace icmpstr
{


class CPackageOverviewComp:
			public iqtgui::TDesignerGuiObserverCompBase<Ui::CPackageOverviewComp, IElementSelectionInfo>,
			virtual public ibase::ICommandsProvider,
			virtual public IAttributeSelectionObserver
{
	Q_OBJECT

public:
	typedef iqtgui::TDesignerGuiObserverCompBase<Ui::CPackageOverviewComp, IElementSelectionInfo> BaseClass;

	I_BEGIN_COMPONENT(CPackageOverviewComp);
		I_REGISTER_INTERFACE(ibase::ICommandsProvider);
		I_REGISTER_INTERFACE(IAttributeSelectionObserver);
		I_ASSIGN(m_envManagerCompPtr, "EnvironmentManager", "Packages manager used to provide icon paths", true, "PackagesManager");
		I_ASSIGN(m_consistInfoCompPtr, "ConsistencyInfo", "Allows to check consistency of registries and access to buffered icons", false, "ConsistencyInfo");
		I_ASSIGN(m_quickHelpViewerCompPtr, "QuickHelpGui", "Show help of selected component using its address", false, "HelpViewer");
		I_ASSIGN(m_documentManagerCompPtr, "DocumentManager", "Document manager allowing to load files on double click", false, "DocumentManager");
		I_ASSIGN(m_configFilePathCompPtr, "ConfigFilePath", "Path of packages configuration file will be loaded, if enabled", false, "ConfigFilePath");
		I_ASSIGN_TO(m_configFilePathModelCompPtr, m_configFilePathCompPtr, false);
	I_END_COMPONENT;

	CPackageOverviewComp();

	// reimplemented (ibase::ICommandsProvider)
	virtual const ibase::IHierarchicalCommand* GetCommands() const;

	// reimplemented (icmpstr::IAttributeSelectionObserver)
	virtual void OnAttributeSelected(const icomp::IAttributeStaticInfo* attributeStaticInfoPtr);

protected:
	struct RootInfo
	{
		RootInfo(): itemPtr(NULL), hasRealComponents(false), hasCompositeComponents(false){}

		QTreeWidgetItem* itemPtr;
		bool hasRealComponents;
		bool hasCompositeComponents;
	};

	void GenerateComponentTree(bool forceUpdate);
	void UpdateComponentGroups();
	void UpdateInterfaceList();
	void UpdateAllLists();

	/**
		Get a list of components that match the filter criteria.
	*/
	icomp::IMetaInfoManager::ComponentAddresses GetFilteredComponentAdresses() const;

	/**
		Create the drag preview for the component.
	*/
	QPixmap CreateComponentDragPixmap(const icomp::CComponentAddress &address) const;

	RootInfo& EnsureRoot(const QByteArray& path, const icomp::CComponentAddress& address, const icomp::IComponentStaticInfo* staticInfoPtr);

	void SetCommandNames();

	// reimplemented (QObject)
	virtual bool eventFilter(QObject* sourcePtr, QEvent* eventPtr);

	// reimplemented (iqtgui::TGuiObserverWrap)
	virtual void UpdateGui(int updateFlags = 0);

	// reimplemented (iqtgui::CGuiComponentBase)
	virtual void OnGuiCreated();
	virtual void OnGuiDestroyed();
	virtual void OnRetranslate();
	virtual void OnGuiRetranslate();

protected Q_SLOTS:
	void on_FilterEdit_editingFinished();
	void on_GroupByCB_currentIndexChanged(int index);
	void on_PackagesList_itemSelectionChanged();
	void on_PackagesList_itemExpanded(QTreeWidgetItem* item);
	void on_PackagesList_itemClicked(QTreeWidgetItem* item, int column);
	void on_PackagesList_itemDoubleClicked(QTreeWidgetItem* item, int column);
	void on_FilterGB_toggled(bool on);
	void on_InterfaceCB_currentIndexChanged(int index);
	void OnReloadPackages();
	void on_KeywordClearButton_clicked();
	void on_InterfaceClearButton_clicked();

private:
	class PackageItemBase: public QTreeWidgetItem
	{
	public:
		PackageItemBase(
					CPackageOverviewComp& parent,
					const QString& description,
					const QIcon& icon = QIcon());

		virtual const QString& GetDescription() const;

	protected:
		CPackageOverviewComp& m_parent;
		QString m_description;
	};

	class PackageComponentItem: public PackageItemBase
	{
	public:
		typedef PackageItemBase BaseClass;

		PackageComponentItem(
					CPackageOverviewComp& parent,
					const icomp::CComponentAddress& address,
					const icomp::IComponentStaticInfo* staticInfoPtr,
					const QIcon& icon);

		bool IsInterfaceSupported(const istd::CClassInfo& interfaceInfo) const;
		const icomp::CComponentAddress& GetAddress() const;

	private:
		icomp::CComponentAddress m_address;
	};

	class PackageItem: public PackageItemBase
	{
	public:
		typedef PackageItemBase BaseClass;

		PackageItem(
					CPackageOverviewComp& parent,
					const QString& description,
					const QIcon& icon = QIcon());
	};

	class RegistryObserver: public imod::CSingleModelObserverBase
	{
	public:
		RegistryObserver(CPackageOverviewComp* parentPtr);

	protected:
		// reimplemented (imod::CSingleModelObserverBase)
		virtual void OnUpdate(int updateFlags, istd::IPolymorphic* updateParamsPtr);

	private:
		CPackageOverviewComp& m_parent;
	};

	class ConfigObserver: public imod::CSingleModelObserverBase
	{
	public:
		ConfigObserver(CPackageOverviewComp* parentPtr);

	protected:
		// reimplemented (imod::CSingleModelObserverBase)
		virtual void OnUpdate(int updateFlags, istd::IPolymorphic* updateParamsPtr);

	private:
		CPackageOverviewComp& m_parent;
	};

	enum GruppingMode
	{
		GM_NONE,
		GM_PACKAGE
	};

	I_REF(icomp::IComponentEnvironmentManager, m_envManagerCompPtr);
	I_REF(IRegistryConsistInfo, m_consistInfoCompPtr);
	I_REF(idoc::IHelpViewer, m_quickHelpViewerCompPtr);
	I_REF(idoc::IDocumentManager, m_documentManagerCompPtr);
	I_REF(ifile::IFileNameParam, m_configFilePathCompPtr);
	I_REF(imod::IModel, m_configFilePathModelCompPtr);

	iqtgui::CHierarchicalCommand m_commands;
	iqtgui::CHierarchicalCommand m_packagesCommand;
	iqtgui::CHierarchicalCommand m_reloadCommand;

	typedef QMap<QByteArray, RootInfo> RootInfos;
	RootInfos m_roots;

	RegistryObserver m_registryObserver;
	ConfigObserver m_configObserver;

	typedef QSet<QByteArray> InterfaceFilter;
	InterfaceFilter m_interfaceFilter;
	QStringList m_keywordsFilter;

	QIcon m_realComponentIcon;
	QIcon m_compositeComponentIcon;
	QIcon m_mixedComponentIcon;
	QIcon m_embeddedComponentIcon;

	typedef QMap<QString, QTreeWidget*> CategoryWidgetsMap;
	CategoryWidgetsMap m_categoryWidgetsMap;

	icomp::IMetaInfoManager::ComponentAddresses m_shownAddresses;

	int m_currentPackageGroupIndex;
	bool m_startDrag;
	QPoint m_starDragPosition;

	friend class CItemDelegate;
};


} // namespace icmpstr


#endif // !icmpstr_CPackageOverviewComp_included

