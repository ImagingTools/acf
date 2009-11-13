#include "icmpstr/CRegistryElementShape.h"


// Qt includes
#include <QPainter>
#include <QFont>
#include <QFontMetrics>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsScene>
#include <QApplication>

#include "icomp/CCompositeComponentStaticInfo.h"

#include "icmpstr/CVisualRegistryScenographerComp.h"
#include "icmpstr/CRegistryElementShape.h"


namespace icmpstr
{


CRegistryElementShape::CRegistryElementShape(const CVisualRegistryScenographerComp* registryViewPtr, const iqt2d::ISceneProvider* providerPtr)
:	BaseClass(true, providerPtr),
	m_registryView(*registryViewPtr),
	m_registryObserver(this)
{
	I_ASSERT(registryViewPtr != NULL);
}


QRectF CRegistryElementShape::GetViewRect() const
{
	QRectF retVal = rect();
	retVal.moveCenter(pos());

	return retVal;
}


// reimplemented (QGraphicsItem)

QRectF CRegistryElementShape::boundingRect() const
{
	QRectF retVal = rect();

	retVal.adjust(0, 0, SHADOW_OFFSET, SHADOW_OFFSET);

	return retVal;
}


void CRegistryElementShape::paint(QPainter* painterPtr, const QStyleOptionGraphicsItem* /*stylePtr*/, QWidget* /*widgetPtr*/)
{
	const CVisualRegistryElement* objectPtr = GetObjectPtr();
	if (objectPtr == NULL){
		return;
	}

	painterPtr->setRenderHints(QPainter::Antialiasing, true);

	QRectF mainRect = rect();

	QRectF shadowRect = mainRect;
	shadowRect.adjust(SHADOW_OFFSET, SHADOW_OFFSET, SHADOW_OFFSET, SHADOW_OFFSET);
	
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

	const icomp::CComponentAddress& address = objectPtr->GetAddress();

	mainRect.adjust(SIDE_OFFSET, SIDE_OFFSET, -SIDE_OFFSET, -SIDE_OFFSET);

	const QIcon* iconPtr = m_registryView.GetIcon(address);
	if ((iconPtr != NULL) && (mainRect.width() > mainRect.height())){
		int minSideSize = int(istd::Min(mainRect.width(), mainRect.height()));

		QRectF iconRect(
					mainRect.left(),
					mainRect.top(),
					minSideSize,
					minSideSize);
		iconPtr->paint(painterPtr, iconRect.toRect());

		mainRect.adjust(minSideSize + SIDE_OFFSET, 0, 0, 0);
	}

	I_DWORD elementFlags = objectPtr->GetElementFlags();
	if ((elementFlags & icomp::IRegistryElement::EF_AUTO_INSTANCE) != 0){
		int minSideSize = int(istd::Min(mainRect.width(), mainRect.height()));

		QRectF iconRect(
					mainRect.right() - minSideSize * 0.5,
					mainRect.top() + minSideSize * 0.5,
					minSideSize * 0.5,
					minSideSize * 0.5);

		QIcon(":/Icons/AutoInit.svg").paint(painterPtr, iconRect.toRect());
	}

	const icomp::IComponentStaticInfo& info = objectPtr->GetComponentStaticInfo();
	if (dynamic_cast<const icomp::CCompositeComponentStaticInfo*>(&info) != NULL){
		painterPtr->drawRect(mainRect);

		mainRect.adjust(SIDE_OFFSET, SIDE_OFFSET, -SIDE_OFFSET, -SIDE_OFFSET);
	}

	if (!m_exportedInterfacesList.empty()){
		int minSideSize = int(istd::Min(mainRect.width(), mainRect.height()));

		QRectF iconRect(
					mainRect.right() - minSideSize * 0.5,
					mainRect.top(),
					minSideSize * 0.5,
					minSideSize * 0.5);
		QIcon(":/Icons/Export.svg").paint(painterPtr, iconRect.toRect());
	}

	painterPtr->setPen(Qt::black);

	std::string componentName = objectPtr->GetName();

	const QFont& nameFont = m_registryView.GetElementNameFont();
	painterPtr->setFont(nameFont);
	painterPtr->drawText(mainRect, componentName.c_str(), Qt::AlignTop | Qt::AlignLeft);

	const QFont& detailFont = m_registryView.GetElementDetailFont();
	painterPtr->setFont(detailFont);
	painterPtr->drawText(
				mainRect, 
				QString(address.GetPackageId().c_str()) + QString("/") + address.GetComponentId().c_str(),
				Qt::AlignBottom | Qt::AlignLeft);
}


// reimplemented (imod::IObserver)

bool CRegistryElementShape::OnAttached(imod::IModel* modelPtr)
{
	const CVisualRegistryElement* objectPtr = dynamic_cast<const CVisualRegistryElement*>(modelPtr);
	if (objectPtr == NULL){
		return false;
	}

	CalcExportedInteraces(*objectPtr);

	if (BaseClass::OnAttached(modelPtr)){
		I_ASSERT(objectPtr != NULL);

		const icomp::IComponentStaticInfo& staticInfo = objectPtr->GetComponentStaticInfo();
		setToolTip(iqt::GetQString(staticInfo.GetDescription()));

		return true;
	}

	return false;
}


// protected methods

void CRegistryElementShape::CalcExportedInteraces(const CVisualRegistryElement& element)
{
	m_exportedInterfacesList.clear();

	const icomp::IRegistry* registryPtr = element.GetRegistry();

	if (registryPtr != NULL){
		const std::string& componentName = element.GetName();

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


// public methods of embedded class RegistryObserver

CRegistryElementShape::RegistryObserver::RegistryObserver(CRegistryElementShape* parentPtr)
:	m_parent(*parentPtr)
{
	I_ASSERT(parentPtr != NULL);
}


// reimplemented (iqt2d::TObjectShapeBase)

void CRegistryElementShape::UpdateGraphicsItem(const CVisualRegistryElement& element)
{
	setPos(iqt::GetQPointF(element.GetCenter()));

	const QFont& nameFont = m_registryView.GetElementNameFont();
	QFontMetrics nameFontInfo(nameFont);

	QFontMetrics detailFontInfo(m_registryView.GetElementDetailFont());

	const icomp::CComponentAddress& address = element.GetAddress();
	const std::string& componentName = element.GetName();
	QString componentPath = QString(address.GetPackageId().c_str()) + QString("/") + address.GetComponentId().c_str();

	int width = qMax(nameFontInfo.width(componentName.c_str()), detailFontInfo.width(componentPath));
	int height = nameFontInfo.height() + detailFontInfo.height();

	I_DWORD elementFlags = element.GetElementFlags();
	if (		((elementFlags & icomp::IRegistryElement::EF_AUTO_INSTANCE) != 0) ||
				(!m_exportedInterfacesList.empty())){
		width += height * 0.5;
	}

	const QIcon* iconPtr = m_registryView.GetIcon(address);
	if (iconPtr != NULL){
		width += height + SIDE_OFFSET;
	}

	const icomp::IComponentStaticInfo& info = element.GetComponentStaticInfo();
	if (dynamic_cast<const icomp::CCompositeComponentStaticInfo*>(&info) != NULL){
		width += SIDE_OFFSET * 2;
		height += SIDE_OFFSET * 2;
	}

	width += SIDE_OFFSET * 2;
	height += SIDE_OFFSET * 2;

	double gridSize;
	const iqt2d::ISceneProvider* providerPtr = GetSceneProvider();
	if ((providerPtr != NULL) && providerPtr->GetSceneAlignment(gridSize)){
		width = ::ceil(width / (gridSize * 2)) * (gridSize * 2);
	}

	setRect(QRectF(-width * 0.5, -height * 0.5, width, height));

	Q_EMIT RectChanged(GetViewRect());
}


// reimplemented (TShapeBase)

void CRegistryElementShape::OnSelectionChanged(bool isSelected)
{
	BaseClass::OnSelectionChanged(isSelected);

	Q_EMIT SelectionChanged(isSelected);
}


// protected methods of embedded class

// reimplemented (imod::CSingleModelObserverBase)

void CRegistryElementShape::RegistryObserver::OnUpdate(int updateFlags, istd::IPolymorphic* updateParamsPtr)
{
	BaseClass::OnUpdate(updateFlags, updateParamsPtr);

	if ((updateFlags & icomp::IRegistry::CF_COMPONENT_EXPORTED) != 0){
		const CVisualRegistryElement* objectPtr = m_parent.GetObjectPtr();
		if (objectPtr == NULL){
			return;
		}

		m_parent.CalcExportedInteraces(*objectPtr);

		m_parent.update();
	}
}


} // namespace icmpstr


