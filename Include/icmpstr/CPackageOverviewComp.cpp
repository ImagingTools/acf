#include "icmpstr/CPackageOverviewComp.h"


// Qt includes
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QApplication>
#include <QHeaderView>
#include <QItemDelegate>

#include "istd/CString.h"

#include "iser/CMemoryReadArchive.h"
#include "iser/CMemoryWriteArchive.h"


namespace icmpstr
{


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


// reimplemented (IAttributeSelectionObserver)

void CPackageOverviewComp::OnAttributeSelected(const icomp::IAttributeStaticInfo* attributeStaticInfoPtr)
{
	if (attributeStaticInfoPtr != NULL){		
		bool isReference = (attributeStaticInfoPtr->GetAttributeType().IsType<icomp::CReferenceAttribute>());
		bool isMultiReference = (attributeStaticInfoPtr->GetAttributeType().IsType<icomp::CMultiReferenceAttribute>());
		bool isFactory = (attributeStaticInfoPtr->GetAttributeType().IsType<icomp::CFactoryAttribute>());
		bool isMultiFactory = (attributeStaticInfoPtr->GetAttributeType().IsType<icomp::CMultiFactoryAttribute>());

		if (isReference || isMultiReference || isFactory || isMultiFactory){
			HighlightComponents(attributeStaticInfoPtr->GetRelatedInterfaceType());

			return;
		}
	}

	HighlightComponents();
}


// protected methods

void CPackageOverviewComp::GenerateComponentTree(const QString& filter, bool expandComponents)
{
	if (!m_generalStaticInfoCompPtr.IsValid()){
		return;
	}

	PackagesList->clear();
	KeywordsList->clear();

	icomp::IComponentStaticInfo::Ids subcomponentIds = m_generalStaticInfoCompPtr->GetSubcomponentIds();

	for (		icomp::IComponentStaticInfo::Ids::const_iterator iter = subcomponentIds.begin();
				iter != subcomponentIds.end();
				++iter){
		const std::string& packageId = *iter;

		const icomp::IComponentStaticInfo* packageInfoPtr = m_generalStaticInfoCompPtr->GetSubcomponentInfo(packageId);
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

				QTreeWidgetItem* packageItem = packageItemPtr.PopPtr();
				PackagesList->addTopLevelItem(packageItem);
				packageItem->setExpanded(expandComponents);
			}
		}
	}

	// sort keywords alphabetically:
	QAbstractItemModel* keywordsListModelPtr = KeywordsList->model();
	I_ASSERT(keywordsListModelPtr != NULL);
	if (keywordsListModelPtr != NULL){
		keywordsListModelPtr->sort(KeywordColumn);
	}

	HighlightComponents();
}


void CPackageOverviewComp::HighlightComponents(const istd::CClassInfo& interfaceInfo)
{
	int topLevelItemCount = PackagesList->topLevelItemCount();
	if (topLevelItemCount == 0){
		return;
	}

	// process top level items:
	for (int topLevelItemIndex = 0; topLevelItemIndex < topLevelItemCount; topLevelItemIndex++){
		QTreeWidgetItem* itemPtr = PackagesList->topLevelItem(topLevelItemIndex);
		if (itemPtr->childCount()){
			if (itemPtr->isExpanded()){
				itemPtr->setIcon(0, m_openIcon);
			}
			else{			
				itemPtr->setIcon(0, m_closedIcon);
			}
		}
		else{
			itemPtr->setTextColor(0, Qt::lightGray);
		}
	}

	// process component highlighting:
	for (QTreeWidgetItemIterator treeIter(PackagesList); *treeIter != NULL; ++treeIter){
		QTreeWidgetItem* itemPtr = *treeIter;

		QTreeWidgetItem* parentItemPtr = itemPtr->parent();

		const icomp::IComponentStaticInfo* staticInfoPtr = GetItemStaticInfo(*itemPtr);

		QIcon itemIcon;
		if ((staticInfoPtr != NULL) && interfaceInfo.IsValid()){
			icomp::IComponentStaticInfo::InterfaceExtractors interfaceExtractors = staticInfoPtr->GetInterfaceExtractors();
			const icomp::IComponentStaticInfo::InterfaceExtractorPtr* extractorPtr = interfaceExtractors.FindElement(interfaceInfo);
			if (extractorPtr != NULL){
				itemIcon = m_validIcon;

				// if parent item is closed we must expand it:
				if (parentItemPtr != NULL){
					parentItemPtr->setExpanded(true);
				}
			}
			else{
				itemIcon = m_invalidIcon;
			}
		}

		// set result icon to component item:
		if (parentItemPtr != NULL){
			itemPtr->setIcon(1, itemIcon);
		}
	}
}


// protected slots

void CPackageOverviewComp::on_PackagesList_itemCollapsed(QTreeWidgetItem* item)
{
	if (item != NULL && item->childCount() > 0){
		item->setIcon(0, m_closedIcon);
	}
}


void CPackageOverviewComp::on_PackagesList_itemExpanded(QTreeWidgetItem* item)
{
	if (item != NULL && item->childCount() > 0){
		item->setIcon(0, m_openIcon);
	}
}


void CPackageOverviewComp::on_FilterEdit_textChanged(const QString& text)
{
	GenerateComponentTree(text, true);
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

	QDir packageDir;
	bool hasPackageInfo = false;
	if (m_packagesManagerCompPtr.IsValid()){
		QString packageDirPath = iqt::GetQString(m_packagesManagerCompPtr->GetPackageDirPath(packageId));
		if (!packageDirPath.isEmpty()){
			packageDir.setPath(packageDirPath + ".info");
			if (packageDir.exists()){
				hasPackageInfo = true;
			}
		}
	}

	for (		icomp::IComponentStaticInfo::Ids::const_iterator iter = subcomponentIds.begin();
				iter != subcomponentIds.end();
				++iter){
		const std::string& componentId = *iter;
		const icomp::IComponentStaticInfo* componentInfoPtr = packageInfo.GetSubcomponentInfo(componentId);
		if (componentInfoPtr == NULL){
			continue;
		}

		QString keywordString = iqt::GetQString(componentInfoPtr->GetKeywords());
		QStringList keywords = keywordString.split(QChar(' '), QString::SkipEmptyParts,  Qt::CaseInsensitive);

		if (!filter.isEmpty()){
			QStringList filterKeywords = filter.split(QChar(' '), QString::SkipEmptyParts,  Qt::CaseInsensitive);

			bool containsFilter = true;
			for (		QStringList::const_iterator filterIter = filterKeywords.begin();
						filterIter != filterKeywords.end();
						++filterIter){
				const QString& filter = *filterIter;

				bool filterFound = false;
				for (		QStringList::const_iterator keywordIter = keywords.begin();
							keywordIter != keywords.end();
							++keywordIter){
					const QString& keyword = *keywordIter;

					if (keyword.contains(filter, Qt::CaseInsensitive)){
						filterFound = true;
						break;
					}
				}

				if (!filterFound){
					containsFilter = false;
					break;
				}
			}

			if (!containsFilter){
				continue;
			}
		}

		icomp::CComponentAddress address(packageId, componentId);

		for (		QStringList::const_iterator keywordIter = keywords.begin();
					keywordIter != keywords.end();
					++keywordIter){
			const QString& keyword = *keywordIter;

			QTreeWidgetItem* keywordItemPtr = NULL;
			QList<QTreeWidgetItem*> keywordItems = KeywordsList->findItems(keyword, Qt::MatchFixedString);
			if (!keywordItems.isEmpty()){
				keywordItemPtr = keywordItems.first();
			}
			else{
				keywordItemPtr = new QTreeWidgetItem();
				if (keywordItemPtr == NULL){
					return;
				}

				keywordItemPtr->setText(0, keyword);
				keywordItemPtr->setForeground(0, Qt::darkBlue);
				keywordItemPtr->setForeground(1, Qt::darkBlue);

				keywordItemPtr->setToolTip(0, iqt::GetQString(packageInfo.GetDescription()));

				KeywordsList->addTopLevelItem(keywordItemPtr);
			}
			I_ASSERT(keywordItemPtr != NULL);

			keywordItemPtr->addChild(new PackageComponentItem(keywordItemPtr, address, *componentInfoPtr, hasPackageInfo? &packageDir: NULL));
		}

		PackageComponentItem* componentItem = new PackageComponentItem(&root, address, *componentInfoPtr, hasPackageInfo? &packageDir: NULL);

		root.addChild(componentItem);
	}
}


const icomp::IComponentStaticInfo* CPackageOverviewComp::GetItemStaticInfo(const QTreeWidgetItem& item) const
{
	const PackageComponentItem* componentItemPtr = dynamic_cast<const PackageComponentItem*>(&item);
	if ((componentItemPtr != NULL) && (m_generalStaticInfoCompPtr.IsValid())){
		const icomp::CComponentAddress& address = componentItemPtr->GetAddress();
		const icomp::IComponentStaticInfo* packageInfoPtr = m_generalStaticInfoCompPtr->GetSubcomponentInfo(address.GetPackageId());
		if (packageInfoPtr != NULL){
			return packageInfoPtr->GetSubcomponentInfo(address.GetComponentId());
		}
	}

	return NULL;
}


// reimplemented (QObject)

bool CPackageOverviewComp::eventFilter(QObject* eventObject, QEvent* event)
{
	QWidget* sourceWidgetPtr = dynamic_cast<QWidget*>(eventObject);
	I_ASSERT(sourceWidgetPtr != NULL);

	switch (event->type()){
	case QEvent::MouseButtonPress:
		{
			QMouseEvent* mouseEvent = dynamic_cast<QMouseEvent*>(event);
			I_ASSERT(mouseEvent != NULL);

			if (mouseEvent->button() == Qt::LeftButton){
				PackageComponentItem* selectedItemPtr = NULL;

				QModelIndex packagesModelIndex = PackagesList->indexAt(mouseEvent->pos());
				QModelIndex keywordsModelIndex = KeywordsList->indexAt(mouseEvent->pos());

				if (packagesModelIndex.isValid()){
					selectedItemPtr = dynamic_cast<PackageComponentItem*>(reinterpret_cast<QTreeWidgetItem*>(packagesModelIndex.internalPointer()));
				}
				
				if ((selectedItemPtr == NULL) && keywordsModelIndex.isValid()){
					selectedItemPtr = dynamic_cast<PackageComponentItem*>(reinterpret_cast<QTreeWidgetItem*>(keywordsModelIndex.internalPointer()));
				}

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
		break;

	default:
		break;
	}

	return false;
}


// reimplemented (CGuiComponentBase)

void CPackageOverviewComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	m_closedIcon = QIcon(":/Resources/Icons/dirclosed-16.png");
	m_openIcon = QIcon(":/Resources/Icons/diropen-16.png");
	m_validIcon = QIcon(QString::fromUtf8(":/Resources/Icons/ok-16.png"));
	m_invalidIcon = QIcon(QString::fromUtf8(":/Resources/Icons/close_a_128.png"));

	// set up the tree view:
	PackagesList->setColumnCount(2);
	QStringList labels;
	labels << tr("Component");
	PackagesList->setHeaderLabels(labels);
	PackagesList->setItemDelegate(new CItemDelegate());

	PackagesList->header()->setResizeMode(0, QHeaderView::ResizeToContents);
	PackagesList->header()->setResizeMode(1, QHeaderView::Fixed);
	PackagesList->header()->hide();

	PackagesList->setIndentation(15);

	PackagesList->viewport()->installEventFilter(this);

	KeywordsList->setColumnCount(2);
	KeywordsList->setHeaderLabels(labels);
	KeywordsList->setItemDelegate(new CItemDelegate());

	KeywordsList->header()->setResizeMode(0, QHeaderView::ResizeToContents);
	KeywordsList->header()->setResizeMode(1, QHeaderView::Fixed);
	KeywordsList->header()->hide();

	KeywordsList->setIndentation(15);

	KeywordsList->viewport()->installEventFilter(this);

	GenerateComponentTree();
}


// public methods of embedded class 

CPackageOverviewComp::PackageComponentItem::PackageComponentItem(
			QTreeWidgetItem* parentItemPtr,
			const icomp::CComponentAddress& address,
			const icomp::IComponentStaticInfo& staticInfo,
			const QDir* packageDirPtr)
:	QTreeWidgetItem(parentItemPtr), m_address(address)
{
	setFlags(Qt::ItemIsDragEnabled | Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled);
	setText(0, iqt::GetQString(address.GetComponentId()));

	QString toolTip = QObject::tr("Component %1.%2:\n%3")
				.arg(address.GetPackageId().c_str())
				.arg(address.GetComponentId().c_str())
				.arg(iqt::GetQString(staticInfo.GetDescription()));
	setToolTip(0, toolTip);

	if (packageDirPtr != NULL){
		QString iconPath = packageDirPtr->absoluteFilePath((address.GetComponentId() + ".small.png").c_str());
		
		setIcon(0, QIcon(iconPath));
	}
}


} // namespace icmpstr


