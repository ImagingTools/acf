#ifndef CPackageOverviewComp_included
#define CPackageOverviewComp_included


#include <QTreeWidget>
#include <QMimeData>
#include <QDrag>


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
	typedef iqtgui::TDesignerGuiCompBase<Ui::CPackageOverviewComp> BaseClass;
	
	I_BEGIN_COMPONENT(CPackageOverviewComp)
		I_REGISTER_INTERFACE(IAttributeSelectionObserver)
		I_ASSIGN(m_generalStaticInfoCompPtr, "StaticComponentInfo", "Static Component Info", true, "StaticComponentInfo")
		I_ASSIGN(m_packagesManagerCompPtr, "PackagesManager", "Packages manager used to provide icon paths", true, "PackagesManager")
	I_END_COMPONENT

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
	void on_PackagesList_itemCollapsed(QTreeWidgetItem* item);
	void on_PackagesList_itemExpanded(QTreeWidgetItem* item);
	void on_FilterEdit_textChanged(const QString& text);
	void on_ResetFilterButton_clicked();

protected:
	void GeneratePackageTree(
				const std::string& packageId,
				const icomp::IComponentStaticInfo& packageInfo,
				const QString& filter,
				QTreeWidgetItem& root);

	const icomp::IComponentStaticInfo* GetItemStaticInfo(const QTreeWidgetItem& item) const;

	// reimplemented (QObject)
	virtual bool eventFilter(QObject* eventObject, QEvent* event);

	// reimplemented (CGuiComponentBase)
	virtual void OnGuiCreated();

private:
	class PackageComponentItem: public QTreeWidgetItem
	{
	public:
		PackageComponentItem(QTreeWidgetItem* parentItemPtr, const icomp::CComponentAddress& address)
		:	QTreeWidgetItem(parentItemPtr), m_address(address)
		{
			setFlags(Qt::ItemIsDragEnabled | Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled);
			setText(0, iqt::GetQString(address.GetComponentId()));
		}

		const icomp::CComponentAddress& GetAddress() const
		{
			return m_address;
		}

	private:
		icomp::CComponentAddress m_address;
	};

	I_REF(icomp::IComponentStaticInfo, m_generalStaticInfoCompPtr);
	I_REF(icomp::IRegistriesManager, m_packagesManagerCompPtr);

	QIcon m_closedIcon;
	QIcon m_openIcon;
	QIcon m_validIcon;
	QIcon m_invalidIcon;
};


} // namespace icmpstr


#endif // !CPackageOverviewComp_included

