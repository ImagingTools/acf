#include "icmpstr/CRegistryElementShape.h"


// Qt includes
#include <QPainter>
#include <QFont>
#include <QFontMetrics>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsScene>
#include <QApplication>
#include <QDir>


// ACF includes
#include "icmpstr/CVisualRegistryScenographerComp.h"
#include "icmpstr/CRegistryElementShape.h"
#include "icmpstr/IRegistryConsistInfo.h"


namespace icmpstr
{


CRegistryElementShape::CRegistryElementShape(
			const CVisualRegistryScenographerComp* registryViewPtr,
			const iqt2d::ISceneProvider* providerPtr)
:	BaseClass(true, providerPtr),
	m_registryView(*registryViewPtr),
	m_registryObserver(this),
	m_isConsistent(false)
{
	I_ASSERT(registryViewPtr != NULL);
}


QRectF CRegistryElementShape::GetViewRect() const
{
	QRectF retVal = rect();
	retVal.moveCenter(pos());

	return retVal;
}


void CRegistryElementShape::CheckConsistency()
{
	bool isConsistent = false;

	const CVisualRegistryElement* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		const icomp::IRegistry* registryPtr = objectPtr->GetRegistry();
		const IRegistryConsistInfo* infoPtr = m_registryView.GetRegistryConsistInfo();
		if (infoPtr != NULL){
			isConsistent = infoPtr->IsElementValid(
						objectPtr->GetName(),
						*registryPtr,
						true,
						false,
						NULL);
		}
	}

	if (isConsistent != m_isConsistent){
		m_isConsistent = isConsistent;
		update();
	}
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

	const icomp::IComponentStaticInfo* metaInfoPtr = NULL;
	const icomp::IComponentEnvironmentManager* managerPtr = m_registryView.GetEnvironmentManager();
	if (managerPtr != NULL){
		metaInfoPtr = managerPtr->GetComponentMetaInfo(objectPtr->GetAddress());
	}

	if (isSelected()){
		painterPtr->fillRect(shadowRect, QColor(10, 242, 126, 50));

		if (metaInfoPtr != NULL){
			painterPtr->fillRect(mainRect, QColor(10, 242, 126, 255));
		}
		else{
			painterPtr->fillRect(mainRect, QColor(69, 185, 127, 255));
		}
	}
	else{
		painterPtr->fillRect(shadowRect, QColor(0, 0, 0, 30));

		if (metaInfoPtr != NULL){
			painterPtr->fillRect(mainRect, Qt::white);
		}
		else{
			painterPtr->fillRect(mainRect, QColor(128, 128, 128, 255));
		}
	}

	if (m_isConsistent){
		painterPtr->setPen(Qt::black);
	}
	else{
		painterPtr->setPen(Qt::red);
	}

	painterPtr->drawRect(mainRect);

	mainRect.adjust(SIDE_OFFSET, SIDE_OFFSET, -SIDE_OFFSET, -SIDE_OFFSET);

	if (!m_icon.isNull() && (mainRect.width() > mainRect.height())){
		int minSideSize = int(istd::Min(mainRect.width(), mainRect.height()));

		QRectF iconRect(
					mainRect.left(),
					mainRect.top(),
					minSideSize,
					minSideSize);
		m_icon.paint(painterPtr, iconRect.toRect());

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

	if ((metaInfoPtr != NULL) && (metaInfoPtr->GetComponentType() == icomp::IComponentStaticInfo::CT_COMPOSITE)){
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

	mainRect.adjust(SIDE_OFFSET, 0, -SIDE_OFFSET, 0);

	const QFont& nameFont = m_registryView.GetElementNameFont();
	painterPtr->setFont(nameFont);
	painterPtr->drawText(mainRect, componentName.c_str(), Qt::AlignTop | Qt::AlignLeft);

	const QFont& detailFont = m_registryView.GetElementDetailFont();
	painterPtr->setFont(detailFont);
	painterPtr->drawText(
				mainRect,
				m_addressString,
				Qt::AlignBottom | Qt::AlignLeft);
}


void CRegistryElementShape::mouseMoveEvent(QGraphicsSceneMouseEvent* eventPtr)
{
	BaseClass::mouseMoveEvent(eventPtr);

	Q_EMIT RectChanged(GetViewRect());
}


void CRegistryElementShape::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* eventPtr)
{
	CVisualRegistryElement* objectPtr = GetObjectPtr();
	if (objectPtr != NULL){
		m_registryView.TryOpenComponent(*objectPtr);
	}

	// preserve event delegation:
	eventPtr->accept();

	BaseClass::mouseDoubleClickEvent(eventPtr);
}


// reimplemented (imod::IObserver)

bool CRegistryElementShape::OnAttached(imod::IModel* modelPtr)
{
	const CVisualRegistryElement* objectPtr = dynamic_cast<const CVisualRegistryElement*>(modelPtr);
	if (objectPtr == NULL){
		return false;
	}

	return BaseClass::OnAttached(modelPtr);
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

	CalcExportedInteraces(element);

	const icomp::CComponentAddress& address = element.GetAddress();
	m_addressString = QString(address.GetPackageId().c_str()) + QString("/") + address.GetComponentId().c_str();

	const IRegistryConsistInfo* constistInfoPtr = m_registryView.GetRegistryConsistInfo();
	if (constistInfoPtr != NULL){
		m_icon = constistInfoPtr->GetComponentIcon(address);
	}

	QFontMetrics nameFontInfo(m_registryView.GetElementNameFont());
	QFontMetrics detailFontInfo(m_registryView.GetElementDetailFont());

	const std::string& componentName = element.GetName();

	int width = istd::Max(nameFontInfo.width(componentName.c_str()), detailFontInfo.width(m_addressString)) + SIDE_OFFSET * 2;
	int height = nameFontInfo.height() + detailFontInfo.height();

	I_DWORD elementFlags = element.GetElementFlags();
	if (		((elementFlags & icomp::IRegistryElement::EF_AUTO_INSTANCE) != 0) ||
				(!m_exportedInterfacesList.empty())){
		width += height * 0.5;
	}

	if (!m_icon.isNull()){
		width += height + SIDE_OFFSET;
	}

	const icomp::IComponentEnvironmentManager* managerPtr = m_registryView.GetEnvironmentManager();
	if (managerPtr != NULL){
		const icomp::IComponentStaticInfo* metaInfoPtr = managerPtr->GetComponentMetaInfo(element.GetAddress());
		if (metaInfoPtr != NULL){
			setToolTip(iqt::GetQString(metaInfoPtr->GetDescription()));

			if (metaInfoPtr->GetComponentType() == icomp::IComponentStaticInfo::CT_COMPOSITE){
				width += SIDE_OFFSET * 2;
				height += SIDE_OFFSET * 2;
			}
		}
		else{
			setToolTip(tr("Package or component not found"));
		}
	}

	width += SIDE_OFFSET * 2;
	height += SIDE_OFFSET * 2;

	double gridSize;
	const iqt2d::ISceneProvider* providerPtr = GetSceneProvider();
	if ((providerPtr != NULL) && providerPtr->GetSceneAlignment(gridSize)){
		width = ::ceil(width / (gridSize * 2)) * (gridSize * 2);
		height = ::ceil(height / gridSize) * gridSize;
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


