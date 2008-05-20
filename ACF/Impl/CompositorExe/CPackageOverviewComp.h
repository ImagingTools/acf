#ifndef CPackageOverviewComp_included
#define CPackageOverviewComp_included


#include <QTreeWidget>
#include <QMimeData>
#include <QDrag>


#include "iqt/TGuiComponentBase.h"

#include "icomp/IRegistry.h"
#include "icomp/CPackageStaticInfo.h"

#include "IAttributeSelectionObserver.h"


class CPackageOverviewComp: public iqt::TGuiComponentBase<QTreeWidget>,
							public IAttributeSelectionObserver
{
    Q_OBJECT

public:
	typedef iqt::TGuiComponentBase<QTreeWidget> BaseClass;
	
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
	void GenerateComponentTree();
 	void HighlightComponents(const QString& interfaceId);

protected slots:
	void OnItemCollapsed(QTreeWidgetItem* item);
	void OnItemExpanded(QTreeWidgetItem* item);

protected:
	void GeneratePackageTree(
				const std::string& packageId,
				const icomp::CPackageStaticInfo& packageInfo,
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

	I_REF(icomp::CPackageStaticInfo, m_generalStaticInfoPtr);

	QTreeWidgetItem* m_rootLocalItem;
	QTreeWidgetItem* m_rootComposedItem;

	static QString s_closedIcon;
	static QString s_openIcon;
};


#endif // !CPackageOverviewComp_included

