#include "icmpstr/CRegistryElementShape.h"


// Qt includes
#include <QPainter>
#include <QFont>
#include <QFontMetrics>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsScene>


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
	m_exportIcon(":/Icons/Export.svg"),
	m_noteAttachedIcon(":/Icons/NoteAttached.svg"),
	m_autoInitIcon(":/Icons/AutoInit.svg"),
	m_isConsistent(false),
	m_componentType(0)
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

	if (isSelected()){
		painterPtr->fillRect(shadowRect, QColor(10, 242, 126, 50));

		painterPtr->fillRect(mainRect, m_backgroundSelectedColor);
	}
	else{
		painterPtr->fillRect(shadowRect, QColor(0, 0, 0, 30));

		painterPtr->fillRect(mainRect, m_backgroundColor);
	}

	if (m_isConsistent){
		painterPtr->setPen(Qt::black);
	}
	else{
		painterPtr->setPen(Qt::red);
	}

	painterPtr->drawRect(mainRect);

	mainRect.adjust(SIDE_OFFSET, SIDE_OFFSET, -SIDE_OFFSET, -SIDE_OFFSET);

	// draw component icon:
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

	// draw composite indication rectangle:
	if (m_componentType == icomp::IComponentStaticInfo::CT_COMPOSITE){
		painterPtr->setPen(Qt::black);
		painterPtr->drawRect(mainRect);
		mainRect.adjust(SIDE_OFFSET, SIDE_OFFSET, -SIDE_OFFSET, -SIDE_OFFSET);
	}

	painterPtr->setPen(Qt::black);

	std::string componentName = objectPtr->GetName();

	// draw component name:
	const QFont& nameFont = m_registryView.GetElementNameFont();
	painterPtr->setFont(nameFont);
	painterPtr->drawText(mainRect, componentName.c_str(), Qt::AlignTop | Qt::AlignLeft);

	const QFont& detailFont = m_registryView.GetElementDetailFont();
	painterPtr->setFont(detailFont);
	painterPtr->drawText(
				mainRect,
				m_addressString,
				Qt::AlignBottom | Qt::AlignLeft);

	int iconsCount = 0;

	QFontMetrics nameFontInfo(m_registryView.GetElementNameFont());
	int attributeIconSize = nameFontInfo.height();

	// draw export interfaces icon:
	if (!m_exportedInterfacesList.empty()){
		QRectF iconRect(
					mainRect.right() - (attributeIconSize + SIDE_OFFSET) * ++iconsCount,
					mainRect.top(),
					attributeIconSize,
					attributeIconSize);
		m_exportIcon.paint(painterPtr, iconRect.toRect());
	}

	// draw element flags icon:
	I_DWORD elementFlags = objectPtr->GetElementFlags();
	if ((elementFlags & icomp::IRegistryElement::EF_AUTO_INSTANCE) != 0){
		QRectF iconRect(
					mainRect.right() - (attributeIconSize + SIDE_OFFSET) * ++iconsCount,
					mainRect.top(),
					attributeIconSize,
					attributeIconSize);

		m_autoInitIcon.paint(painterPtr, iconRect.toRect());
	}

	// draw 'note attached' icon:
	if (!objectPtr->GetNote().IsEmpty()){
		QRectF iconRect(
					mainRect.right() - (attributeIconSize + SIDE_OFFSET) * ++iconsCount,
					mainRect.top(),
					attributeIconSize,
					attributeIconSize);
		m_noteAttachedIcon.paint(painterPtr, iconRect.toRect());
	}
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
				m_exportedInterfacesList.push_back(iter->first.c_str());
			}
		}
	}
}


// reimplemented (iqt2d::TObjectShapeBase)

void CRegistryElementShape::UpdateGraphicsItem(const CVisualRegistryElement& element)
{
	setPos(iqt::GetQPointF(element.GetCenter()));

	CalcExportedInteraces(element);

	m_componentType = 0;

	QString toolTip;

	const icomp::CComponentAddress& address = element.GetAddress();
	const std::string& packageId = address.GetPackageId();
	if (!packageId.empty()){
		m_addressString = QString(packageId.c_str()) + QString("/") + address.GetComponentId().c_str();

		const IRegistryConsistInfo* constistInfoPtr = m_registryView.GetRegistryConsistInfo();
		if (constistInfoPtr != NULL){
			m_icon = constistInfoPtr->GetComponentIcon(address);
		}

		const icomp::IComponentEnvironmentManager* managerPtr = m_registryView.GetEnvironmentManager();
		if (managerPtr != NULL){
			const icomp::IComponentStaticInfo* metaInfoPtr = managerPtr->GetComponentMetaInfo(element.GetAddress());
			if (metaInfoPtr != NULL){
				toolTip = iqt::GetQString(metaInfoPtr->GetDescription());

				m_componentType = metaInfoPtr->GetComponentType();

				m_backgroundColor = Qt::white;
				m_backgroundSelectedColor = QColor(10, 242, 126, 255);
			}
			else{
				toolTip = tr("Package or component not found");

				m_backgroundColor = QColor(128, 128, 128, 255);
				m_backgroundSelectedColor = QColor(69, 185, 127, 255);
			}
		}
	}
	else{
		m_addressString = tr("Embedded: %1").arg(address.GetComponentId().c_str());
		m_icon = QIcon(":/Icons/EmbeddedComponent.svg");

		m_backgroundColor = QColor(200, 220, 255, 255);
		m_backgroundSelectedColor = QColor(10, 126, 242, 255);
	}

	const istd::CString& note = element.GetNote();
	if (!note.IsEmpty()){
		if (!toolTip.isEmpty()){
			toolTip += "\n\nAttached note:\n";
		}

		toolTip += iqt::GetQString(note).simplified();
	}

	setToolTip(toolTip);

	QFontMetrics nameFontInfo(m_registryView.GetElementNameFont());
	QFontMetrics detailFontInfo(m_registryView.GetElementDetailFont());

	const std::string& componentName = element.GetName();

	int titleWidth = nameFontInfo.width(componentName.c_str());
	int height = nameFontInfo.height() + detailFontInfo.height();

	int attributeIconSize = nameFontInfo.height();

	I_DWORD elementFlags = element.GetElementFlags();
	if (((elementFlags & icomp::IRegistryElement::EF_AUTO_INSTANCE) != 0)){
		titleWidth += attributeIconSize + SIDE_OFFSET;
	}

	if ((!m_exportedInterfacesList.empty())){
		titleWidth += attributeIconSize + SIDE_OFFSET;
	}

	if ((!element.GetNote().IsEmpty())){
		titleWidth += attributeIconSize + SIDE_OFFSET;
	}

	int width = istd::Max(titleWidth, detailFontInfo.width(m_addressString)) + SIDE_OFFSET * 2;

	width += SIDE_OFFSET * 2;
	height += SIDE_OFFSET * 2;

	if (m_componentType == icomp::IComponentStaticInfo::CT_COMPOSITE){
		width += SIDE_OFFSET * 2;
		height += SIDE_OFFSET * 2;
	}

	if (!m_icon.isNull()){
		width += height + SIDE_OFFSET;
	}

	double gridSize;
	const iqt2d::ISceneProvider* providerPtr = GetSceneProvider();
	if ((providerPtr != NULL) && providerPtr->GetSceneAlignment(gridSize)){
		width = ::ceil(width / gridSize) * gridSize;
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


} // namespace icmpstr


