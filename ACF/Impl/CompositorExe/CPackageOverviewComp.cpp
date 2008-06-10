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

void CPackageOverviewComp::GenerateComponentTree(const QString& filter)
{
	if (!m_generalStaticInfoPtr.IsValid()){
		return;
	}

	PackagesList->clear();

	m_rootLocalItem = new QTreeWidgetItem();
	m_rootLocalItem->setText(0, tr("Local Components"));
	PackagesList->addTopLevelItem(m_rootLocalItem);
	m_rootLocalItem->setExpanded(true);
	QFont font = qApp->font();
	font.setBold(true);
	m_rootLocalItem->setFont(0, font);

	m_rootComposedItem = new QTreeWidgetItem();
	m_rootComposedItem->setText(0,tr("Composed Components"));
	PackagesList->addTopLevelItem(m_rootComposedItem);
	m_rootComposedItem->setExpanded(true);
	m_rootComposedItem->setFont(0, font);

	icomp::IComponentStaticInfo::Ids subcomponentIds = m_generalStaticInfoPtr->GetSubcomponentIds();

	for (		icomp::IComponentStaticInfo::Ids::const_iterator iter = subcomponentIds.begin();
				iter != subcomponentIds.end();
				++iter){
		const std::string& packageId = *iter;

		const icomp::IComponentStaticInfo* packageInfoPtr = m_generalStaticInfoPtr->GetSubcomponent(packageId);
		if (packageInfoPtr == NULL){
			continue;
		}

		if (packageInfoPtr != NULL){
			istd::TDelPtr<QTreeWidgetItem> packageItemPtr(new QTreeWidgetItem());

			GeneratePackageTree(packageId, *packageInfoPtr, filter, *packageItemPtr);

			if (packageItemPtr->childCount() > 0){
				packageItemPtr->setText(0, packageId.c_str());
				packageItemPtr->setForeground(0, Qt::darkBlue);
				packageItemPtr->setForeground(1, Qt::darkBlue);

				packageItemPtr->setToolTip(0, iqt::GetQString(packageInfoPtr->GetDescription()));

				m_rootLocalItem->addChild(packageItemPtr.PopPtr());
			}
		}
	}
}


void CPackageOverviewComp::HighlightComponents(const QString& interfaceId)
{
	if (PackagesList->topLevelItemCount() == 0){
		return;
	}

	for (QTreeWidgetItemIterator treeIter(PackagesList); *treeIter != NULL; ++treeIter){
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

void CPackageOverviewComp::on_PackagesList_itemCollapsed(QTreeWidgetItem* item)
{
	if (item != NULL && item->childCount() > 0){
		item->setIcon(0, QIcon(s_closedIcon));
	}
}


void CPackageOverviewComp::on_PackagesList_itemExpanded(QTreeWidgetItem* item)
{
	if (item != NULL && item->childCount() > 0){
		item->setIcon(0, QIcon(s_openIcon));
	}
}


void CPackageOverviewComp::on_FilterEdit_textChanged(const QString& text)
{
	GenerateComponentTree(text);
}


void CPackageOverviewComp::on_ResetFilterButton_clicked()
{
	FilterEdit->clear();
}


// protected methods

void CPackageOverviewComp::GeneratePackageTree(
			const std::string& packageId,
			const icomp::IComponentStaticInfo& packageInfo,
			const QString& filter,
			QTreeWidgetItem& root)
{
	// create the component list:
	icomp::IComponentStaticInfo::Ids subcomponentIds = packageInfo.GetSubcomponentIds();

	for (		icomp::IComponentStaticInfo::Ids::const_iterator iter = subcomponentIds.begin();
				iter != subcomponentIds.end();
				++iter){
		const std::string& componentId = *iter;
		const icomp::IComponentStaticInfo* componentInfoPtr = packageInfo.GetSubcomponent(componentId);
		if (componentInfoPtr == NULL){
			continue;
		}

		if (!filter.isEmpty()){
			QString keywords = " " + iqt::GetQString(componentInfoPtr->GetKeywords());
			if (!keywords.contains(" " + filter, Qt::CaseInsensitive)){
				continue;
			}
		}

		icomp::CComponentAddress address(packageId, componentId);
		PackageComponentItem* componentItem = new PackageComponentItem(&root, address);
		componentItem->setToolTip(0, iqt::GetQString(componentInfoPtr->GetDescription()));
		root.addChild(componentItem);
	}

	HighlightComponents(QString());

	root.setExpanded(true);
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

	switch (event->type()){
	case QEvent::MouseButtonPress:
		{
			QMouseEvent* mouseEvent = dynamic_cast<QMouseEvent*>(event);
			I_ASSERT(mouseEvent != NULL);

			if (mouseEvent->button() == Qt::LeftButton){
				QModelIndex modelIndex = PackagesList->indexAt(mouseEvent->pos());
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

	// set up the tree view:
	PackagesList->setColumnCount(2);
	QStringList labels;
	labels << tr("Component") << tr("Group");
	PackagesList->setHeaderLabels(labels);
	PackagesList->setItemDelegate(new CItemDelegate());

	PackagesList->header()->setResizeMode(QHeaderView::ResizeToContents);
	PackagesList->header()->hide();

	PackagesList->setIndentation(15);

	PackagesList->viewport()->installEventFilter(this);

	GenerateComponentTree();
}


// private static members

QString CPackageOverviewComp::s_closedIcon = ":/Resources/Icons/dirclosed-16.png";
QString CPackageOverviewComp::s_openIcon = ":/Resources/Icons/diropen-16.png";


