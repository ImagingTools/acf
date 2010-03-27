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

#include "iqt/CSignalBlocker.h"


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
			GenerateComponentTree();
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

	icomp::IMetaInfoManager::ComponentAddresses addresses = m_envManagerCompPtr->GetComponentAddresses();

	for (		icomp::IMetaInfoManager::ComponentAddresses::const_iterator addressIter = addresses.begin();
				addressIter != addresses.end();
				++addressIter){
		const icomp::CComponentAddress& address = *addressIter;
		const icomp::IComponentStaticInfo* metaInfoPtr = m_envManagerCompPtr->GetComponentMetaInfo(address);

		QStringList keywords;
		keywords << address.GetComponentId().c_str();

		if (metaInfoPtr != NULL){
			QString keywordString = iqt::GetQString(metaInfoPtr->GetKeywords());
			keywords << keywordString.split(QChar(' '), QString::SkipEmptyParts,  Qt::CaseInsensitive);
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

		int keywordIndex = 1;

		bool continueIterationFlag;
		do{
			continueIterationFlag = false;

			std::string groupId;
			std::string elementName = address.GetPackageId() + "/" + address.GetComponentId();
			switch (GroupByCB->currentIndex()){
			case GM_NONE:
				break;

			case GM_KEYWORD:
				if (keywordIndex < keywords.size()){
					groupId = keywords[keywordIndex++].toStdString();
					continueIterationFlag = true;
				}
				else{
					continue;
				}
				break;

			case GM_NAME:
				groupId = elementName;
				elementName = "Test";
				break;

			default:
				groupId = address.GetPackageId();
				elementName = address.GetComponentId();
				break;
			}

			istd::CString infoPath = m_envManagerCompPtr->GetComponentInfoPath(address);

			QIcon icon;
			if (m_consistInfoCompPtr.IsValid()){
				icon = m_consistInfoCompPtr->GetComponentIcon(address);
			}

			QDir packageDir(iqt::GetQString(infoPath));
			PackageComponentItem* itemPtr = new PackageComponentItem(
						*this,
						address,
						metaInfoPtr,
						icon);
			itemPtr->setText(0, elementName.c_str());

			int compType = metaInfoPtr->GetComponentType();
			RootInfo& rootInfo = EnsureRoot(groupId, compType);
			I_ASSERT(rootInfo.itemPtr != NULL);

			rootInfo.itemPtr->addChild(itemPtr);
		} while (continueIterationFlag);
	}

	for (		RootInfos::const_iterator toolTipIter = m_roots.begin();
				toolTipIter != m_roots.end();
				++toolTipIter){
		const RootInfo& info = toolTipIter->second;
		if (info.itemPtr != NULL){
			QString typeName;
			if (info.hasRealComponents){
				if (info.hasCompositeComponents){
					info.itemPtr->setIcon(0, m_mixedComponentIcon);
					typeName = "mixed";
				}
				else{
					info.itemPtr->setIcon(0, m_realComponentIcon);
					typeName = "real";
				}
			}
			else if (info.hasCompositeComponents){
				info.itemPtr->setIcon(0, m_compositeComponentIcon);
				typeName = "composite";
			}
			else{
				info.itemPtr->setIcon(0, QIcon());
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


// protected slots

void CPackageOverviewComp::on_FilterEdit_editingFinished()
{
	QStringList keywordsFilter = FilterEdit->text().split(QChar(' '), QString::SkipEmptyParts,  Qt::CaseInsensitive);

	if (keywordsFilter != m_keywordsFilter){
		m_keywordsFilter = keywordsFilter;

		if (FilterGB->isChecked()){
			GenerateComponentTree();
		}
	}
}


void CPackageOverviewComp::on_GroupByCB_currentIndexChanged(int /*index*/)
{
	GenerateComponentTree();
}


void CPackageOverviewComp::on_PackagesList_customContextMenuRequested(const QPoint& menuPoint)
{
	I_ASSERT(IsGuiCreated());

	QPoint localPoint;
	localPoint = PackagesList->viewport()->mapToGlobal(menuPoint);

	QAction actionCollapseAll(tr("&Collapse All"), this);
	connect(&actionCollapseAll, SIGNAL(triggered()), PackagesList, SLOT(collapseAll()), Qt::QueuedConnection);
	QAction actionExpandAll(tr("&Expand All"), this);
    connect(&actionExpandAll, SIGNAL(triggered()), PackagesList, SLOT(expandAll()), Qt::QueuedConnection);
	QList<QAction*> actionList;
	actionList << &actionCollapseAll << &actionExpandAll;

	QMenu::exec(actionList, localPoint);
}


void CPackageOverviewComp::on_FilterGB_toggled(bool /*on*/)
{
	if (!m_interfaceFilter.empty() || !m_keywordsFilter.isEmpty()){
		GenerateComponentTree();
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
			GenerateComponentTree();
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


CPackageOverviewComp::RootInfo& CPackageOverviewComp::EnsureRoot(const std::string& path, int compType)
{
	RootInfo& rootInfo = m_roots[path];

	switch (compType){
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

			RootInfo& groupRoot = EnsureRoot(groupName, compType);
			I_ASSERT(groupRoot.itemPtr != NULL);

			rootInfo.itemPtr = new QTreeWidgetItem();
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
	I_ASSERT(sourceWidgetPtr != NULL);

	switch (eventPtr->type()){
	case QEvent::MouseButtonPress:
		{
			QMouseEvent* mouseEvent = dynamic_cast<QMouseEvent*>(eventPtr);
			I_ASSERT(mouseEvent != NULL);

			if (mouseEvent->button() == Qt::LeftButton && PackagesList != NULL){
				PackageComponentItem* selectedItemPtr = NULL;

				QModelIndex componentModelIndex = PackagesList->indexAt(mouseEvent->pos());
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

	m_realComponentIcon = QIcon(QPixmap(":/Icons/RealComponent.svg"));
	m_compositeComponentIcon = QIcon(QPixmap(":/Icons/CompositeComponent.svg"));
	m_mixedComponentIcon = QIcon(QPixmap(":/Icons/MixedComponent.svg"));

	// set up the tree view:
	PackagesList->setColumnCount(1);
	PackagesList->setItemDelegate(new CItemDelegate());

	PackagesList->header()->setResizeMode(0, QHeaderView::Stretch);
	PackagesList->header()->hide();

	PackagesList->setIndentation(15);

	PackagesList->viewport()->installEventFilter(this);

	PackagesList->setContextMenuPolicy( Qt::CustomContextMenu);

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


	GenerateComponentTree();
}


// public methods of embedded class 

CPackageOverviewComp::PackageComponentItem::PackageComponentItem(
			CPackageOverviewComp& parent,
			const icomp::CComponentAddress& address,
			const icomp::IComponentStaticInfo* staticInfoPtr,
			const QIcon& icon)
:	m_parent(parent),
	m_address(address)
{
	QString toolTip = QObject::tr("Component %1.%2").arg(address.GetPackageId().c_str()).arg(address.GetComponentId().c_str());

	if (staticInfoPtr != NULL){
		setFlags(Qt::ItemIsDragEnabled | Qt::ItemIsEnabled | Qt::ItemIsSelectable);

		toolTip += "\n";
		toolTip += iqt::GetQString(staticInfoPtr->GetDescription());
	}
	else{
		setFlags(Qt::ItemIsDragEnabled | Qt::ItemIsSelectable);
	}

	setToolTip(0, toolTip);
	setIcon(0, icon);
}


} // namespace icmpstr


