#ifndef icmpstr_CPackageOverviewComp_included
#define icmpstr_CPackageOverviewComp_included


// Qt includes
#include <QTreeWidget>
#include <QDir>


// ACF includes
#include "istd/TDelPtr.h"

#include "icomp/IComponentEnvironmentManager.h"
#include "icomp/CComponentAddress.h"

#include "idoc/IHelpViewer.h"

#include "iqtgui/TDesignerGuiCompBase.h"

#include "icmpstr/IRegistryConsistInfo.h"
#include "icmpstr/IAttributeSelectionObserver.h"

#include "Generated/ui_CPackageOverviewComp.h"


namespace icmpstr
{


class CPackageOverviewComp:
			public iqtgui::TDesignerGuiCompBase<Ui::CPackageOverviewComp>,
			public IAttributeSelectionObserver
{
    Q_OBJECT

public:
	typedef iqtgui::TDesignerGuiCompBase<Ui::CPackageOverviewComp> BaseClass;
	
	I_BEGIN_COMPONENT(CPackageOverviewComp);
		I_REGISTER_INTERFACE(IAttributeSelectionObserver);
		I_ASSIGN(m_envManagerCompPtr, "EnvironmentManager", "Packages manager used to provide icon paths", true, "PackagesManager");
		I_ASSIGN(m_consistInfoCompPtr, "ConsistencyInfo", "Allows to check consistency of registries and access to buffred icons", false, "ConsistencyInfo");
		I_ASSIGN(m_quickHelpViewerCompPtr, "QuickHelpViewer", "Show help of selected component using its address", false, "HelpViewer");
	I_END_COMPONENT;

	enum
	{
		ComponentStaticInfo = Qt::UserRole
	};

public:
	// reimplemented (IAttributeSelectionObserver)
	virtual void OnAttributeSelected(const icomp::IAttributeStaticInfo* attributeStaticInfoPtr);

protected:
	struct RootInfo
	{
		RootInfo(): itemPtr(NULL), hasRealComponents(false), hasCompositeComponents(false){}

		QTreeWidgetItem* itemPtr;
		bool hasRealComponents;
		bool hasCompositeComponents;
	};

	void GenerateComponentTree();
	void UpdateComponentsView();
	void UpdateComponentGroups();

	/**
		Get component list, that match to the filter criteria.
	*/
	icomp::IMetaInfoManager::ComponentAddresses GetFilteredComponentAdresses() const;

	bool IsInterfaceSupportedByComponent(const istd::CClassInfo& interfaceInfo, const QTreeWidgetItem& item) const;

	/**
		Create the drag preview for the component.
	*/
	QPixmap CreateComponentDragPixmap(const icomp::CComponentAddress &address) const;

	RootInfo& EnsureRoot(const std::string& path, const icomp::CComponentAddress& address, const icomp::IComponentStaticInfo* staticInfoPtr);

	// reimplemented (QObject)
	virtual bool eventFilter(QObject* sourcePtr, QEvent* eventPtr);

	// reimplemented (CGuiComponentBase)
	virtual void OnGuiCreated();

protected slots:
	void on_FilterEdit_editingFinished();
	void on_GroupByCB_currentIndexChanged(int index);
	void on_PackagesList_itemSelectionChanged();
	void on_PackagesList_itemExpanded(QTreeWidgetItem* item);
	void on_PackagesList_itemClicked(QTreeWidgetItem* item, int column);
	void on_FilterGB_toggled(bool on);
	void on_InterfaceCB_currentIndexChanged(int index);
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
		QString m_description;
		CPackageOverviewComp& m_parent;
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

	enum GruppingMode
	{
		GM_NONE,
		GM_PACKAGE
	};

	I_REF(icomp::IComponentEnvironmentManager, m_envManagerCompPtr);
	I_REF(IRegistryConsistInfo, m_consistInfoCompPtr);
	I_REF(idoc::IHelpViewer, m_quickHelpViewerCompPtr);

	typedef std::map<std::string, RootInfo> RootInfos;
	RootInfos m_roots;

	typedef std::set<istd::CClassInfo> InterfaceFilter;
	InterfaceFilter m_interfaceFilter;
	QStringList m_keywordsFilter;

	QIcon m_realComponentIcon;
	QIcon m_compositeComponentIcon;
	QIcon m_mixedComponentIcon;

	typedef std::map<istd::CString, QTreeWidget*> CategoryWidgetsMap;
	CategoryWidgetsMap m_categoryWidgetsMap;

	friend class CItemDelegate;
};


} // namespace icmpstr


#endif // !icmpstr_CPackageOverviewComp_included

