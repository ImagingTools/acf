#include "icmpstr/CComponentView.h"


// Qt includes
#include <QPainter>
#include <QFont>
#include <QFontMetrics>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsScene>
#include <QApplication>

#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"

#include "icomp/CCompositeComponentStaticInfo.h"

#include "icmpstr/CComponentConnector.h"
#include "icmpstr/CRegistryView.h"


namespace icmpstr
{


CComponentView::CComponentView(
			const CRegistryView* registryViewPtr,
			icomp::IRegistry* registryPtr,
			const icomp::IRegistry::ElementInfo* elementInfoPtr, 
			const std::string& componentName,
			QGraphicsItem* parent) 
:	QGraphicsRectItem(parent),
	m_registryView(*registryViewPtr),
	m_registry(*registryPtr),
	m_componentName(componentName),
	m_componentLabelFontSize(14),
	m_componentIdFontSize(10)
{
	I_ASSERT(registryViewPtr != NULL);
	I_ASSERT(registryPtr != NULL);

	CalcExportedInteraces();

	SetElementInfo(elementInfoPtr);

	setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
	setCursor(Qt::ArrowCursor);

	imod::IModel* registryModelPtr = dynamic_cast<imod::IModel*>(registryPtr);
	if (registryModelPtr != NULL){
		registryModelPtr->AttachObserver(this);
	}
}


CComponentView::~CComponentView()
{
	foreach(CComponentConnector* connector, m_connectors){
		connector->SetSourceComponent(NULL);
		connector->SetDestinationComponent(NULL);
	}

}


const icomp::IRegistry::ElementInfo& CComponentView::GetElementInfo() const
{
	I_ASSERT(m_elementInfoPtr != NULL);

	return *m_elementInfoPtr;
}


void CComponentView::SetElementInfo(const icomp::IRegistry::ElementInfo* elementInfoPtr)
{
	I_ASSERT(elementInfoPtr != NULL);

	m_elementInfoPtr = elementInfoPtr;

	if (m_elementInfoPtr->elementPtr.IsValid()){
		const icomp::IComponentStaticInfo& staticInfo = m_elementInfoPtr->elementPtr->GetComponentStaticInfo();
		setToolTip(iqt::GetQString(staticInfo.GetDescription()));
	}

	m_image = m_registryView.GetIcon(m_elementInfoPtr->address).pixmap(128, 128);

	setRect(CalculateRect());
}


const std::string& CComponentView::GetComponentName() const
{
	return m_componentName;
}


void CComponentView::AddConnector(CComponentConnector* connector)
{
	if (connector != NULL){
		m_connectors.push_back(connector);

		connector->Adjust();
	}

}


void CComponentView::RemoveConnector(const CComponentConnector* connector)
{
	QList<CComponentConnector*>::iterator found = qFind(m_connectors.begin(), m_connectors.end(), connector);
	if (found != m_connectors.end()){
		CComponentConnector* connector = *found;
		CComponentView* view1 =  connector->GetSourceComponent();
		CComponentView* view2 = connector->GetDestinationComponent();
		if (view1 == this){
			connector->SetSourceComponent(NULL);
		}
		if (view2 == this){
			connector->SetDestinationComponent(NULL);
		}
		m_connectors.erase(found);
	}
}


void CComponentView::RemoveAllConnectors()
{
	foreach (CComponentConnector* connectorPtr, m_connectors){
		I_ASSERT(connectorPtr != NULL);

		CComponentView* view1 =  connectorPtr->GetSourceComponent();
		CComponentView* view2 = connectorPtr->GetDestinationComponent();
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


QRectF CComponentView::GetInnerRect()const
{
	QRectF mainRect = rect();
	mainRect.adjust(0,0,-10,-10);

	return mainRect;
}


// reimplemented (icmpstr::IElementSelectionInfo)

icomp::IRegistry* CComponentView::GetSelectedRegistry() const
{
	return &m_registry;
}


iser::ISerializable* CComponentView::GetSelectedElement() const
{
	if (m_elementInfoPtr != NULL){
		return m_elementInfoPtr->elementPtr.GetPtr();
	}

	return NULL;
}


const std::string& CComponentView::GetSelectedElementName() const
{
	return m_componentName;
}


QIcon CComponentView::GetSelectedElementIcon() const
{
	if (m_elementInfoPtr != NULL){
		return m_registryView.GetIcon(m_elementInfoPtr->address);
	}

	return QIcon();
}


const icomp::CComponentAddress* CComponentView::GetSelectedElementAddress() const
{
	if (m_elementInfoPtr != NULL){
		return &m_elementInfoPtr->address;
	}

	return NULL;
}


// protected members
 
QRect CComponentView::CalculateRect() const
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

	QString componentPath = QString(m_elementInfoPtr->address.GetPackageId().c_str()) + QString(".") + m_elementInfoPtr->address.GetComponentId().c_str();
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


void CComponentView::CalcExportedInteraces()
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

void CComponentView::paint(QPainter* painter, const QStyleOptionGraphicsItem* /*option*/, QWidget* /*widget*/)
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

	if (m_elementInfoPtr->elementPtr.IsValid()){
		const icomp::IComponentStaticInfo& info = m_elementInfoPtr->elementPtr->GetComponentStaticInfo();
		if (dynamic_cast<const icomp::CCompositeComponentStaticInfo*>(&info) != NULL){
			QRectF compositeRect = mainRect;
			compositeRect.adjust(4, 4, -4, -4);
			painter->drawRect(compositeRect);
		}
	}

	if (!m_exportedInterfacesList.empty()){
		int minSideSize = int(istd::Min(mainRect.width(), mainRect.height()));
		painter->drawPixmap(
					int(mainRect.right() - minSideSize * 0.4),
					int(minSideSize * 0.1),
					int(minSideSize * 0.3),
					int(minSideSize * 0.3),
					QIcon(":/Resources/Icons/Export.png").pixmap(128, 128));
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

	painter->drawText(	mainRect, 
						Qt::AlignLeft | Qt::TextSingleLine, 
						QString(m_elementInfoPtr->address.GetPackageId().c_str()) + QString(".") + m_elementInfoPtr->address.GetComponentId().c_str());
	
	painter->restore();
}


QVariant CComponentView::itemChange(GraphicsItemChange change, const QVariant& value)
{
	switch(change){
	case QGraphicsItem::ItemSelectedChange:
		foreach (CComponentConnector* connector, m_connectors){
			connector->update();
		}
		emit selectionChanged(this, value.toBool());
		break;

	case QGraphicsItem::ItemPositionChange:
		{
			QSizeF size = CalculateRect().size();
			QPoint newPos = value.toPoint();
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

			emit positionChanged(this, value.toPoint());

			break;
		}

	default:
		break;
	}

	return QGraphicsRectItem::itemChange(change, value);
}


// reimplemented (imod::CSingleModelObserverBase)

void CComponentView::OnUpdate(int updateFlags, istd::IPolymorphic* updateParamsPtr)
{
	BaseObserverClass::OnUpdate(updateFlags, updateParamsPtr);

	if ((updateFlags & icomp::IRegistry::CF_COMPONENT_EXPORTED) != 0){
		CalcExportedInteraces();

		update();
	}
}


} // namespace icmpstr


