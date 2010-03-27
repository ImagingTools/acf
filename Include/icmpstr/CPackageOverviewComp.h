#ifndef icmpstr_CPackageOverviewComp_included
#define icmpstr_CPackageOverviewComp_included


// Qt includes
#include <QTreeWidget>
#include <QDir>


// ACF includes
#include "istd/TDelPtr.h"

#include "icomp/IComponentEnvironmentManager.h"
#include "icomp/CComponentAddress.h"

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

	bool IsInterfaceSupportedByComponent(const istd::CClassInfo& interfaceInfo, const QTreeWidgetItem& item) const;

	/**
		Create the drag preview for the component.
	*/
	QPixmap CreateComponentDragPixmap(const icomp::CComponentAddress &address) const;

	RootInfo& EnsureRoot(const std::string& path, int compType);

	// reimplemented (QObject)
	virtual bool eventFilter(QObject* sourcePtr, QEvent* eventPtr);

	// reimplemented (CGuiComponentBase)
	virtual void OnGuiCreated();

protected slots:
	void on_FilterEdit_editingFinished();
	void on_GroupByCB_currentIndexChanged(int index);
	void on_PackagesList_customContextMenuRequested(const QPoint& menuPoint);
	void on_FilterGB_toggled(bool on);
	void on_InterfaceCB_currentIndexChanged(int index);

private:
	class PackageComponentItem: public QTreeWidgetItem
	{
	public:
		PackageComponentItem(
					CPackageOverviewComp& parent,
					const icomp::CComponentAddress& address,
					const icomp::IComponentStaticInfo* staticInfoPtr,
					const QIcon& icon);

		bool IsInterfaceSupported(const istd::CClassInfo& interfaceInfo) const;

		const icomp::CComponentAddress& GetAddress() const
		{
			return m_address;
		}

	private:
		icomp::CComponentAddress m_address;
		CPackageOverviewComp& m_parent;
	};

	I_REF(icomp::IComponentEnvironmentManager, m_envManagerCompPtr);
	I_REF(IRegistryConsistInfo, m_consistInfoCompPtr);

	enum GruppingMode
	{
		GM_NONE,
		GM_PACKAGE,
		GM_KEYWORD,
		GM_NAME
	};

	typedef std::map<std::string, RootInfo> RootInfos;

	RootInfos m_roots;

	typedef std::set<istd::CClassInfo> InterfaceFilter;
	InterfaceFilter m_interfaceFilter;
	QStringList m_keywordsFilter;

	QIcon m_realComponentIcon;
	QIcon m_compositeComponentIcon;
	QIcon m_mixedComponentIcon;
};


} // namespace icmpstr


#endif // !icmpstr_CPackageOverviewComp_included

