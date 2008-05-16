#ifndef CPackageOverviewComp_included
#define CPackageOverviewComp_included


#include <QTreeWidget>
#include <QMimeData>
#include <QDrag>


#include "iqt/TGuiComponentBase.h"

#include "icomp/IRegistry.h"

#include "CStaticComponentInfo.h"


class CPackageOverviewComp: public iqt::TGuiComponentBase<QTreeWidget>
{
    Q_OBJECT

public:
	typedef iqt::TGuiComponentBase<QTreeWidget> BaseClass;
	
	I_BEGIN_COMPONENT(CPackageOverviewComp)
		I_ASSIGN(m_generalStaticInfoPtr, "StaticComponentInfo", "Static Component Info", true, "StaticComponentInfo")
	I_END_COMPONENT

	enum
	{
		ComponentStaticInfo = Qt::UserRole
	};

    CPackageOverviewComp();

public slots:
	void GenerateComponentTree();
 	void HighlightComponents(const QString& interfaceId);

protected slots:
	void OnItemSelected();
	void OnItemCollapsed(QTreeWidgetItem* item);
	void OnItemExpanded(QTreeWidgetItem* item);

protected:
	void AddPackage(const icomp::IComponentStaticInfo* staticInfoPtr, const QString& name);

	// reimplemented (QObject)
	virtual bool eventFilter(QObject* eventObject, QEvent* event);

	// reimplemented (CGuiComponentBase)
	virtual void OnGuiCreated();

private:
	class PackageComponentItem: public QTreeWidgetItem
	{
	public:
		PackageComponentItem()
		{
		}

		PackageComponentItem(QTreeWidgetItem* parentItemPtr)
			:QTreeWidgetItem(parentItemPtr)
		{
		}
		
		istd::TDelPtr<CStaticComponentInfo> m_componentInfoPtr;
	};

	I_REF(icomp::IComponentStaticInfo, m_generalStaticInfoPtr);

	QTreeWidgetItem* m_rootLocalItem;
	QTreeWidgetItem* m_rootComposedItem;
	CStaticComponentInfo* m_selectedComponentInfoPtr;

	static QString s_closedIcon;
	static QString s_openIcon;
};


#endif // !CPackageOverviewComp_included

