#include <iqtgui/CGuiComponentBase.h>


// Qt includes
#include <QtCore/QCoreApplication>
#include <QtCore/QEvent>
#include <QtCore/QMetaMethod>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QLayout>
#else
#include <QtGui/QLayout>
#endif

// ACF includes
#include <istd/CChangeNotifier.h>


namespace iqtgui
{


CGuiComponentBase::CGuiComponentBase()
:	m_widgetPtr(NULL),
	m_isGuiShown(false)
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

		QByteArray componentId;
		QByteArray widgetId;

		if (m_widgetPtr != NULL){
			if (m_objectIdAttrPtr.IsValid() && m_widgetPtr->objectName().isEmpty()){
				widgetId = *m_objectIdAttrPtr;
			}
			else{
				widgetId = GetComponentId(GetComponentContext());
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
			}

			OnGuiCreated();
			OnGuiRetranslate();

			if (m_styleSheetPathAttrPtr.IsValid()){
				if (!iqtgui::SetStyleSheetFromFile(*m_widgetPtr, *m_styleSheetPathAttrPtr)){
					qDebug("Style sheet file could not be set: %s", (*m_styleSheetPathAttrPtr).toLocal8Bit().constData());
				}
			}

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

void CGuiComponentBase::OnGuiShown()
{
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
		m_visualStatus.m_statusIcon = QIcon(*m_defaultStatusIconPathAttrPtr);
	}

	m_visualStatus.m_statusText = *m_defaultStatusTextAttrPtr;

	OnRetranslate();
}


// private methods

void CGuiComponentBase::MakeAutoSlotConnection()
{
	const QMetaObject *mo = metaObject();
	Q_ASSERT(mo != NULL);

#if QT_VERSION < 0X050000
	const QObjectList list = qFindChildren<QObject *>(m_widgetPtr, QString());
	#define GetMethodSignature signature
#else
	const QObjectList list = m_widgetPtr->findChildren<QObject*>(QString());
	#define GetMethodSignature methodSignature
#endif

	for (int i = 0; i < mo->methodCount(); ++i){
		QByteArray methodSignature = mo->method(i).GetMethodSignature();

		const char* slot = methodSignature.constData();
		if (slot[0] != 'o' || slot[1] != 'n' || slot[2] != '_'){
			continue;
		}

		bool foundIt = false;
		for (int j = 0; j < list.count(); ++j){
			const QObject *co = list.at(j);
			QByteArray objName = co->objectName().toLatin1();
			int len = objName.length();
			if (!len || qstrncmp(slot + 3, objName.data(), len) || slot[len+3] != '_')
				continue;
			const QMetaObject *smo = co->metaObject();
			int sigIndex = smo->indexOfMethod(slot + len + 4);
			if (sigIndex < 0){ // search for compatible signals
				int slotlen = qstrlen(slot + len + 4) - 1;
				for (int k = 0; k < co->metaObject()->methodCount(); ++k){
					if (smo->method(k).methodType() != QMetaMethod::Signal)
						continue;

					if (!qstrncmp(smo->method(k).GetMethodSignature(), slot + len + 4, slotlen)){
						sigIndex = k;
						break;
					}
				}
			}
			if (sigIndex < 0)
				continue;
			if (QMetaObject::connect(co, sigIndex, this, i)){
				foundIt = true;
				break;
			}
		}
		if (foundIt){
			// we found our slot, now skip all overloads
			while (mo->method(i + 1).attributes() & QMetaMethod::Cloned){
				  ++i;
			}
		}
		else if (!(mo->method(i).attributes() & QMetaMethod::Cloned)){
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


