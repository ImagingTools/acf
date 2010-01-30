#include "icmpstr/CComponentSceneItem.h"


// Qt includes
#include <QPainter>
#include <QFont>
#include <QFontMetrics>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsScene>
#include <QApplication>
#include <QDir>


// ACF includes
#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"

#include "icomp/CCompositeComponent.h"
#include "icomp/CCompositeComponentStaticInfo.h"

#include "iqt/iqt.h"

#include "icmpstr/CComponentConnector.h"
#include "icmpstr/CRegistryView.h"


namespace icmpstr
{


CComponentSceneItem::CComponentSceneItem(
			const CRegistryView* registryViewPtr,
			icomp::IRegistry* registryPtr,
			const icomp::IRegistry::ElementInfo* elementInfoPtr,
			const std::string& componentName,
			QGraphicsItem* parent) 
:	QGraphicsRectItem(parent),
	m_registryView(*registryViewPtr),
	m_registryObserver(this),
	m_registry(*registryPtr),
	m_componentName(componentName),
	m_componentLabelFontSize(14),
	m_componentIdFontSize(10)
{
	I_ASSERT(registryViewPtr != NULL);
	I_ASSERT(registryPtr != NULL);

	CalcExportedInteraces();

	SetElementInfo(elementInfoPtr);

	setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemSendsGeometryChanges);
	setCursor(Qt::ArrowCursor);

	imod::IModel* registryModelPtr = dynamic_cast<imod::IModel*>(registryPtr);
	if (registryModelPtr != NULL){
		registryModelPtr->AttachObserver(&m_registryObserver);
	}
}


CComponentSceneItem::~CComponentSceneItem()
{
	foreach(CComponentConnector* connector, m_connectors){
		connector->SetSourceComponent(NULL);
		connector->SetDestinationComponent(NULL);
	}

}


const icomp::IRegistry::ElementInfo& CComponentSceneItem::GetElementInfo() const
{
	I_ASSERT(m_elementInfoPtr != NULL);

	return *m_elementInfoPtr;
}


void CComponentSceneItem::SetElementInfo(const icomp::IRegistry::ElementInfo* elementInfoPtr)
{
	I_ASSERT(elementInfoPtr != NULL);

	m_elementInfoPtr = elementInfoPtr;

	const icomp::IComponentEnvironmentManager* managerPtr = m_registryView.GetPackagesManager();
	if (managerPtr != NULL){
		const icomp::IComponentStaticInfo* metaInfoPtr = managerPtr->GetComponentMetaInfo(elementInfoPtr->address);
		if (metaInfoPtr != NULL){
			setToolTip(iqt::GetQString(metaInfoPtr->GetDescription()));
		}
	}

	m_image = m_registryView.GetIcon(m_elementInfoPtr->address).pixmap(128, 128);

	setRect(CalculateRect());
}


const std::string& CComponentSceneItem::GetComponentName() const
{
	return m_componentName;
}


void CComponentSceneItem::AddConnector(CComponentConnector* connector)
{
	if (connector != NULL){
		m_connectors.push_back(connector);

		connector->Adjust();
	}

}


void CComponentSceneItem::RemoveConnector(const CComponentConnector* connector)
{
	QList<CComponentConnector*>::iterator found = qFind(m_connectors.begin(), m_connectors.end(), connector);
	if (found != m_connectors.end()){
		CComponentConnector* connector = *found;
		CComponentSceneItem* view1 =  connector->GetSourceComponent();
		CComponentSceneItem* view2 = connector->GetDestinationComponent();
		if (view1 == this){
			connector->SetSourceComponent(NULL);
		}
		if (view2 == this){
			connector->SetDestinationComponent(NULL);
		}
		m_connectors.erase(found);
	}
}


void CComponentSceneItem::RemoveAllConnectors()
{
	foreach (CComponentConnector* connectorPtr, m_connectors){
		I_ASSERT(connectorPtr != NULL);

		CComponentSceneItem* view1 =  connectorPtr->GetSourceComponent();
		CComponentSceneItem* view2 = connectorPtr->GetDestinationComponent();
		if (view1 == this){
			connectorPtr->SetSourceComponent(NULL);
		}
		if (view2 == this){
			connectorPtr->SetDestinationComponent(NULL);
		}

		delete connectorPtr;
	}

	m_connectors.clear();
}


QRectF CComponentSceneItem::GetInnerRect()const
{
	QRectF mainRect = rect();
	mainRect.adjust(0,0,-10,-10);

	return mainRect;
}


// protected members
 
QRect CComponentSceneItem::CalculateRect() const
{
	I_ASSERT(m_elementInfoPtr != NULL);

	const int margin = 10;

	QFont labelFont(qApp->font());
	labelFont.setBold(true);
	labelFont.setPixelSize(m_componentLabelFontSize);
	QFontMetrics fontInfo(labelFont);

	QFont labelFont2(qApp->font());
	labelFont.setPixelSize(m_componentIdFontSize);
	QFontMetrics fontInfo2(labelFont2);

	int width = margin * 2;
	int height = margin * 2;

	QString componentPath = QString(m_elementInfoPtr->address.GetPackageId().c_str()) + QString("/") + m_elementInfoPtr->address.GetComponentId().c_str();
	width += qMax(fontInfo.width(iqt::GetQString(m_componentName)), fontInfo2.width(componentPath));

	height += fontInfo.height();
	m_componentLabelHeight = fontInfo.height();

	const int verticalSpace = 10;
	m_componentIdHeight = fontInfo2.height();
	height += (m_componentIdHeight + verticalSpace);

	double gridSize = m_registryView.GetGrid();

	if (!m_image.isNull()){
		width += height;
	}

	const int shadowOffset = 10;
	width += shadowOffset;
	height += shadowOffset;

	width = int(::ceil(width / gridSize) * gridSize);
	height = int(::ceil(height / gridSize) * gridSize);

	return QRect(int(pos().x()), int(pos().y()), width, height);
}


void CComponentSceneItem::CalcExportedInteraces()
{
	m_exportedInterfacesList.clear();

	const icomp::IRegistry::ExportedInterfacesMap& interfacesMap = m_registry.GetExportedInterfacesMap();
	for (		icomp::IRegistry::ExportedInterfacesMap::const_iterator iter = interfacesMap.begin();
				iter != interfacesMap.end();
				++iter){
		if (iter->second == m_componentName){
			m_exportedInterfacesList.push_back(iqt::GetQString(iter->first.GetName()));
		}
	}
}


// reimplemented (QGraphicsRectItem)

void CComponentSceneItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* /*option*/, QWidget* /*widget*/)
{
	I_ASSERT(m_elementInfoPtr != NULL);

	painter->setRenderHints(QPainter::Antialiasing, true);

	QRectF mainRect = rect();

	mainRect.adjust(0, 0, -10, -10);

	QRectF shadowRect = mainRect;
	shadowRect.adjust(10, 10, 10, 10);
	
	if (isSelected()){
		painter->fillRect(shadowRect, QColor(10, 242, 126, 50));
		painter->setPen(Qt::blue);
		painter->fillRect(mainRect, QColor(10, 242, 126, 255));
	}
	else{
		painter->fillRect(shadowRect, QColor(0, 0, 0, 30));
		painter->fillRect(mainRect, Qt::white);
		painter->setPen(Qt::black);
	}

	painter->drawRect(mainRect);

	if (!m_image.isNull() && (mainRect.width() > mainRect.height())){
		mainRect.adjust(mainRect.height(), 0, 0, 0);

		painter->drawPixmap(
					int(mainRect.height() * 0.05),
					int(mainRect.height() * 0.05),
					int(mainRect.height() * 0.9),
					int(mainRect.height() * 0.9),
					m_image);
	}

	int minSideSize = int(istd::Min(mainRect.width(), mainRect.height()));

	const icomp::IComponentEnvironmentManager* managerPtr = m_registryView.GetPackagesManager();
	if (managerPtr != NULL){
		const icomp::IComponentStaticInfo* infoPtr = managerPtr->GetComponentMetaInfo(m_elementInfoPtr->address);
		if (infoPtr->GetComponentType() == icomp::IComponentStaticInfo::CT_COMPOSITE){
			QRectF compositeRect = mainRect;
			compositeRect.adjust(4, 4, -4, -4);
			painter->drawRect(compositeRect);
		}
	}

	if (m_elementInfoPtr->elementPtr.IsValid()){
		I_DWORD elementFlags = m_elementInfoPtr->elementPtr->GetElementFlags();
		if ((elementFlags & icomp::IRegistryElement::EF_AUTO_INSTANCE) != 0){
			QRectF iconRect(
						mainRect.right() - minSideSize * 0.5,
						minSideSize * 0.5,
						minSideSize * 0.4,
						minSideSize * 0.4);
			QRectF worldIconRect = painter->worldTransform().mapRect(iconRect);
			QSize iconSize(istd::Min(256, int(worldIconRect.width())), istd::Min(256, int(worldIconRect.height())));
			painter->drawPixmap(
						iconRect,
						QIcon(":/Icons/AutoInit.svg").pixmap(iconSize),
						QRectF(0, 0, iconSize.width(), iconSize.height()));
		}
	}

	if (!m_exportedInterfacesList.empty()){
		QRectF iconRect(
					mainRect.right() - minSideSize * 0.5,
					minSideSize * 0.1,
					minSideSize * 0.4,
					minSideSize * 0.4);
		QRectF worldIconRect = painter->worldTransform().mapRect(iconRect);
		QSize iconSize(istd::Min(256, int(worldIconRect.width())), istd::Min(256, int(worldIconRect.height())));
		painter->drawPixmap(
					iconRect,
					QIcon(":/Icons/Export.svg").pixmap(iconSize),
					QRectF(0, 0, iconSize.width(), iconSize.height()));
	}

	mainRect.adjust(10, 10, 0, 0);
	mainRect.setHeight(m_componentLabelHeight);
	painter->save();
	painter->setPen(Qt::black);

	QFont labelFont(qApp->font());
	labelFont.setBold(true);
	labelFont.setPixelSize(m_componentLabelFontSize);
	painter->setFont(labelFont);
	painter->drawText(mainRect, Qt::AlignLeft | Qt::TextSingleLine, m_componentName.c_str());

	mainRect.moveTop(mainRect.bottom() + 10);
	mainRect.setHeight(m_componentIdHeight);
	QFont labelFont2(qApp->font());
	labelFont2.setPixelSize(m_componentIdFontSize);
	painter->setFont(labelFont2);

	painter->drawText(
				mainRect, 
				Qt::AlignLeft | Qt::TextSingleLine, 
				QString(m_elementInfoPtr->address.GetPackageId().c_str()) + QString("/") + m_elementInfoPtr->address.GetComponentId().c_str());
	
	painter->restore();
}


QVariant CComponentSceneItem::itemChange(GraphicsItemChange change, const QVariant& value)
{
	switch(change){
	case QGraphicsItem::ItemSelectedChange:
		foreach (CComponentConnector* connector, m_connectors){
			connector->update();
		}
		Q_EMIT selectionChanged(this, value.toBool());
		break;

	case QGraphicsItem::ItemPositionChange:
		{
			QSizeF size = CalculateRect().size();
			QPointF newPos = value.toPointF();
/*			double gridSize = m_registryView.GetGrid();
			newPos.setX(int(::ceil((newPos.x() + size.width() * 0.5) / gridSize + 0.5) * gridSize - size.width() * 0.5));
			newPos.setY(int(::ceil((newPos.y() + size.height() * 0.5) / gridSize + 0.5) * gridSize - size.height() * 0.5));
*/
			return QVariant(newPos);
		}
	
	case QGraphicsItem::ItemPositionHasChanged:
		{
			foreach (CComponentConnector* connector, m_connectors){
				connector->Adjust();
			}

			Q_EMIT positionChanged(this, value.toPointF());

			break;
		}

	default:
		break;
	}

	return QGraphicsRectItem::itemChange(change, value);
}


void CComponentSceneItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* I_IF_DEBUG(eventPtr))
{
	I_ASSERT(eventPtr != NULL);
	I_ASSERT(m_elementInfoPtr != NULL);
	I_ASSERT(m_elementInfoPtr->elementPtr.IsValid());

	const icomp::IRegistriesManager* managerPtr = m_registryView.GetPackagesManager();
	idoc::IMainWindowCommands* mainWindowPtr = m_registryView.GetMainWindowCommands();
	if ((managerPtr != NULL) && (mainWindowPtr != NULL)){
		QDir packageDir(iqt::GetQString(managerPtr->GetPackageDirPath(m_elementInfoPtr->address.GetPackageId())));
		QString filePath = packageDir.absoluteFilePath((m_elementInfoPtr->address.GetComponentId() + ".arx").c_str());

		mainWindowPtr->OpenFile(iqt::GetCString(filePath));
	}
}


// public methods of embedded class RegistryObserver

CComponentSceneItem::RegistryObserver::RegistryObserver(CComponentSceneItem* parentPtr)
:	m_parent(*parentPtr)
{
	I_ASSERT(parentPtr != NULL);
}


// protected methods of embedded class

// reimplemented (imod::CSingleModelObserverBase)

void CComponentSceneItem::RegistryObserver::OnUpdate(int updateFlags, istd::IPolymorphic* updateParamsPtr)
{
	BaseObserverClass::OnUpdate(updateFlags, updateParamsPtr);

	if ((updateFlags & icomp::IRegistry::CF_COMPONENT_EXPORTED) != 0){
		m_parent.CalcExportedInteraces();

		m_parent.update();
	}
}


} // namespace icmpstr


