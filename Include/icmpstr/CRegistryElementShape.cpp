#include "icmpstr/CRegistryElementShape.h"


// Qt includes
#include <QPainter>
#include <QFont>
#include <QFontMetrics>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsScene>
#include <QApplication>

#include "icomp/CCompositeComponentStaticInfo.h"


namespace icmpstr
{


CRegistryElementShape::CRegistryElementShape(const CRegistryView* registryViewPtr)
:	m_registryView(*registryViewPtr),
	m_registryObserver(this)
{
	I_ASSERT(registryViewPtr != NULL);
}


// reimplemented (QGraphicsItem)

QRectF CRegistryElementShape::boundingRect() const
{
	return m_boundingBox;
}


void CRegistryElementShape::paint(QPainter* painterPtr, const QStyleOptionGraphicsItem* /*stylePtr*/, QWidget* /*widgetPtr*/)
{
	const CGeometricalRegistryElement* objectPtr = GetObjectPtr();
	if (objectPtr == NULL){
		return;
	}

	painterPtr->setRenderHints(QPainter::Antialiasing, true);

	QRectF mainRect = m_boundingBox;

	mainRect.adjust(0, 0, -10, -10);

	QRectF shadowRect = mainRect;
	shadowRect.adjust(10, 10, 10, 10);
	
	if (isSelected()){
		painterPtr->fillRect(shadowRect, QColor(10, 242, 126, 50));
		painterPtr->setPen(Qt::blue);
		painterPtr->fillRect(mainRect, QColor(10, 242, 126, 255));
	}
	else{
		painterPtr->fillRect(shadowRect, QColor(0, 0, 0, 30));
		painterPtr->fillRect(mainRect, Qt::white);
		painterPtr->setPen(Qt::black);
	}

	painterPtr->drawRect(mainRect);

	if (!m_image.isNull() && (mainRect.width() > mainRect.height())){
		mainRect.adjust(mainRect.height(), 0, 0, 0);

		painterPtr->drawPixmap(
					int(mainRect.height() * 0.05),
					int(mainRect.height() * 0.05),
					int(mainRect.height() * 0.9),
					int(mainRect.height() * 0.9),
					m_image);
	}

	const icomp::IComponentStaticInfo& info = objectPtr->GetComponentStaticInfo();
	if (dynamic_cast<const icomp::CCompositeComponentStaticInfo*>(&info) != NULL){
		QRectF compositeRect = mainRect;
		compositeRect.adjust(4, 4, -4, -4);
		painterPtr->drawRect(compositeRect);
	}

	if (!m_exportedInterfacesList.empty()){
		int minSideSize = int(istd::Min(mainRect.width(), mainRect.height()));
		painterPtr->drawPixmap(
					int(mainRect.right() - minSideSize * 0.4),
					int(minSideSize * 0.1),
					int(minSideSize * 0.3),
					int(minSideSize * 0.3),
					QIcon(":/Icons/Export").pixmap(128, 128));
	}

	mainRect.adjust(10, 10, 0, 0);
	mainRect.setHeight(m_componentLabelHeight);
	painterPtr->save();
	painterPtr->setPen(Qt::black);

	std::string componentName = objectPtr->GetName();

	QFont labelFont(qApp->font());
	labelFont.setBold(true);
	labelFont.setPixelSize(labelFont.pixelSize() * 2);
	painterPtr->setFont(labelFont);
	painterPtr->drawText(mainRect, Qt::AlignLeft | Qt::TextSingleLine, componentName.c_str());

	mainRect.moveTop(mainRect.bottom() + 10);
	mainRect.setHeight(m_componentIdHeight);
	painterPtr->setFont(qApp->font());

	const icomp::CComponentAddress& adress = objectPtr->GetAddress();

	painterPtr->drawText(
				mainRect, 
				Qt::AlignLeft | Qt::TextSingleLine, 
				QString(adress.GetPackageId().c_str()) + QString("/") + adress.GetComponentId().c_str());
	
	painterPtr->restore();
}


bool CRegistryElementShape::contains(const QPointF& point) const
{
	return m_boundingBox.contains(point);
}


// reimplemented (imod::IObserver)

bool CRegistryElementShape::OnAttached(imod::IModel* modelPtr)
{
	if (BaseClass::OnAttached(modelPtr)){
		const CGeometricalRegistryElement* objectPtr = GetObjectPtr();
		I_ASSERT(objectPtr == NULL);

		const icomp::IComponentStaticInfo& staticInfo = objectPtr->GetComponentStaticInfo();
		setToolTip(iqt::GetQString(staticInfo.GetDescription()));

		m_image = m_registryView.GetIcon(objectPtr->GetAddress()).pixmap(128, 128);

		CalcExportedInteraces();

		return true;
	}

	return false;
}


void CRegistryElementShape::AfterUpdate(imod::IModel* modelPtr, int updateFlags, istd::IPolymorphic* updateParamsPtr)
{
	BaseClass::AfterUpdate(modelPtr, updateFlags, updateParamsPtr);

	const CGeometricalRegistryElement* objectPtr = GetObjectPtr();
	if (objectPtr == NULL){
		return;
	}

	const int margin = 10;

	QFont labelFont(qApp->font());
	labelFont.setBold(true);
	labelFont.setPixelSize(labelFont.pixelSize() * 2);
	QFontMetrics fontInfo(labelFont);

	QFontMetrics fontInfo2(qApp->font());

	int width = margin * 2;
	int height = margin * 2;

	const icomp::CComponentAddress& address = objectPtr->GetAddress();
	const std::string& componentName = objectPtr->GetName();
	QString componentPath = QString(address.GetPackageId().c_str()) + QString("/") + address.GetComponentId().c_str();
	width += qMax(fontInfo.width(componentName.c_str()), fontInfo2.width(componentPath));

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

	m_boundingBox = QRect(int(pos().x()), int(pos().y()), width, height);

	update();
}


// protected methods

void CRegistryElementShape::CalcExportedInteraces()
{
	m_exportedInterfacesList.clear();

	const CGeometricalRegistryElement* objectPtr = GetObjectPtr();
	if (objectPtr == NULL){
		return;
	}

	const icomp::IRegistry* registryPtr = objectPtr->GetRegistry();

	if (registryPtr != NULL){
		const std::string& componentName = objectPtr->GetName();

		const icomp::IRegistry::ExportedInterfacesMap& interfacesMap = registryPtr->GetExportedInterfacesMap();
		for (		icomp::IRegistry::ExportedInterfacesMap::const_iterator iter = interfacesMap.begin();
					iter != interfacesMap.end();
					++iter){
			if (iter->second == componentName){
				m_exportedInterfacesList.push_back(iqt::GetQString(iter->first.GetName()));
			}
		}
	}
}


// reimplemented (QGraphicsItem)

void CRegistryElementShape::mousePressEvent(QGraphicsSceneMouseEvent* /*eventPtr*/)
{
}


void CRegistryElementShape::mouseMoveEvent(QGraphicsSceneMouseEvent* /*eventPtr*/)
{
}


void CRegistryElementShape::mouseReleaseEvent(QGraphicsSceneMouseEvent* /*eventPtr*/)
{
}


// public methods of embedded class RegistryObserver

CRegistryElementShape::RegistryObserver::RegistryObserver(CRegistryElementShape* parentPtr)
:	m_parent(*parentPtr)
{
	I_ASSERT(parentPtr != NULL);
}


// protected methods of embedded class

// reimplemented (imod::CSingleModelObserverBase)

void CRegistryElementShape::RegistryObserver::OnUpdate(int updateFlags, istd::IPolymorphic* updateParamsPtr)
{
	BaseClass::OnUpdate(updateFlags, updateParamsPtr);

	if ((updateFlags & icomp::IRegistry::CF_COMPONENT_EXPORTED) != 0){
		m_parent.CalcExportedInteraces();

		m_parent.update();
	}
}


} // namespace icmpstr


