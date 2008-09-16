#ifndef CPackageOverviewComp_included
#define CPackageOverviewComp_included


#include <QTreeWidget>
#include <QMimeData>
#include <QDrag>


#include "istd/TDelPtr.h"

#include "icomp/IComponentStaticInfo.h"
#include "icomp/CComponentAddress.h"

#include "iqt/TDesignerGuiCompBase.h"

#include "IAttributeSelectionObserver.h"

#include "Generated/ui_CPackageOverviewComp.h"


class CPackageOverviewComp:
			public iqt::TDesignerGuiCompBase<Ui::CPackageOverviewComp>,
			public IAttributeSelectionObserver
{
    Q_OBJECT

public:
	typedef iqt::TDesignerGuiCompBase<Ui::CPackageOverviewComp> BaseClass;
	
	I_BEGIN_COMPONENT(CPackageOverviewComp)
		I_REGISTER_INTERFACE(IAttributeSelectionObserver)
		I_ASSIGN(m_generalStaticInfoPtr, "StaticComponentInfo", "Static Component Info", true, "StaticComponentInfo")
	I_END_COMPONENT

	enum
	{
		ComponentStaticInfo = Qt::UserRole
	};

    CPackageOverviewComp();

public:
	// reimplemented (IAttributeSelectionObserver)
	virtual void OnAttributeSelected(const icomp::IAttributeStaticInfo* attributeStaticInfoPtr);

protected:
	void GenerateComponentTree(const QString& filter = "");
 	void HighlightComponents(const QString& interfaceId);

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

	I_REF(icomp::IComponentStaticInfo, m_generalStaticInfoPtr);

	static QString s_closedIcon;
	static QString s_openIcon;
};


#endif // !CPackageOverviewComp_included

