#include "CPackageOverviewComp.h"


#include <QVBoxLayout>
#include <QMouseEvent>
#include <QApplication>
#include <QHeaderView>
#include <QItemDelegate>


#include "istd/CString.h"


class CItemDelegate: public QItemDelegate
{
public:
	virtual QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
	{
		QSize size = QItemDelegate::sizeHint(option, index);
		size.setHeight(20);

		return size;
	}
};


CPackageOverviewComp::CPackageOverviewComp()
	: BaseClass(),
	m_selectedComponentInfoPtr(NULL)
{
}


// public slots

void CPackageOverviewComp::GenerateComponentTree()
{
	if (!m_generalStaticInfoPtr.IsValid()){
		return;
	}

	icomp::IComponentStaticInfo::SubcomponentInfos packages = m_generalStaticInfoPtr->GetSubcomponentInfos();

	for(int packageIndex = 0; packageIndex < packages.GetLocalElementsCount(); packageIndex++){
		const std::string& packageName = packages.GetLocalKeyAt(packageIndex);
		const icomp::IComponentStaticInfo* staticInfoPtr = packages.GetValueAt(packageIndex);

		AddPackage(staticInfoPtr, packageName.c_str());
	}
}


void CPackageOverviewComp::HighlightComponents(const QString& interfaceId)
{
	QTreeWidget* widgetPtr = GetQtWidget();
	I_ASSERT(widgetPtr);

	if (widgetPtr->topLevelItemCount() == 0){
		return;
	}

	QTreeWidgetItemIterator treeIter(widgetPtr);
	while (*treeIter != NULL){
		(*treeIter)->setIcon(0, QIcon());

		int metaInfoData = (*treeIter)->data(0, ComponentStaticInfo).toInt();
		const icomp::IComponentStaticInfo* staticInfoPtr = reinterpret_cast<const icomp::IComponentStaticInfo*>(metaInfoData);

		QTreeWidgetItem* parentItem = (*treeIter)->parent();
		if (parentItem != NULL){
			if (parentItem->isExpanded()){
				parentItem->setIcon(0, QIcon(QString::fromUtf8(":/Resources/Icons/dirclosed-16.png")));
			}
			else{			
				parentItem->setIcon(0, QIcon(QString::fromUtf8(":/Resources/Icons/diropen-16.png")));
			}
		}

		if (staticInfoPtr != NULL && !interfaceId.isEmpty()){
			icomp::IComponentStaticInfo::InterfaceExtractors interfaceExtractors = staticInfoPtr->GetInterfaceExtractors();
			const icomp::IComponentStaticInfo::InterfaceExtractorPtr* extractorPtr = interfaceExtractors.FindElement(interfaceId.toStdString().c_str());
			if (extractorPtr != NULL){
				(*treeIter)->setIcon(0, QIcon(QString::fromUtf8(":/Resources/Icons/ok-16.png")));
				QTreeWidgetItem* parentItem = (*treeIter)->parent();
				if (parentItem != NULL){
					parentItem->setExpanded(true);
				}
				break;
			}
		}
		treeIter++;
	}
}


// protected slots

void CPackageOverviewComp::OnItemSelected()
{
	QTreeWidget* widgetPtr = GetQtWidget();
	I_ASSERT(widgetPtr);

	m_selectedComponentInfoPtr = NULL;

	QList<QTreeWidgetItem*> items = widgetPtr->selectedItems();
	if (items.count() != 0){
		PackageComponentItem* selectedItemPtr = dynamic_cast<PackageComponentItem*>(items.at(0));
		if (selectedItemPtr != NULL){
			m_selectedComponentInfoPtr = selectedItemPtr->m_componentInfoPtr.GetPtr();
		}
	}
}


void CPackageOverviewComp::OnItemCollapsed(QTreeWidgetItem* item)
{
	if (item != NULL && item->childCount() > 0){
		item->setIcon(0, QIcon(QString::fromUtf8(":/Resources/Icons/dirclosed-16.png")));
	}
}


void CPackageOverviewComp::OnItemExpanded(QTreeWidgetItem* item)
{
	if (item != NULL && item->childCount() > 0){
		item->setIcon(0, QIcon(QString::fromUtf8(":/Resources/Icons/diropen-16.png")));
	}
}


// protected methods

void CPackageOverviewComp::AddPackage(const icomp::IComponentStaticInfo* staticInfoPtr, const QString& packageName)
{
	using iqt::GetQString;

	QTreeWidgetItem* libraryItem = new QTreeWidgetItem();
	libraryItem->setText(0, packageName);
	libraryItem->setForeground(0, Qt::darkBlue);
	libraryItem->setForeground(1, Qt::darkBlue);

	m_rootLocalItem->addChild(libraryItem);

	// create the component list:
	const icomp::IComponentStaticInfo::SubcomponentInfos& subComponents = staticInfoPtr->GetSubcomponentInfos();

	for(int packageIndex = 0; packageIndex < subComponents.GetLocalElementsCount(); packageIndex++){
		const std::string& componentName = subComponents.GetLocalKeyAt(packageIndex);
		const icomp::IComponentStaticInfo* staticInfoPtr = subComponents.GetValueAt(packageIndex);
	
		PackageComponentItem* componentItem = new PackageComponentItem(libraryItem);
		componentItem->m_componentInfoPtr.SetPtr(new CStaticComponentInfo(staticInfoPtr, packageName.toStdString(), componentName));
		libraryItem->addChild(componentItem);

		componentItem->setFlags(Qt::ItemIsDragEnabled | Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled);
		componentItem->setText(0, componentName.c_str());

		QVariant componentStaticInfoData(QVariant::Int,&staticInfoPtr);
		componentItem->setData(0, ComponentStaticInfo, componentStaticInfoData);
	}

	libraryItem->setExpanded(true);

	HighlightComponents(QString());
}


// reimplemented (QObject)

bool CPackageOverviewComp::eventFilter(QObject* eventObject, QEvent* event)
{
	int componentInfoAddress = 0;

	QWidget* sourceWidgetPtr = dynamic_cast<QWidget*>(eventObject);
	I_ASSERT(sourceWidgetPtr != NULL);

	switch (event->type()){
	case QEvent::MouseButtonPress:
		{
			QMouseEvent* mouseEvent = dynamic_cast<QMouseEvent*>(event);
			I_ASSERT(mouseEvent != NULL);

			if (mouseEvent->button() == Qt::LeftButton && m_selectedComponentInfoPtr != NULL){
				QMimeData* mimeData = new QMimeData;
				if (m_selectedComponentInfoPtr != NULL){
					componentInfoAddress = int(m_selectedComponentInfoPtr);
				}
				QByteArray byteData = QByteArray::number(componentInfoAddress);

				mimeData->setData("component", byteData);

				QDrag *drag = new QDrag(sourceWidgetPtr);
				drag->setMimeData(mimeData);
				drag->start(Qt::MoveAction);
			}
		}
	}

	return false;
}


// reimplemented (CGuiComponentBase)

void CPackageOverviewComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	QTreeWidget* widgetPtr = GetQtWidget();
	I_ASSERT(widgetPtr);
	
	connect(widgetPtr, 
		SIGNAL(itemSelectionChanged()), 
		this, 
		SLOT(OnItemSelected()));

	connect(widgetPtr, 
		SIGNAL(itemCollapsed(QTreeWidgetItem*)), 
		this, 
		SLOT(OnItemCollapsed(QTreeWidgetItem*)));

	connect(widgetPtr, 
		SIGNAL(itemExpanded(QTreeWidgetItem*)), 
		this, 
		SLOT(OnItemExpanded(QTreeWidgetItem*)));

	// set up the tree view:
	widgetPtr->setColumnCount(2);
	QStringList labels;
	labels << tr("Component") << tr("Group");
	widgetPtr->setHeaderLabels(labels);
	widgetPtr->setItemDelegate(new CItemDelegate());

	widgetPtr->header()->setResizeMode(QHeaderView::ResizeToContents);
	widgetPtr->header()->hide();

//	widgetPtr->setDragEnabled(true);
//	widgetPtr->setDragDropMode(QAbstractItemView::DragOnly);

	widgetPtr->setIndentation(15);

	m_rootLocalItem = new QTreeWidgetItem();
	m_rootLocalItem->setText(0, tr("Local Components"));
	widgetPtr->addTopLevelItem(m_rootLocalItem);
	m_rootLocalItem->setExpanded(true);
	QFont font = qApp->font();
	font.setBold(true);
	m_rootLocalItem->setFont(0, font);

	m_rootComposedItem = new QTreeWidgetItem();
	m_rootComposedItem->setText(0,tr("Composed Components"));
	widgetPtr->addTopLevelItem(m_rootComposedItem);
	m_rootComposedItem->setExpanded(true);
	m_rootComposedItem->setFont(0, font);

	widgetPtr->viewport()->installEventFilter(this);

	GenerateComponentTree();
}
