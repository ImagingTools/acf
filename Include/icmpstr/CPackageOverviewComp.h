#ifndef icmpstr_CPackageOverviewComp_included
#define icmpstr_CPackageOverviewComp_included


// Qt includes
#include <QTreeWidget>
#include <QMimeData>
#include <QDrag>
#include <QDir>


// ACF includes
#include "istd/TDelPtr.h"

#include "icomp/IComponentStaticInfo.h"
#include "icomp/IRegistriesManager.h"
#include "icomp/CComponentAddress.h"

#include "iqtgui/TDesignerGuiCompBase.h"

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
	enum
	{
		KeywordColumn = 0
	};

	typedef iqtgui::TDesignerGuiCompBase<Ui::CPackageOverviewComp> BaseClass;
	
	I_BEGIN_COMPONENT(CPackageOverviewComp);
		I_REGISTER_INTERFACE(IAttributeSelectionObserver);
		I_ASSIGN(m_generalStaticInfoCompPtr, "StaticComponentInfo", "Static Component Info", true, "StaticComponentInfo");
		I_ASSIGN(m_packagesManagerCompPtr, "PackagesManager", "Packages manager used to provide icon paths", true, "PackagesManager");
	I_END_COMPONENT;

	enum
	{
		ComponentStaticInfo = Qt::UserRole
	};

public:
	// reimplemented (IAttributeSelectionObserver)
	virtual void OnAttributeSelected(const icomp::IAttributeStaticInfo* attributeStaticInfoPtr);

protected:
	void GenerateComponentTree(const QString& filter = "", bool expandComponents = false);
	void HighlightComponents(const istd::CClassInfo& interfaceInfo = istd::CClassInfo());

protected slots:
	void on_FilterEdit_textEdited(const QString& text);
	void on_ResetFilterButton_clicked();

	void OnContextMenuRequested(const QPoint& menuPoint);

protected:
	/**
		Generate tree items for some package.
	*/
	void GeneratePackageTree(
				const std::string& packageId,
				const icomp::IComponentStaticInfo& packageInfo,
				const QString& filter,
				QTreeWidgetItem& root);
	const icomp::IComponentStaticInfo* GetItemStaticInfo(const QTreeWidgetItem& item) const;
	bool IsInterfaceSupportedByComponent(const istd::CClassInfo& interfaceInfo, const QTreeWidgetItem& item) const;

	/**
		Create the drag preview for the component.
	*/
	QPixmap CreateComponentDragPixmap(const icomp::CComponentAddress &address) const;

	/**
		Get icon for a given component.
	*/
	QIcon GetComponentIcon(const icomp::CComponentAddress& componentAddress)const;

	QIcon GetIconFromPath(const QString& iconPath) const;

	// reimplemented (QObject)
	virtual bool eventFilter(QObject* sourcePtr, QEvent* eventPtr);

	// reimplemented (CGuiComponentBase)
	virtual void OnGuiCreated();

private:
	class PackageComponentItem: public QTreeWidgetItem
	{
	public:
		PackageComponentItem(
					CPackageOverviewComp& parent,
					QTreeWidgetItem* parentItemPtr,
					const icomp::CComponentAddress& address,
					const icomp::IComponentStaticInfo& staticInfo,
					const QDir* packageDirPtr);

		bool IsInterfaceSupported(const istd::CClassInfo& interfaceInfo) const;

		const icomp::CComponentAddress& GetAddress() const
		{
			return m_address;
		}

		const icomp::IComponentStaticInfo& GetStaticInfo() const
		{
			return m_staticInfo;
		}

	private:
		icomp::CComponentAddress m_address;
		const icomp::IComponentStaticInfo& m_staticInfo;
		CPackageOverviewComp& m_parent;
	};

	I_REF(icomp::IComponentStaticInfo, m_generalStaticInfoCompPtr);
	I_REF(icomp::IRegistriesManager, m_packagesManagerCompPtr);

	QIcon m_validIcon;
	QIcon m_invalidIcon;
	QIcon m_realComponentIcon;
	QIcon m_compositeComponentIcon;
	QIcon m_mixedComponentIcon;

	typedef QHash<QString, QIcon> IconCache;
	mutable IconCache m_iconCache;
};


} // namespace icmpstr


#endif // !icmpstr_CPackageOverviewComp_included

