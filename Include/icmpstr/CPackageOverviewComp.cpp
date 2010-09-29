#include "icmpstr/CPackageOverviewComp.h"


// Qt includes
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QApplication>
#include <QHeaderView>
#include <QItemDelegate>
#include <QBitmap>
#include <QPushButton>
#include <QPainter>
#include <QMenu>
#include <QMimeData>
#include <QDrag>


// ACF includes
#include "istd/CString.h"

#include "iser/CMemoryReadArchive.h"
#include "iser/CMemoryWriteArchive.h"

#include "icomp/CComponentMetaDescriptionEncoder.h"

#include "iqt/CSignalBlocker.h"


namespace icmpstr
{


class CItemDelegate: public QItemDelegate
{
public:
	typedef QItemDelegate BaseClass;

	enum Margin
	{
		SIDE_OFFSET = 4
	};

	CItemDelegate()
	{
		m_componentNameFont.setPointSize(10);
		m_componentNameFont.setBold(true);
		m_componentDescriptionFont.setPointSize(8);

		m_defaultComponentIcon = QIcon(":/Icons/CompositorIcon.svg").pixmap(QSize(64, 64), QIcon::Disabled);
}

	void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
	{
		CPackageOverviewComp::PackageItemBase* selectedItemPtr = dynamic_cast<CPackageOverviewComp::PackageItemBase*>(reinterpret_cast<QTreeWidgetItem*>(index.internalPointer()));
		if (selectedItemPtr == NULL){ 
			BaseClass::paint(painter, option, index);
			
			return;
		}

		UpdateGradients(option);

		CPackageOverviewComp::PackageItem* packageItemPtr = dynamic_cast<CPackageOverviewComp::PackageItem*>(selectedItemPtr);

	 	QRect mainRect = option.rect;
		mainRect.adjust(2 * SIDE_OFFSET, SIDE_OFFSET, 2 * -SIDE_OFFSET, -1);

		if (packageItemPtr != NULL){
			if (((option.state & QStyle::State_Selected) != 0) || ((option.state & QStyle::State_HasFocus) != 0)){
				painter->setBrush(m_selectedPackageItemBrush);
			}
			else{
				painter->setBrush(m_normalPackageItemBrush);
			}

			if ((option.state & QStyle::State_Open) != 0){
				painter->save();
				painter->setRenderHint(QPainter::Antialiasing, true);
				painter->setPen(QPen(option.palette.dark().color(), 2));
				painter->drawRoundedRect(mainRect, SIDE_OFFSET, SIDE_OFFSET);
				painter->restore();
			}
			else{
				QColor packageAroundColor = option.palette.dark().color();
				packageAroundColor.setAlpha(150);
				painter->setPen(QPen(packageAroundColor));
				painter->drawRoundedRect(mainRect, SIDE_OFFSET, SIDE_OFFSET);
			}
		}
		else{
			if (option.state & QStyle::State_Selected){
				painter->fillRect(mainRect, m_selectedItemBrush);
			}

			// draw icon:
			int iconSize = mainRect.height() - SIDE_OFFSET * 2;
			QIcon componentIcon = selectedItemPtr->icon(0);
			if (componentIcon.isNull()){
				componentIcon = m_defaultComponentIcon;
			}

			if (!componentIcon.isNull()){
				QRectF iconRect(
							mainRect.left() + SIDE_OFFSET,
							mainRect.top() + SIDE_OFFSET,
							iconSize,
							iconSize);

				componentIcon.paint(painter, iconRect.toRect());
				mainRect.adjust(mainRect.height() + SIDE_OFFSET, 0, 0, 0);
			}
		}

		mainRect.adjust(SIDE_OFFSET, 1, 0, -1);

		painter->setPen(option.palette.text().color());

		QString componentName = selectedItemPtr->text(0);
		
		QString componentDescription = selectedItemPtr->GetDescription();		

		painter->setFont(m_componentNameFont);
		painter->drawText(mainRect, Qt::AlignTop | Qt::AlignLeft | Qt::TextSingleLine, componentName);

		if (!componentDescription.isEmpty()){
			painter->setFont(m_componentDescriptionFont);
			painter->drawText(
						mainRect,
						Qt::AlignBottom | Qt::AlignLeft | Qt::TextSingleLine, componentDescription);
		}
	}


	virtual QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
	{
		CPackageOverviewComp::PackageItemBase* selectedItemPtr = dynamic_cast<CPackageOverviewComp::PackageItemBase*>(reinterpret_cast<QTreeWidgetItem*>(index.internalPointer()));
		if (selectedItemPtr == NULL){
			QSize size = QItemDelegate::sizeHint(option, index);

			size.setHeight(25);

			return size;
		}

		QSize size = BaseClass::sizeHint(option, index);

		int itemHeight = CalculateItemHeight();

		size.setHeight(itemHeight);

		return size;
	}

private:
	int CalculateItemHeight() const
	{
		QFontMetrics nameFontMetrics(m_componentNameFont);
		QFontMetrics descriptionFontMetrics(m_componentDescriptionFont);

		int height = nameFontMetrics.height();
		height += descriptionFontMetrics.height();
		height += 3 * SIDE_OFFSET; // margin

		return height;
	}

	void UpdateGradients(const QStyleOptionViewItem& option) const
	{
		QRectF rect = option.rect;

		QColor startColor = QColor("#fafafa");
		QColor midColor = QColor("#f4f4f4");
		QColor endColor = QColor("#e7e7e7");

		midColor.setAlpha(192);
		startColor.setAlpha(192);
		endColor.setAlpha(192);

		QLinearGradient normalPackageItemGradient(rect.left(), rect.top(), rect.left(), rect.bottom());
		normalPackageItemGradient.setColorAt(0, startColor);
		normalPackageItemGradient.setColorAt(0.4, midColor);
		normalPackageItemGradient.setColorAt(0.5, endColor);
		normalPackageItemGradient.setColorAt(0.9, startColor);
		m_normalPackageItemBrush = normalPackageItemGradient;

		QColor startColor3 = QColor(0, 202, 86, 192);
		QColor endColor3 = startColor3;
		startColor3.setAlpha(80);
		endColor3.setAlpha(164);

		QLinearGradient selectedPackageItemGradient(rect.left(), rect.top(), rect.left(), rect.bottom());
		selectedPackageItemGradient.setColorAt(0, startColor3);
		selectedPackageItemGradient.setColorAt(0.4, startColor3);
		selectedPackageItemGradient.setColorAt(0.5, endColor3);
		selectedPackageItemGradient.setColorAt(0.9, startColor3);
		m_selectedPackageItemBrush = selectedPackageItemGradient;

		m_selectedItemBrush =  QColor(10, 242, 126, 128);
	}

private:
	QFont m_componentNameFont;
	QFont m_componentDescriptionFont;

	QIcon m_defaultComponentIcon;

	mutable QBrush m_selectedPackageItemBrush;
	mutable QBrush m_normalPackageItemBrush;
	mutable QBrush m_selectedItemBrush;
};


// reimplemented (IAttributeSelectionObserver)

void CPackageOverviewComp::OnAttributeSelected(const icomp::IAttributeStaticInfo* attributeStaticInfoPtr)
{
	InterfaceFilter filter;

	if (attributeStaticInfoPtr != NULL){
		const std::string attributeType = attributeStaticInfoPtr->GetAttributeTypeName();
		bool isReference = (attributeType == icomp::CReferenceAttribute::GetTypeName());
		bool isMultiReference = (attributeType == icomp::CMultiReferenceAttribute::GetTypeName());
		bool isFactory = (attributeType == icomp::CFactoryAttribute::GetTypeName());
		bool isMultiFactory = (attributeType == icomp::CMultiFactoryAttribute::GetTypeName());

		if (isReference || isMultiReference || isFactory || isMultiFactory){
			const istd::CClassInfo& interfaceType = attributeStaticInfoPtr->GetRelatedInterfaceType();
			int index = InterfaceCB->findText(interfaceType.GetName().c_str());
			if (index > 0){
				filter.insert(interfaceType);

				InterfaceCB->setCurrentIndex(index);
			}
		}
	}

	if (filter != m_interfaceFilter){
		m_interfaceFilter = filter;

		if (filter.empty()){
			iqt::CSignalBlocker blocker(InterfaceCB);

			InterfaceCB->setCurrentIndex(0);
		}

		if (FilterGB->isChecked()){
			UpdateComponentsView();
		}
	}
}


// protected methods

void CPackageOverviewComp::GenerateComponentTree()
{
	PackagesList->clear();
	m_roots.clear();

	if (!m_envManagerCompPtr.IsValid()){
		return;
	}

	icomp::IMetaInfoManager::ComponentAddresses addresses = GetFilteredComponentAdresses();

	istd::CString currentKey = iqt::GetCString(GroupByCB->currentText()); 

	for (		icomp::IMetaInfoManager::ComponentAddresses::const_iterator addressIter = addresses.begin();
				addressIter != addresses.end();
				++addressIter){
		const icomp::CComponentAddress& address = *addressIter;
		const icomp::IComponentStaticInfo* metaInfoPtr = m_envManagerCompPtr->GetComponentMetaInfo(address);

		istd::CStringList groupIds;
		std::string elementName = address.GetPackageId() + "/" + address.GetComponentId();
		switch (GroupByCB->currentIndex()){
		case GM_NONE:
			groupIds.push_back("");
			break;

		case GM_PACKAGE:
			groupIds.push_back(address.GetPackageId());
			elementName = address.GetComponentId();
			break;

		default:
			{
				icomp::CComponentMetaDescriptionEncoder encoder(metaInfoPtr->GetKeywords());

				groupIds = encoder.GetValues(currentKey);
			}
		}

		QIcon icon;
		if (m_consistInfoCompPtr.IsValid()){
			icon = m_consistInfoCompPtr->GetComponentIcon(address);
		}

		if (icon.isNull() && metaInfoPtr != NULL){
			switch (metaInfoPtr->GetComponentType()){
				case icomp::IComponentStaticInfo::CT_COMPOSITE:
					icon = m_compositeComponentIcon.pixmap(QSize(64, 64), QIcon::Disabled);
					break;
				case icomp::IComponentStaticInfo::CT_REAL:
					icon = m_realComponentIcon.pixmap(QSize(64, 64), QIcon::Disabled);
					break;
			}
		}

		for (		istd::CStringList::const_iterator iter = groupIds.begin();
					iter != groupIds.end();
					++iter){
			PackageComponentItem* itemPtr = new PackageComponentItem(
						*this,
						address,
						metaInfoPtr,
						icon);
			itemPtr->setText(0, elementName.c_str());

			RootInfo& rootInfo = EnsureRoot(iter->ToString(), address, metaInfoPtr);
			I_ASSERT(rootInfo.itemPtr != NULL);

			rootInfo.itemPtr->addChild(itemPtr);
		}
	}

	for (		RootInfos::const_iterator toolTipIter = m_roots.begin();
				toolTipIter != m_roots.end();
				++toolTipIter){
		const RootInfo& info = toolTipIter->second;
		if (info.itemPtr != NULL){
			QString typeName;
			if (info.hasRealComponents){
				if (info.hasCompositeComponents){
					typeName = "mixed";
				}
				else{
					typeName = "real";
				}
			}
			else if (info.hasCompositeComponents){
				typeName = "composite";
			}
			else{
				typeName = "unknown";
			}

			QString toolTip = tr("Group containing components of %2 type").arg(typeName);
			info.itemPtr->setToolTip(0, toolTip);
		}
	}

	// sort keywords alphabetically:
	QAbstractItemModel* keywordsListModelPtr = PackagesList->model();
	I_ASSERT(keywordsListModelPtr != NULL);
	if (keywordsListModelPtr != NULL){
		keywordsListModelPtr->sort(0);
	}
}


void CPackageOverviewComp::UpdateComponentsView()
{
	GenerateComponentTree();
}


void CPackageOverviewComp::UpdateComponentGroups()
{
	if (!m_envManagerCompPtr.IsValid()){
		return;
	}

	QSet<QString> categories;

	icomp::IMetaInfoManager::ComponentAddresses addresses = m_envManagerCompPtr->GetComponentAddresses();

	for (		icomp::IMetaInfoManager::ComponentAddresses::const_iterator addressIter = addresses.begin();
				addressIter != addresses.end();
				++addressIter){
		const icomp::CComponentAddress& address = *addressIter;
		const icomp::IComponentStaticInfo* metaInfoPtr = m_envManagerCompPtr->GetComponentMetaInfo(address);
	
		if (metaInfoPtr != NULL){
			icomp::CComponentMetaDescriptionEncoder encoder(metaInfoPtr->GetKeywords());

			QStringList categoryList = iqt::GetQStringList(encoder.GetMetaKeys());

			for (int index = 0; index < categoryList.count(); index++){
				categories.insert(categoryList[index]);
			}
		}
	}

	GroupByCB->addItems(categories.toList());
}


icomp::IMetaInfoManager::ComponentAddresses CPackageOverviewComp::GetFilteredComponentAdresses() const
{
	icomp::IMetaInfoManager::ComponentAddresses filteredComponentAdresses;

	if (!m_envManagerCompPtr.IsValid()){
		return filteredComponentAdresses;
	}
	
	icomp::IMetaInfoManager::ComponentAddresses addresses = m_envManagerCompPtr->GetComponentAddresses();

	for (		icomp::IMetaInfoManager::ComponentAddresses::const_iterator addressIter = addresses.begin();
				addressIter != addresses.end();
				++addressIter){
		const icomp::CComponentAddress& address = *addressIter;
		const icomp::IComponentStaticInfo* metaInfoPtr = m_envManagerCompPtr->GetComponentMetaInfo(address);

		QStringList keywords;
		keywords << address.GetComponentId().c_str();

		if (metaInfoPtr != NULL){
			icomp::CComponentMetaDescriptionEncoder encoder(metaInfoPtr->GetKeywords());

			keywords << iqt::GetQStringList(encoder.GetValues());
		}

		if (FilterGB->isChecked()){
			bool isFilterMatched = true;

			if (metaInfoPtr != NULL){
				const icomp::IComponentStaticInfo::InterfaceExtractors& interfaceExtractors = metaInfoPtr->GetInterfaceExtractors();
				for (		InterfaceFilter::const_iterator iterfaceIter = m_interfaceFilter.begin();
							iterfaceIter != m_interfaceFilter.end();
							++iterfaceIter){
					const istd::CClassInfo& interfaceInfo = *iterfaceIter;

					if (interfaceExtractors.FindElement(interfaceInfo) == NULL){
						isFilterMatched = false;
						break;
					}
				}
			}

			for (		QStringList::const_iterator filterIter = m_keywordsFilter.begin();
						filterIter != m_keywordsFilter.end();
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
					isFilterMatched = false;
					break;
				}
			}

			if (!isFilterMatched){
				continue;
			}
		}

		filteredComponentAdresses.insert(address);
	}

	return filteredComponentAdresses;

}


// protected slots

void CPackageOverviewComp::on_FilterEdit_editingFinished()
{
	QStringList keywordsFilter = FilterEdit->text().split(QChar(' '), QString::SkipEmptyParts,  Qt::CaseInsensitive);

	if (keywordsFilter != m_keywordsFilter){
		m_keywordsFilter = keywordsFilter;

		if (FilterGB->isChecked()){
			UpdateComponentsView();
		}
	}
}


void CPackageOverviewComp::on_GroupByCB_currentIndexChanged(int /*index*/)
{
	UpdateComponentsView();
}


void CPackageOverviewComp::on_PackagesList_itemSelectionChanged()
{
	QList<QTreeWidgetItem*> items = PackagesList->selectedItems();
	if (items.count() > 0){
		const PackageComponentItem* itemPtr = dynamic_cast<const PackageComponentItem*>(items.at(0));

		if ((itemPtr != NULL) && m_quickHelpViewerCompPtr.IsValid()){
			const icomp::CComponentAddress& address = itemPtr->GetAddress();

			m_quickHelpViewerCompPtr->ShowHelp(address.GetPackageId() + "/" + address.GetComponentId(), &address);
		}
	}
}


void CPackageOverviewComp::on_PackagesList_itemExpanded(QTreeWidgetItem* item)
{
	for (int index = 0; index < PackagesList->topLevelItemCount(); index++){
		QTreeWidgetItem* itemPtr = PackagesList->topLevelItem(index);

		if (itemPtr != item && itemPtr->isExpanded()){
			itemPtr->setExpanded(false);
		}
	}
}


void CPackageOverviewComp::on_PackagesList_itemClicked(QTreeWidgetItem* item, int /*column*/)
{
	PackageItem* packageItemPtr = dynamic_cast<CPackageOverviewComp::PackageItem*>(item);
	if (packageItemPtr != NULL){
		item->setExpanded(!item->isExpanded());
	}
}


void CPackageOverviewComp::on_FilterGB_toggled(bool /*on*/)
{
	if (!m_interfaceFilter.empty() || !m_keywordsFilter.isEmpty()){
		UpdateComponentsView();
	}
}


void CPackageOverviewComp::on_InterfaceCB_currentIndexChanged(int index)
{
	InterfaceFilter filter;

	if (index > 0){
		istd::CClassInfo info(InterfaceCB->itemText(index).toStdString());

		filter.insert(info);
	}

	if (filter != m_interfaceFilter){
		m_interfaceFilter = filter;

		if (FilterGB->isChecked()){
			UpdateComponentsView();
		}
	}
}


// protected methods

QPixmap CPackageOverviewComp::CreateComponentDragPixmap(const icomp::CComponentAddress &address) const
{
	QPushButton componentLabel(NULL);
	componentLabel.setFlat(true);
	QFont font = componentLabel.font();
	font.setBold(true);
	font.setPointSize(12);
	componentLabel.setFont(font);
	componentLabel.setText(iqt::GetQString(address.GetPackageId()) + "/" + iqt::GetQString(address.GetComponentId()));
	if (m_consistInfoCompPtr.IsValid()){
		componentLabel.setIconSize(QSize(64, 64));
		componentLabel.setIcon(m_consistInfoCompPtr->GetComponentIcon(address));
	}

	componentLabel.adjustSize();

	QPixmap pixmap = QPixmap::grabWidget(&componentLabel);
	QPalette palette = componentLabel.palette();

	pixmap.setMask(pixmap.createMaskFromColor(palette.color(QPalette::Window), Qt::MaskInColor));
	
	QPainter painter(&pixmap);
	painter.setPen(Qt::DashDotLine);
	QRectF mainRect = pixmap.rect();
	mainRect.adjust(0, 0, -1, -1);
	painter.drawRect(mainRect);
	painter.end();

	return pixmap;
}


CPackageOverviewComp::RootInfo& CPackageOverviewComp::EnsureRoot(const std::string& path, const icomp::CComponentAddress& address, const icomp::IComponentStaticInfo* staticInfoPtr)
{
	RootInfo& rootInfo = m_roots[path];

	int componentType = icomp::IComponentStaticInfo::CT_NONE; 
	if (staticInfoPtr != NULL){
		componentType = staticInfoPtr->GetComponentType();
	}

	switch (componentType){
	case icomp::IComponentStaticInfo::CT_REAL:
		rootInfo.hasRealComponents = true;
		break;

	case icomp::IComponentStaticInfo::CT_COMPOSITE:
		rootInfo.hasCompositeComponents = true;
		break;
	}

	if (rootInfo.itemPtr == NULL){
		if (path.empty()){
			rootInfo.itemPtr = PackagesList->invisibleRootItem();
		}
		else{
			std::string groupName;
			std::string elementName = path;

			std::string::size_type slashPos = path.rfind('/');
			if (slashPos != std::string::npos){
				groupName = path.substr(0, slashPos);
				elementName = path.substr(slashPos + 1);
			}

			RootInfo& groupRoot = EnsureRoot(groupName, address, staticInfoPtr);
			I_ASSERT(groupRoot.itemPtr != NULL);

			QString packageDescription;
			if ((GroupByCB->currentIndex() == GM_PACKAGE) && m_envManagerCompPtr.IsValid()){
				const icomp::IComponentStaticInfo* packageInfoPtr = m_envManagerCompPtr->GetPackageMetaInfo(address.GetPackageId());
				if (packageInfoPtr != NULL){
					packageDescription = iqt::GetQString(packageInfoPtr->GetDescription());
				}
			}

			rootInfo.itemPtr = new PackageItem(*this, packageDescription, QIcon());
			rootInfo.itemPtr->setText(0, elementName.c_str());
			groupRoot.itemPtr->addChild(rootInfo.itemPtr);
		}
	}

	return rootInfo;
}


// reimplemented (QObject)

bool CPackageOverviewComp::eventFilter(QObject* sourcePtr, QEvent* eventPtr)
{
	QWidget* sourceWidgetPtr = dynamic_cast<QWidget*>(sourcePtr);
	QAbstractItemView* itemViewPtr = NULL;
	if (sourceWidgetPtr != NULL){
		itemViewPtr = dynamic_cast<QAbstractItemView*>(sourceWidgetPtr->parentWidget());
		if (itemViewPtr == NULL){
			return BaseClass::eventFilter(sourcePtr, eventPtr);
		}
	}

	switch (eventPtr->type()){
		case QEvent::MouseButtonPress:
		{
			QMouseEvent* mouseEvent = dynamic_cast<QMouseEvent*>(eventPtr);
			I_ASSERT(mouseEvent != NULL);

			if (mouseEvent->button() == Qt::LeftButton && sourceWidgetPtr != NULL){
				PackageComponentItem* selectedItemPtr = NULL;

				QModelIndex componentModelIndex = itemViewPtr->indexAt(mouseEvent->pos());
				selectedItemPtr = dynamic_cast<PackageComponentItem*>(reinterpret_cast<QTreeWidgetItem*>(componentModelIndex.internalPointer()));

				if (selectedItemPtr != NULL){
					QMimeData* mimeData = new QMimeData;

					icomp::CComponentAddress address = selectedItemPtr->GetAddress();
					iser::CMemoryWriteArchive archive;
					if (address.Serialize(archive)){
						QByteArray byteData = QByteArray((const char*)archive.GetBuffer(), archive.GetBufferSize());

						mimeData->setData("component", byteData);

						QDrag *drag = new QDrag(sourceWidgetPtr);
						drag->setMimeData(mimeData);
						drag->setPixmap(CreateComponentDragPixmap(address));
						drag->setHotSpot(QPoint(drag->pixmap().width()/2, drag->pixmap().height()));
						drag->start(Qt::MoveAction);
					}
				}
			}
		}
		break;

	default:
		break;
	}

	return BaseClass::eventFilter(sourcePtr, eventPtr);
}


// reimplemented (CGuiComponentBase)

void CPackageOverviewComp::OnGuiCreated()
{
	BaseClass::OnGuiCreated();

	m_realComponentIcon = QIcon(":/Icons/RealComponent.svg");
	m_compositeComponentIcon = QIcon(":/Icons/CompositeComponent.svg");
	m_mixedComponentIcon = QIcon(":/Icons/MixedComponent.svg");

	// set up the tree view:
	PackagesList->setColumnCount(1);
	PackagesList->setItemDelegate(new CItemDelegate());

	PackagesList->header()->setResizeMode(0, QHeaderView::Stretch);
	PackagesList->header()->hide();

	QPalette palette = qApp->palette();
	palette.setBrush(QPalette::Highlight, palette.base());
	PackagesList->setPalette(palette);

	PackagesList->setStyleSheet("QTreeView::branch {background: palette(base);}");

	PackagesList->viewport()->installEventFilter(this);

	InterfaceFilter knownInterfaces;
	if (m_envManagerCompPtr.IsValid()){
		icomp::IMetaInfoManager::ComponentAddresses addresses = m_envManagerCompPtr->GetComponentAddresses();

		for (		icomp::IMetaInfoManager::ComponentAddresses::const_iterator addressIter = addresses.begin();
					addressIter != addresses.end();
					++addressIter){
			const icomp::CComponentAddress& address = *addressIter;

			const icomp::IComponentStaticInfo* metaInfoPtr = m_envManagerCompPtr->GetComponentMetaInfo(address);
			if (metaInfoPtr != NULL){
				const icomp::IComponentStaticInfo::InterfaceExtractors& interfaceExtractors = metaInfoPtr->GetInterfaceExtractors();

				int interfacesCount = interfaceExtractors.GetElementsCount();
				for (int i = 0; i < interfacesCount; ++i){
					const istd::CClassInfo& interfaceInfo = interfaceExtractors.GetKeyAt(i);
					knownInterfaces.insert(interfaceInfo);
				}
			}
		}

		InterfaceCB->setMaxCount(0);
		InterfaceCB->setMaxCount(int(knownInterfaces.size()));

		InterfaceCB->addItem(tr("Any"));
		for (		InterfaceFilter::const_iterator iterfaceIter = knownInterfaces.begin();
					iterfaceIter != knownInterfaces.end();
					++iterfaceIter){
			const istd::CClassInfo& interfaceInfo = *iterfaceIter;

			InterfaceCB->addItem(interfaceInfo.GetName().c_str());
		}

		InterfaceLabel->setVisible(true);
		InterfaceCB->setVisible(true);
	}
	else{
		InterfaceLabel->setVisible(false);
		InterfaceCB->setVisible(false);
	}

	UpdateComponentGroups();

	UpdateComponentsView();
}


// public methods of embedded class PackageItemBase

CPackageOverviewComp::PackageItemBase::PackageItemBase(
			CPackageOverviewComp& parent,
			const QString& description,
			const QIcon& icon)
:	m_parent(parent),
	m_description(description)
{
	setIcon(0, icon);
}


const QString& CPackageOverviewComp::PackageItemBase::GetDescription() const
{
	return m_description;
}


// public methods of embedded class PackageComponentItem

CPackageOverviewComp::PackageComponentItem::PackageComponentItem(
			CPackageOverviewComp& parent,
			const icomp::CComponentAddress& address,
			const icomp::IComponentStaticInfo* staticInfoPtr,
			const QIcon& icon)
:	BaseClass(parent, (staticInfoPtr != NULL) ? iqt::GetQString(staticInfoPtr->GetDescription()) : QString(), icon),
	m_address(address)
{
	QString toolTip = QObject::tr("Component %1.%2").arg(address.GetPackageId().c_str()).arg(address.GetComponentId().c_str());

	if (staticInfoPtr != NULL){
		setFlags(Qt::ItemIsDragEnabled | Qt::ItemIsEnabled | Qt::ItemIsSelectable);

		toolTip += "\n";

		toolTip += m_description;
	}
	else{
		setFlags(Qt::ItemIsDragEnabled | Qt::ItemIsSelectable);
	}

	setToolTip(0, toolTip);
}


const icomp::CComponentAddress& CPackageOverviewComp::PackageComponentItem::GetAddress() const
{
	return m_address;
}


// public methods of embedded class PackageItem

CPackageOverviewComp::PackageItem::PackageItem(
			CPackageOverviewComp& parent,
			const QString& description,
			const QIcon& icon)
:	BaseClass(parent, description, icon)
{
	setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
}


} // namespace icmpstr


