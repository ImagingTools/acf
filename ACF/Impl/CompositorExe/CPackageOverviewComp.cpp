#include "CPackageOverviewComp.h"


// Qt includes
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QApplication>
#include <QHeaderView>
#include <QItemDelegate>

#include "istd/CString.h"

#include "iser/CMemoryReadArchive.h"
#include "iser/CMemoryWriteArchive.h"


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
	: BaseClass()
{
}


// reimplemented (IAttributeSelectionObserver)

void CPackageOverviewComp::OnAttributeSelected(const icomp::IAttributeStaticInfo* attributeStaticInfoPtr)
{
	if (attributeStaticInfoPtr != NULL){		
		bool isReference = (attributeStaticInfoPtr->GetAttributeType() == typeid(icomp::CReferenceAttribute));
		bool isMultiReference = (attributeStaticInfoPtr->GetAttributeType() == typeid(icomp::CMultiReferenceAttribute));
		bool isFactory = (attributeStaticInfoPtr->GetAttributeType() == typeid(icomp::CFactoryAttribute));
		bool isMultiFactory = (attributeStaticInfoPtr->GetAttributeType() == typeid(icomp::CMultiFactoryAttribute));

		if (isReference || isMultiReference || isFactory || isMultiFactory){
			HighlightComponents(attributeStaticInfoPtr->GetRelatedInterfaceType().name());

			return;
		}
	}

	HighlightComponents(QString());
}


// protected methods

void CPackageOverviewComp::GenerateComponentTree()
{
	if (!m_generalStaticInfoPtr.IsValid()){
		return;
	}

	const icomp::CPackageStaticInfo::SubcomponentInfos& subcomponentInfos = m_generalStaticInfoPtr->GetSubcomponentInfos();
	int subcomponentsCount = subcomponentInfos.GetElementsCount();

	for (int i = 0; i < subcomponentsCount; ++i){
		const std::string& packageId = subcomponentInfos.GetKeyAt(i);
		const icomp::CPackageStaticInfo* packageInfoPtr = dynamic_cast<const icomp::CPackageStaticInfo*>(subcomponentInfos.GetValueAt(i));
		I_ASSERT(packageInfoPtr != NULL);

		if (packageInfoPtr != NULL){
			QTreeWidgetItem* packageItemPtr = new QTreeWidgetItem();
			packageItemPtr->setText(0, packageId.c_str());
			packageItemPtr->setForeground(0, Qt::darkBlue);
			packageItemPtr->setForeground(1, Qt::darkBlue);

			GeneratePackageTree(packageId, *packageInfoPtr, *packageItemPtr);

			m_rootLocalItem->addChild(packageItemPtr);
		}
	}
}


void CPackageOverviewComp::HighlightComponents(const QString& interfaceId)
{
	QTreeWidget* widgetPtr = GetQtWidget();
	I_ASSERT(widgetPtr);

	if (widgetPtr->topLevelItemCount() == 0){
		return;
	}

	for (QTreeWidgetItemIterator treeIter(widgetPtr); *treeIter != NULL; ++treeIter){
		QTreeWidgetItem* itemPtr = *treeIter;

		QTreeWidgetItem* parentItem = itemPtr->parent();
		bool hasChilds = itemPtr->childCount() > 0;

		const icomp::IComponentStaticInfo* staticInfoPtr = GetItemStaticInfo(*itemPtr);

		QIcon itemIcon;
		if ((staticInfoPtr != NULL) && !interfaceId.isEmpty()){
			icomp::IComponentStaticInfo::InterfaceExtractors interfaceExtractors = staticInfoPtr->GetInterfaceExtractors();
			const icomp::IComponentStaticInfo::InterfaceExtractorPtr* extractorPtr = interfaceExtractors.FindElement(interfaceId.toStdString().c_str());
			if (extractorPtr != NULL){
				itemIcon = QIcon(QString::fromUtf8(":/Resources/Icons/ok-16.png"));

				if (parentItem != NULL){
					parentItem->setExpanded(true);
				}
			}
			else{
				itemIcon = QIcon(QString::fromUtf8(":/Resources/Icons/close_a_128.png"));
			}
		}
		else if (hasChilds){
			if (itemPtr->isExpanded()){
				itemIcon = QIcon(s_openIcon);
			}
			else{			
				itemIcon = QIcon(s_closedIcon);
			}
		}

		itemPtr->setIcon(0, itemIcon);
	}
}


// protected slots

void CPackageOverviewComp::OnItemCollapsed(QTreeWidgetItem* item)
{
	if (item != NULL && item->childCount() > 0){
		item->setIcon(0, QIcon(s_closedIcon));
	}
}


void CPackageOverviewComp::OnItemExpanded(QTreeWidgetItem* item)
{
	if (item != NULL && item->childCount() > 0){
		item->setIcon(0, QIcon(s_openIcon));
	}
}


// protected methods

void CPackageOverviewComp::GeneratePackageTree(
			const std::string& packageId,
			const icomp::CPackageStaticInfo& packageInfo,
			QTreeWidgetItem& root)
{
	// create the component list:
	icomp::CPackageStaticInfo::SubcomponentInfos subcomponentInfos = packageInfo.GetSubcomponentInfos();
	int subcomponentsCount = subcomponentInfos.GetElementsCount();

	for (int i = 0; i < subcomponentsCount; ++i){
		const std::string& componentId = subcomponentInfos.GetKeyAt(i);
		const icomp::IComponentStaticInfo* componentInfoPtr = subcomponentInfos.GetValueAt(i);
		I_ASSERT(componentInfoPtr != NULL);

		icomp::CComponentAddress address(packageId, componentId);
		PackageComponentItem* componentItem = new PackageComponentItem(&root, address);
		root.addChild(componentItem);
	}

	root.setExpanded(true);

	HighlightComponents(QString());
}


const icomp::IComponentStaticInfo* CPackageOverviewComp::GetItemStaticInfo(const QTreeWidgetItem& item) const
{
	const PackageComponentItem* componentItemPtr = dynamic_cast<const PackageComponentItem*>(&item);
	if ((componentItemPtr != NULL) && (m_generalStaticInfoPtr.IsValid())){
		const icomp::CComponentAddress& address = componentItemPtr->GetAddress();
		const icomp::IComponentStaticInfo* packageInfoPtr = m_generalStaticInfoPtr->GetSubcomponent(address.GetPackageId());
		if (packageInfoPtr != NULL){
			return packageInfoPtr->GetSubcomponent(address.GetComponentId());
		}
	}

	return NULL;
}


// reimplemented (QObject)

bool CPackageOverviewComp::eventFilter(QObject* eventObject, QEvent* event)
{
	int componentInfoAddress = 0;

	QWidget* sourceWidgetPtr = dynamic_cast<QWidget*>(eventObject);
	I_ASSERT(sourceWidgetPtr != NULL);

	QTreeWidget* treeWidgetPtr = GetQtWidget();
	I_ASSERT(treeWidgetPtr);

	switch (event->type()){
	case QEvent::MouseButtonPress:
		{
			QMouseEvent* mouseEvent = dynamic_cast<QMouseEvent*>(event);
			I_ASSERT(mouseEvent != NULL);

			if (mouseEvent->button() == Qt::LeftButton){
				QModelIndex modelIndex = treeWidgetPtr->indexAt(mouseEvent->pos());
				QTreeWidgetItem* pressedItemPtr = reinterpret_cast<QTreeWidgetItem*>(modelIndex.internalPointer());
				if (pressedItemPtr != NULL){
					PackageComponentItem* selectedItemPtr = dynamic_cast<PackageComponentItem*>(pressedItemPtr);
					if (selectedItemPtr != NULL){
						QMimeData* mimeData = new QMimeData;

						icomp::CComponentAddress address = selectedItemPtr->GetAddress();
						iser::CMemoryWriteArchive archive;
						if (address.Serialize(archive)){
							QByteArray byteData = QByteArray((const char*)archive.GetBuffer(), archive.GetBufferSize());

							mimeData->setData("component", byteData);

							QDrag *drag = new QDrag(sourceWidgetPtr);
							drag->setMimeData(mimeData);
							drag->start(Qt::MoveAction);
						}
					}
				}
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


// private static members

QString CPackageOverviewComp::s_closedIcon = ":/Resources/Icons/dirclosed-16.png";
QString CPackageOverviewComp::s_openIcon = ":/Resources/Icons/diropen-16.png";
