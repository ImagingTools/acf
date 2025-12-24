#include <iqtgui/CGuiComponentBase.h>


// Qt includes
#include <QtWidgets/QApplication>
#include <QtCore/QEvent>
#include <QtCore/QMetaMethod>
#include <QtWidgets/QStyle>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QLayout>
#else
#include <QtGui/QLayout>
#endif

// ACF includes
#include <istd/CChangeNotifier.h>
#include <istd/CSystem.h>


namespace iqtgui
{


CGuiComponentBase::CGuiComponentBase()
:	m_widgetPtr(NULL),
	m_isGuiShown(false),
	m_hasPendingDesignChanges(false)
{
}


// reimplemented (iqtgui::IGuiObject)

bool CGuiComponentBase::IsGuiCreated() const
{
	return (m_widgetPtr != NULL);
}


bool CGuiComponentBase::CreateGui(QWidget* parentPtr)
{
	if (m_widgetPtr == NULL){
		m_widgetPtr = CreateQtWidget(parentPtr);
		Q_ASSERT(m_widgetPtr != NULL);

		m_widgetPtr->setWindowIcon(m_visualStatus.m_statusIcon);

		QString operationSystemName = istd::CSystem::GetOperationSystemName();
		m_widgetPtr->setProperty("OperatingSystem", operationSystemName);

		QByteArray componentId;
		QByteArray widgetId;

		if (m_widgetPtr != NULL){
			if (m_objectIdAttrPtr.IsValid() && m_widgetPtr->objectName().isEmpty()){
				widgetId = *m_objectIdAttrPtr;
			}
			else{
				widgetId = GetComponentId(GetComponentContext().get());
			}

			componentId = QByteArray("Component [") + widgetId + QByteArray("]");

			setObjectName(componentId);
			m_widgetPtr->setObjectName(widgetId);

			if (parentPtr != NULL){
				QLayout* layoutPtr = parentPtr->layout();

				if (layoutPtr != NULL){
					layoutPtr->addWidget(m_widgetPtr);
				}
			}

			m_widgetPtr->installEventFilter(this);

			EnableLocalization(true);

			MakeAutoSlotConnection();

			if (m_fixedWidthAttrPtr.IsValid()){
				m_widgetPtr->setMinimumWidth(*m_fixedWidthAttrPtr);
				m_widgetPtr->setSizePolicy(QSizePolicy::Maximum, m_widgetPtr->sizePolicy().verticalPolicy());
			}

			if (m_fixedHeightAttrPtr.IsValid()){
				m_widgetPtr->setMinimumHeight(*m_fixedHeightAttrPtr);
				m_widgetPtr->setSizePolicy(m_widgetPtr->sizePolicy().horizontalPolicy(), QSizePolicy::Maximum);

				if (*m_fixedHeightAttrPtr <= 0){
					m_widgetPtr->hide();
				}
			}

			OnGuiCreated();
			OnGuiRetranslate();
			OnGuiDesignChanged();

			return true;
		}
	}

	return false;
}


bool CGuiComponentBase::DestroyGui()
{
	EnableLocalization(false);

	if (m_widgetPtr != NULL){
		OnGuiDestroyed();

		m_widgetPtr->deleteLater();

		m_widgetPtr = NULL;
	}

	return true;
}


void CGuiComponentBase::OnTryClose(bool* ignoredPtr)
{
	if (ignoredPtr != NULL){
		*ignoredPtr = false;
	}
}


// protected methods

void CGuiComponentBase::OnGuiDesignChanged()
{
	if (!IsGuiCreated()){
		return;
	}

	if (m_styleSheetPathAttrPtr.IsValid()){
		Q_ASSERT(qApp != nullptr);

		QStyle* stylePtr = qApp->style();
		Q_ASSERT(stylePtr != nullptr);

		stylePtr->unpolish(qApp);

		QString styleSheetPath = GetStyleSheetPath(*m_styleSheetPathAttrPtr);
		if (!iqtgui::SetStyleSheetFromFile(m_widgetPtr, styleSheetPath)){
			qDebug("Style sheet file could not be set: %s", styleSheetPath.toLocal8Bit().constData());
		}

		stylePtr->polish(qApp);
	}
}


void CGuiComponentBase::OnGuiShown()
{
	if (m_hasPendingDesignChanges){
		OnGuiDesignChanged();

		m_hasPendingDesignChanges = false;
	}
}


void CGuiComponentBase::OnGuiHidden()
{
}


bool CGuiComponentBase::OnKeyPressed(QKeyEvent* /*event*/)
{
	return false;
}


void CGuiComponentBase::OnRetranslate()
{
}


void CGuiComponentBase::OnGuiRetranslate()
{
}


void CGuiComponentBase::OnGuiCreated()
{
}


void CGuiComponentBase::OnGuiDestroyed()
{
}


void CGuiComponentBase::SetStatusIcon(const QIcon& icon)
{
	istd::CChangeNotifier notifier(&m_visualStatus);

	m_visualStatus.m_statusIcon = icon;
}


void CGuiComponentBase::SetStatusText(const QString& text)
{
	if (text != m_visualStatus.m_statusText){
		istd::CChangeNotifier notifier(&m_visualStatus);

		m_visualStatus.m_statusText = text;
	}
}


// reimplemented (ibase::TLocalizableWrap)

void CGuiComponentBase::OnLanguageChanged()
{
	OnRetranslate();

	if (IsGuiCreated()){
		OnGuiRetranslate();
	}
}


// reimplemented (ibase::TDesignSchemaHandlerWrap)

void CGuiComponentBase::OnDesignSchemaChanged(const QByteArray& themeId)
{
	BaseClass::OnDesignSchemaChanged(themeId);

	// Update visual status:
	if (m_defaultStatusIconPathAttrPtr.IsValid()){
		istd::CChangeNotifier changeNotifier(&m_visualStatus);

		m_visualStatus.m_statusIcon = GetIcon(*m_defaultStatusIconPathAttrPtr);
	}

	if (IsGuiShown()){
		OnGuiDesignChanged();
	}
	else{
		m_hasPendingDesignChanges = true;
	}
}


// reimplemented (QObject)

bool CGuiComponentBase::eventFilter(QObject* sourcePtr, QEvent* eventPtr)
{
	QWidget* widgetPtr = GetWidget();
	if ((sourcePtr != widgetPtr) || (widgetPtr == NULL)){
		return false;
	}

	switch (eventPtr->type()){
	case QEvent::Close:
		{
			bool ignoreClose = false;
			OnTryClose(&ignoreClose);
			if (ignoreClose){
				eventPtr->ignore();

				return true;
			}
		}
		break;

	case QEvent::Show:
		if (!m_isGuiShown){
			m_isGuiShown = true;
			OnGuiShown();
		}
		break;

	case QEvent::Hide:
		if (m_isGuiShown){
			m_isGuiShown = false;
			OnGuiHidden();
		}
		break;

	case QEvent::LanguageChange:
		OnLanguageChanged();
		break;

	default:
		break;
	}

	return false;
}


// reimplemented (icomp::CComponentBase)

void CGuiComponentBase::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_defaultStatusIconPathAttrPtr.IsValid()){
		m_visualStatus.m_statusIcon = GetIcon(*m_defaultStatusIconPathAttrPtr);
	}

	m_visualStatus.m_statusText = *m_defaultStatusTextAttrPtr;

	OnRetranslate();

	EnableDesignHandler(true);
}


void CGuiComponentBase::OnComponentDestroyed()
{
	EnableDesignHandler(false);

	BaseClass::OnComponentDestroyed();
}


// private methods

void CGuiComponentBase::MakeAutoSlotConnection()
{
	const QMetaObject *metaObject = this->metaObject();
	Q_ASSERT(metaObject != NULL);

#if QT_VERSION < 0X050000
	const QObjectList list = qFindChildren<QObject *>(m_widgetPtr, QString());
	#define GetMethodSignature signature
#else
	const QObjectList list = m_widgetPtr->findChildren<QObject*>(QString());
	#define GetMethodSignature methodSignature
#endif

	for (int methodIndex = 0; methodIndex < metaObject->methodCount(); ++methodIndex){
		QByteArray methodSignature = metaObject->method(methodIndex).GetMethodSignature();

		const char* slot = methodSignature.constData();
		if (slot[0] != 'o' || slot[1] != 'n' || slot[2] != '_'){
			continue;
		}

		bool foundMatchingSignal = false;
		for (int childIndex = 0; childIndex < list.count(); ++childIndex){
			const QObject *childObject = list.at(childIndex);
			QByteArray objName = childObject->objectName().toLatin1();
			int nameLength = objName.length();
			if (!nameLength || qstrncmp(slot + 3, objName.data(), nameLength) || slot[nameLength+3] != '_')
				continue;
			const QMetaObject *signalMetaObject = childObject->metaObject();
			int signalIndex = signalMetaObject->indexOfMethod(slot + nameLength + 4);
			if (signalIndex < 0){ // search for compatible signals
				int slotNameLength = int(qstrlen(slot + nameLength + 4) - 1);
				for (int signalMethodIndex = 0; signalMethodIndex < childObject->metaObject()->methodCount(); ++signalMethodIndex){
					if (signalMetaObject->method(signalMethodIndex).methodType() != QMetaMethod::Signal)
						continue;

					if (!qstrncmp(signalMetaObject->method(signalMethodIndex).GetMethodSignature(), slot + nameLength + 4, slotNameLength)){
						signalIndex = signalMethodIndex;
						break;
					}
				}
			}
			if (signalIndex < 0)
				continue;
			if (QMetaObject::connect(childObject, signalIndex, this, methodIndex)){
				foundMatchingSignal = true;
				break;
			}
		}
		if (foundMatchingSignal){
			// we found our slot, now skip all overloads
			while (metaObject->method(methodIndex + 1).attributes() & QMetaMethod::Cloned){
				  ++methodIndex;
			}
		}
		else if (!(metaObject->method(methodIndex).attributes() & QMetaMethod::Cloned)){
			qWarning("QMetaObject::connectSlotsByName: No matching signal for %s", slot);
		}
	}
}


// public methods of embedded class VisualStatus

// reimplemented (iqtgui::IVisualStatus)

QIcon CGuiComponentBase::VisualStatus::GetStatusIcon() const
{
	return m_statusIcon;
}


QString CGuiComponentBase::VisualStatus::GetStatusText() const
{
	return m_statusText;
}


} // namespace iqtgui


