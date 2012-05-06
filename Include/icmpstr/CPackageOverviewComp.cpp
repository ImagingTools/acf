#include "icmpstr/CPackageOverviewComp.h"


// Qt includes
#include <QtCore/QString>
#include <QtCore/QMimeData>
#include <QtGui/QVBoxLayout>
#include <QtGui/QMouseEvent>
#include <QtGui/QApplication>
#include <QtGui/QHeaderView>
#include <QtGui/QItemDelegate>
#include <QtGui/QBitmap>
#include <QtGui/QPushButton>
#include <QtGui/QPainter>
#include <QtGui/QMenu>
#include <QtGui/QDrag>

// ACF includes
#include "iser/CXmlStringWriteArchive.h"

#include "icomp/CComponentMetaDescriptionEncoder.h"

#include "iqt/CSignalBlocker.h"
#include "iqt/CSystem.h"


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


CPackageOverviewComp::CPackageOverviewComp()
:	m_packagesCommand("", 110, ibase::ICommand::CF_GLOBAL_MENU),
	m_reloadCommand("", 110, ibase::ICommand::CF_GLOBAL_MENU),
	m_registryObserver(this),
	m_configObserver(this),
	m_startDrag(false),
	m_starDragPosition(0, 0)
{
	connect(&m_reloadCommand, SIGNAL(activated()), this, SLOT(OnReloadPackages()));
	m_packagesCommand.InsertChild(&m_reloadCommand);
	m_commands.InsertChild(&m_packagesCommand);
}

// reimplemented (ibase::ICommandsProvider)

const ibase::IHierarchicalCommand* CPackageOverviewComp::GetCommands() const
{
	return &m_commands;
}


// reimplemented (icmpstr::IAttributeSelectionObserver)

void CPackageOverviewComp::OnAttributeSelected(const icomp::IAttributeStaticInfo* attributeStaticInfoPtr)
{
	InterfaceFilter filter;

	if (attributeStaticInfoPtr != NULL){
		const QByteArray attributeType = attributeStaticInfoPtr->GetAttributeTypeName();
		bool isReference = (attributeType == icomp::CReferenceAttribute::GetTypeName());
		bool isMultiReference = (attributeType == icomp::CMultiReferenceAttribute::GetTypeName());
		bool isFactory = (attributeType == icomp::CFactoryAttribute::GetTypeName());
		bool isMultiFactory = (attributeType == icomp::CMultiFactoryAttribute::GetTypeName());

		if (isReference || isMultiReference || isFactory || isMultiFactory){
			icomp::IElementStaticInfo::Ids interfaceNames = attributeStaticInfoPtr->GetRelatedMetaIds(
						icomp::IComponentStaticInfo::MGI_INTERFACES,
						0,
						icomp::IAttributeStaticInfo::AF_NULLABLE);	// Names of the interfaces which must be set
			if (interfaceNames.isEmpty()){
				interfaceNames = attributeStaticInfoPtr->GetRelatedMetaIds(
							icomp::IComponentStaticInfo::MGI_INTERFACES, 0, 0);	// All asked interfaces
			}

			if (!interfaceNames.isEmpty()){
				const QByteArray& interfaceName = *interfaceNames.begin();
				int index = InterfaceCB->findText(interfaceName);
				if (index > 0){
					filter.insert(interfaceName);

					InterfaceCB->setCurrentIndex(index);
				}
			}
		}
	}

	if (filter != m_interfaceFilter){
		m_interfaceFilter = filter;

		if (filter.isEmpty()){
			iqt::CSignalBlocker blocker(InterfaceCB);

			InterfaceCB->setCurrentIndex(0);
		}

		if (FilterGB->isChecked()){
			GenerateComponentTree(false);
		}
	}
}


// protected methods

void CPackageOverviewComp::GenerateComponentTree(bool forceUpdate)
{
	icomp::IMetaInfoManager::ComponentAddresses addresses = GetFilteredComponentAdresses();
	if (!forceUpdate && (addresses == m_shownAddresses)){
		return;
	}

	PackagesList->clear();
	m_roots.clear();

	QString currentKey = GroupByCB->currentText(); 

	for (		icomp::IMetaInfoManager::ComponentAddresses::const_iterator addressIter = addresses.begin();
				addressIter != addresses.end();
				++addressIter){
		const icomp::CComponentAddress& address = *addressIter;

		const icomp::IComponentStaticInfo* metaInfoPtr = NULL;
		if (m_envManagerCompPtr.IsValid()){
			metaInfoPtr = m_envManagerCompPtr->GetComponentMetaInfo(address);
		}

		QStringList groupIds;
		QByteArray elementName;

		if (!address.GetPackageId().isEmpty()){
			elementName += address.GetPackageId() + "/";
		}
		else{
			groupIds.push_back(tr("<< Local >>"));
		}

		elementName += address.GetComponentId();

		switch (GroupByCB->currentIndex()){
		case GM_NONE:
			groupIds.push_back("");
			break;

		case GM_PACKAGE:
			if (!address.GetPackageId().isEmpty()){
				groupIds.push_back(address.GetPackageId());
			}

			elementName = address.GetComponentId();
			break;

		default:
			{
				if (metaInfoPtr != NULL){
					icomp::CComponentMetaDescriptionEncoder encoder(metaInfoPtr->GetKeywords());

					groupIds = encoder.GetValues(currentKey);
				}
			}
		}

		QIcon icon;
		if (m_consistInfoCompPtr.IsValid()){
			icon = m_consistInfoCompPtr->GetComponentIcon(address);
		}

		if (icon.isNull() && (metaInfoPtr != NULL)){
			switch (metaInfoPtr->GetComponentType()){
				case icomp::IComponentStaticInfo::CT_COMPOSITE:
					icon = m_compositeComponentIcon.pixmap(QSize(64, 64), QIcon::Disabled);
					break;
				case icomp::IComponentStaticInfo::CT_REAL:
					icon = m_realComponentIcon.pixmap(QSize(64, 64), QIcon::Disabled);
					break;
			}
		}

		for (		QStringList::const_iterator iter = groupIds.begin();
					iter != groupIds.end();
					++iter){
			PackageComponentItem* itemPtr = new PackageComponentItem(
						*this,
						address,
						metaInfoPtr,
						icon);
			itemPtr->setText(0, elementName);

			RootInfo& rootInfo = EnsureRoot(iter->toLocal8Bit(), address, metaInfoPtr);
			I_ASSERT(rootInfo.itemPtr != NULL);

			rootInfo.itemPtr->addChild(itemPtr);
		}
	}

	for (		RootInfos::const_iterator toolTipIter = m_roots.begin();
				toolTipIter != m_roots.end();
				++toolTipIter){
		const RootInfo& info = toolTipIter.value();
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

	m_shownAddresses = addresses;
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

			QStringList categoryList = (encoder.GetMetaKeys());

			for (int index = 0; index < categoryList.count(); index++){
				categories.insert(categoryList[index]);
			}
		}
	}

	GroupByCB->addItems(categories.toList());
}


void CPackageOverviewComp::UpdateInterfaceList()
{
	InterfaceFilter knownInterfaces;
	if (m_envManagerCompPtr.IsValid()){
		icomp::IMetaInfoManager::ComponentAddresses addresses = m_envManagerCompPtr->GetComponentAddresses();

		for (		icomp::IMetaInfoManager::ComponentAddresses::const_iterator addressIter = addresses.begin();
					addressIter != addresses.end();
					++addressIter){
			const icomp::CComponentAddress& address = *addressIter;

			const icomp::IComponentStaticInfo* metaInfoPtr = m_envManagerCompPtr->GetComponentMetaInfo(address);
			if (metaInfoPtr != NULL){
				const icomp::IElementStaticInfo::Ids& interfaceNames = metaInfoPtr->GetMetaIds(icomp::IComponentStaticInfo::MGI_INTERFACES);

				for (		icomp::IElementStaticInfo::Ids::const_iterator interfaceIter = interfaceNames.begin();
							interfaceIter != interfaceNames.end();
							++interfaceIter){
					knownInterfaces.insert(*interfaceIter);
				}
			}
		}

		InterfaceCB->setMaxCount(0);
		InterfaceCB->setMaxCount(int(knownInterfaces.size()));

		InterfaceCB->addItem(tr("Any"));
		for (		InterfaceFilter::const_iterator iterfaceIter = knownInterfaces.begin();
					iterfaceIter != knownInterfaces.end();
					++iterfaceIter){
			const QByteArray& interfaceName = *iterfaceIter;

			InterfaceCB->addItem(interfaceName);
		}

		InterfaceLabel->setVisible(true);
		InterfaceCB->setVisible(true);
	}
	else{
		InterfaceLabel->setVisible(false);
		InterfaceCB->setVisible(false);
	}
}


void CPackageOverviewComp::UpdateAllLists()
{
	UpdateInterfaceList();

	UpdateComponentGroups();

	GenerateComponentTree(true);
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
		keywords << address.GetComponentId();

		if (metaInfoPtr != NULL){
			icomp::CComponentMetaDescriptionEncoder encoder(metaInfoPtr->GetKeywords());

			keywords << (encoder.GetValues());
		}

		if (FilterGB->isChecked()){
			bool isFilterMatched = true;

			if (metaInfoPtr != NULL){
				const icomp::IElementStaticInfo::Ids& interfaces = metaInfoPtr->GetMetaIds(icomp::IComponentStaticInfo::MGI_INTERFACES);
				for (		InterfaceFilter::const_iterator iterfaceIter = m_interfaceFilter.begin();
							iterfaceIter != m_interfaceFilter.end();
							++iterfaceIter){
					const QByteArray& filteredInterfaceName = *iterfaceIter;
					if (interfaces.find(filteredInterfaceName) == interfaces.end()){
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

	const IElementSelectionInfo* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		const icomp::IRegistry* registryPtr = objectPtr->GetSelectedRegistry();
		if (registryPtr != NULL){
			icomp::IRegistry::Ids embeddedIds = registryPtr->GetEmbeddedRegistryIds();
			for (		icomp::IRegistry::Ids::const_iterator embeddedIter = embeddedIds.begin();
						embeddedIter != embeddedIds.end();
						++embeddedIter){
				const QByteArray& embeddedId = *embeddedIter;

				const icomp::IRegistry* embeddedRegistryPtr = registryPtr->GetEmbeddedRegistry(embeddedId);
				if (embeddedRegistryPtr == NULL){
					continue;
				}

				QStringList keywords;
				keywords << embeddedId;

				icomp::CComponentMetaDescriptionEncoder encoder(embeddedRegistryPtr->GetKeywords());
				keywords << (encoder.GetValues());

				if (FilterGB->isChecked()){
					bool isFilterMatched = true;

					const icomp::IRegistry::ExportedInterfacesMap& exportedInterfaces = embeddedRegistryPtr->GetExportedInterfacesMap();

					for (		InterfaceFilter::const_iterator iterfaceIter = m_interfaceFilter.begin();
								iterfaceIter != m_interfaceFilter.end();
								++iterfaceIter){
						const QByteArray& filteredInterfaceName = *iterfaceIter;
						if (exportedInterfaces.find(filteredInterfaceName) == exportedInterfaces.end()){
							isFilterMatched = false;
							break;
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

				filteredComponentAdresses.insert(icomp::CComponentAddress("", embeddedId));
			}
		}
	}

	return filteredComponentAdresses;
}


// protected slots

void CPackageOverviewComp::on_FilterEdit_editingFinished()
{
	if (IsUpdateBlocked()){
		return;
	}

	UpdateBlocker blocker(this);

	QStringList keywordsFilter = FilterEdit->text().split(QChar(' '), QString::SkipEmptyParts,  Qt::CaseInsensitive);

	if (keywordsFilter != m_keywordsFilter){
		m_keywordsFilter = keywordsFilter;

		if (FilterGB->isChecked()){
			GenerateComponentTree(false);
		}
	}
}


void CPackageOverviewComp::on_GroupByCB_currentIndexChanged(int /*index*/)
{
	if (IsUpdateBlocked()){
		return;
	}

	UpdateBlocker blocker(this);

	GenerateComponentTree(true);
}


void CPackageOverviewComp::on_PackagesList_itemSelectionChanged()
{
	if (IsUpdateBlocked()){
		return;
	}

	UpdateBlocker blocker(this);

	QList<QTreeWidgetItem*> items = PackagesList->selectedItems();
	if (items.count() > 0){
		const PackageComponentItem* itemPtr = dynamic_cast<const PackageComponentItem*>(items.at(0));

		if ((itemPtr != NULL) && m_quickHelpViewerCompPtr.IsValid()){
			const icomp::CComponentAddress& address = itemPtr->GetAddress();

			m_quickHelpViewerCompPtr->ShowHelp(address.ToString(), &address);
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


void CPackageOverviewComp::on_PackagesList_itemDoubleClicked(QTreeWidgetItem* item, int /*column*/)
{
	PackageComponentItem* itemPtr = dynamic_cast<CPackageOverviewComp::PackageComponentItem*>(item);
	if (m_documentManagerCompPtr.IsValid() && m_envManagerCompPtr.IsValid() && (itemPtr != NULL)){
		const icomp::CComponentAddress& address = itemPtr->GetAddress();

		const icomp::IComponentStaticInfo* metaInfoPtr = m_envManagerCompPtr->GetComponentMetaInfo(address);

		if (metaInfoPtr != NULL &&(metaInfoPtr->GetComponentType() == icomp::IComponentStaticInfo::CT_COMPOSITE)){
			QDir packageDir(m_envManagerCompPtr->GetPackagePath(address.GetPackageId()));
			QString filePath = packageDir.absoluteFilePath(QString(address.GetComponentId()) + ".arx");

			m_documentManagerCompPtr->FileOpen(NULL, &filePath);
		}
	}
}


void CPackageOverviewComp::on_FilterGB_toggled(bool /*on*/)
{
	if (!m_interfaceFilter.isEmpty() || !m_keywordsFilter.isEmpty()){
		GenerateComponentTree(false);
	}
}


void CPackageOverviewComp::on_InterfaceCB_currentIndexChanged(int index)
{
	InterfaceFilter filter;

	if (index > 0){
		filter.insert(InterfaceCB->itemText(index).toLocal8Bit());
	}

	if (filter != m_interfaceFilter){
		m_interfaceFilter = filter;

		if (FilterGB->isChecked()){
			GenerateComponentTree(false);
		}
	}
}


void CPackageOverviewComp::OnReloadPackages()
{
	if (!m_envManagerCompPtr.IsValid()){
		return;
	}

	QString configFilePath;
	if (m_configFilePathCompPtr.IsValid()){
		configFilePath = m_configFilePathCompPtr->GetPath();
	}

	m_envManagerCompPtr->LoadPackages(iqt::CSystem::GetNormalizedPath(configFilePath));

	UpdateAllLists();
}


void CPackageOverviewComp::on_KeywordClearButton_clicked()
{
	FilterEdit->clear();

	on_FilterEdit_editingFinished();
}


void CPackageOverviewComp::on_InterfaceClearButton_clicked()
{
	InterfaceCB->setCurrentIndex(0);
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
	componentLabel.setText(address.ToString());
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


CPackageOverviewComp::RootInfo& CPackageOverviewComp::EnsureRoot(const QByteArray& path, const icomp::CComponentAddress& address, const icomp::IComponentStaticInfo* staticInfoPtr)
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
		if (path.isEmpty()){
			rootInfo.itemPtr = PackagesList->invisibleRootItem();
		}
		else{
			QByteArray groupName;
			QByteArray elementName = path;

			int slashPos = path.lastIndexOf('/');
			if (slashPos >= 0){
				groupName = path.left(slashPos);
				elementName = path.mid(slashPos + 1);
			}

			RootInfo& groupRoot = EnsureRoot(groupName, address, staticInfoPtr);
			I_ASSERT(groupRoot.itemPtr != NULL);

			QString packageDescription;
			if ((GroupByCB->currentIndex() == GM_PACKAGE) && m_envManagerCompPtr.IsValid()){
				const icomp::IComponentStaticInfo* packageInfoPtr = m_envManagerCompPtr->GetPackageMetaInfo(address.GetPackageId());
				if (packageInfoPtr != NULL){
					packageDescription = packageInfoPtr->GetDescription();
				}
			}

			rootInfo.itemPtr = new PackageItem(*this, packageDescription, QIcon());
			rootInfo.itemPtr->setText(0, elementName);
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

	QMouseEvent* mouseEventPtr = dynamic_cast<QMouseEvent*>(eventPtr);
	if (mouseEventPtr == NULL){
		return BaseClass::eventFilter(sourcePtr, eventPtr);
	}

	switch (eventPtr->type()){
		case QEvent::MouseMove:
		{
			bool doDrag = false;

			if (m_startDrag && sourceWidgetPtr != NULL){
				QPoint positionDifference = m_starDragPosition - mouseEventPtr->pos();

				double length = sqrt(double(positionDifference.x() * positionDifference.x() + positionDifference.y() * positionDifference.y()));
				if (length > 10){
					doDrag = true;
				}
			}

			if (doDrag){
				m_startDrag = false;
				QModelIndex componentModelIndex = itemViewPtr->indexAt(mouseEventPtr->pos());
				PackageComponentItem* selectedItemPtr = dynamic_cast<PackageComponentItem*>(reinterpret_cast<QTreeWidgetItem*>(componentModelIndex.internalPointer()));

				if (selectedItemPtr != NULL){
					QMimeData* mimeDataPtr = new QMimeData;

					icomp::CComponentAddress address = selectedItemPtr->GetAddress();
					iser::CXmlStringWriteArchive archive(NULL, false);
					if (address.Serialize(archive)){
						mimeDataPtr->setText(archive.GetString());

						QDrag *drag = new QDrag(sourceWidgetPtr);
						drag->setMimeData(mimeDataPtr);
						drag->setPixmap(CreateComponentDragPixmap(address));
						drag->setHotSpot(QPoint(drag->pixmap().width()/2, drag->pixmap().height()));
						drag->start(Qt::MoveAction);
					}
				}
			}
		}
		break;

		case QEvent::MouseButtonPress:
			if (mouseEventPtr->button() == Qt::LeftButton && sourceWidgetPtr != NULL){
				if (!m_startDrag){
					m_startDrag = true;

					m_starDragPosition = mouseEventPtr->pos();
				}
			}
			break;
	
		case QEvent::MouseButtonRelease:
			m_startDrag = false;
			break;
	
		default:
			break;
	}

	return BaseClass::eventFilter(sourcePtr, eventPtr);
}


// reimplemented (iqtgui::TGuiObserverWrap)

void CPackageOverviewComp::UpdateGui(int updateFlags)
{
	I_ASSERT(IsGuiCreated());

	const IElementSelectionInfo* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		icomp::IRegistry* registryPtr = objectPtr->GetSelectedRegistry();

		imod::IModel* registryModelPtr = dynamic_cast<imod::IModel*>(registryPtr);
		if (!m_registryObserver.IsModelAttached(registryModelPtr)){
			m_registryObserver.EnsureModelDetached();

			if (registryModelPtr != NULL){
				registryModelPtr->AttachObserver(&m_registryObserver);
			}
		}

		if ((updateFlags & IElementSelectionInfo::CF_SELECTION) == 0){	// ignore selection only changes
			GenerateComponentTree(false);
		}
	}
	else{
		m_registryObserver.EnsureModelDetached();

		GenerateComponentTree(false);
	}
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
}


void CPackageOverviewComp::OnGuiDestroyed()
{
	if (m_configFilePathModelCompPtr.IsValid() && m_configFilePathModelCompPtr->IsAttached(&m_configObserver)){
		m_configFilePathModelCompPtr->DetachObserver(&m_configObserver);
	}

	BaseClass::OnGuiDestroyed();
}


void CPackageOverviewComp::OnRetranslate()
{
	BaseClass::OnRetranslate();

	m_packagesCommand.SetVisuals(tr("&Packages"), tr("Packages"), tr("Menu for packages"));
	m_reloadCommand.SetVisuals(tr("&Reload All Packages"), tr("Reload"), tr("Reloads all packages form configuration file"), QIcon(":/Icons/Reload.svg"));
}


void CPackageOverviewComp::OnGuiRetranslate()
{
	UpdateBlocker blocker(this);

	int currentGroupIndex = GroupByCB->currentIndex();

	BaseClass::OnGuiRetranslate();

	// Work around a Qt bug: By retranslation of the UI the combo boxes will be reset and lose their previous model:
	GroupByCB->setCurrentIndex(currentGroupIndex);

	if (m_configFilePathModelCompPtr.IsValid()){
		if (		!m_configFilePathModelCompPtr->IsAttached(&m_configObserver) &&
					!m_configFilePathModelCompPtr->AttachObserver(&m_configObserver)){
			UpdateAllLists();
		}
	}
	else{
		UpdateAllLists();
	}
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
:	BaseClass(parent, (staticInfoPtr != NULL) ? staticInfoPtr->GetDescription() : QString(), icon),
	m_address(address)
{
	QString toolTip;
	if (!address.GetPackageId().isEmpty()){
		toolTip = tr("Component %1").arg(address.ToString());
	}
	else{
		toolTip = tr("Local composite component %2").arg(QString(address.GetComponentId()));
	}

	if (!m_description.isEmpty()){
		toolTip += "\n";
		toolTip += m_description;
	}
	setToolTip(0, toolTip);

	if ((staticInfoPtr != NULL) || address.GetPackageId().isEmpty()){
		setFlags(Qt::ItemIsDragEnabled | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
	}
	else{
		setFlags(Qt::ItemIsDragEnabled | Qt::ItemIsSelectable);
	}
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


// public methods of embedded class RegistryObserver

CPackageOverviewComp::RegistryObserver::RegistryObserver(CPackageOverviewComp* parentPtr)
:	m_parent(*parentPtr)
{
	I_ASSERT(parentPtr != NULL);
}


// protected methods of embedded class RegistryObserver

// reimplemented (imod::CSingleModelObserverBase)

void CPackageOverviewComp::RegistryObserver::OnUpdate(int updateFlags, istd::IPolymorphic* /*updateParamsPtr*/)
{
	if ((updateFlags & icomp::IRegistry::CF_EMBEDDED) != 0){
		if (m_parent.IsUpdateBlocked()){
			return;
		}

		UpdateBlocker blocker(&m_parent);

		m_parent.GenerateComponentTree(false);
	}
}


// public methods of embedded class RegistryObserver

CPackageOverviewComp::ConfigObserver::ConfigObserver(CPackageOverviewComp* parentPtr)
:	m_parent(*parentPtr)
{
	I_ASSERT(parentPtr != NULL);
}


// protected methods of embedded class RegistryObserver

// reimplemented (imod::CSingleModelObserverBase)

void CPackageOverviewComp::ConfigObserver::OnUpdate(int /*updateFlags*/, istd::IPolymorphic* /*updateParamsPtr*/)
{
	if (!m_parent.m_envManagerCompPtr.IsValid()){
		return;
	}

	QString configFilePath;
	if (m_parent.m_configFilePathCompPtr.IsValid()){
		configFilePath = m_parent.m_configFilePathCompPtr->GetPath();
	}

	if (configFilePath != m_parent.m_envManagerCompPtr->GetConfigFilePath()){
		m_parent.m_envManagerCompPtr->LoadPackages(iqt::CSystem::GetNormalizedPath(configFilePath));
		m_parent.UpdateAllLists();
	}
}


} // namespace icmpstr


