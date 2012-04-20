#include "iqtgui/CGuiComponentBase.h"


// Qt includes
#include <QtCore/QCoreApplication>
#include <QtCore/QEvent>
#include <QtCore/QMetaMethod>
#include <QtGui/QLayout>


namespace iqtgui
{


CGuiComponentBase::CGuiComponentBase()
:	m_widgetPtr(NULL),
	m_isGuiShown(false),
	m_languageChangeEventFilter(*this)
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
		m_widgetPtr = InitWidgetToParent(parentPtr);

		if (m_widgetPtr != NULL){
			if (parentPtr != NULL){
				QLayout* layoutPtr = parentPtr->layout();

				if (layoutPtr != NULL){
					layoutPtr->addWidget(m_widgetPtr);
				}
			}

			m_widgetPtr->installEventFilter(this);

			QCoreApplication* applicationPtr = QCoreApplication::instance();
			I_ASSERT(applicationPtr != NULL);
			if (applicationPtr != NULL){
				applicationPtr->installEventFilter(&m_languageChangeEventFilter);
			}

			MakeAutoSlotConnection();

			OnGuiCreated();
			OnGuiRetranslate();

			return true;
		}
	}

	return false;
}


bool CGuiComponentBase::DestroyGui()
{
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


// reimplemented (iqtgui::IVisualStatusProvider)

QIcon CGuiComponentBase::GetStatusIcon() const
{
	return m_defaultStatusIcon;
}


QString CGuiComponentBase::GetStatusText() const
{
	return *m_defaultStatusTextAttrPtr;
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


// reimplemented (QObject)

bool CGuiComponentBase::eventFilter(QObject* sourcePtr, QEvent* eventPtr)
{
	QWidget* widgetPtr = GetWidget();
	if ((sourcePtr != widgetPtr) || (widgetPtr == NULL)){
		return false;
	}

	switch (eventPtr->type()){
	case QEvent::Close:
		bool ignoreClose;
		OnTryClose(&ignoreClose);
		if (ignoreClose){
			eventPtr->ignore();

			return true;
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
		OnRetranslate();
		OnGuiRetranslate();
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

	QCoreApplication* applicationPtr = QCoreApplication::instance();
	if (applicationPtr != NULL){
		applicationPtr->installEventFilter(&m_languageChangeEventFilter);
	}

	if (m_defaultStatusIconPathAttrPtr.IsValid()){
		m_defaultStatusIcon = QIcon(*m_defaultStatusIconPathAttrPtr);
	}

	OnRetranslate();
}


// private methods

void CGuiComponentBase::MakeAutoSlotConnection()
{
    const QMetaObject *mo = metaObject();
    I_ASSERT(mo != NULL);
    const QObjectList list = qFindChildren<QObject *>(m_widgetPtr, QString());
    for (int i = 0; i < mo->methodCount(); ++i) {
        const char *slot = mo->method(i).signature();
        I_ASSERT(slot != NULL);
        if (slot[0] != 'o' || slot[1] != 'n' || slot[2] != '_')
            continue;
        bool foundIt = false;
        for(int j = 0; j < list.count(); ++j) {
            const QObject *co = list.at(j);
            QByteArray objName = co->objectName().toAscii();
            int len = objName.length();
            if (!len || qstrncmp(slot + 3, objName.data(), len) || slot[len+3] != '_')
                continue;
            const QMetaObject *smo = co->metaObject();
            int sigIndex = smo->indexOfMethod(slot + len + 4);
            if (sigIndex < 0) { // search for compatible signals
                int slotlen = qstrlen(slot + len + 4) - 1;
                for (int k = 0; k < co->metaObject()->methodCount(); ++k) {
                    if (smo->method(k).methodType() != QMetaMethod::Signal)
                        continue;

                    if (!qstrncmp(smo->method(k).signature(), slot + len + 4, slotlen)) {
                        sigIndex = k;
                        break;
                    }
                }
            }
            if (sigIndex < 0)
                continue;
            if (QMetaObject::connect(co, sigIndex, this, i)) {
                foundIt = true;
                break;
            }
        }
        if (foundIt) {
            // we found our slot, now skip all overloads
            while (mo->method(i + 1).attributes() & QMetaMethod::Cloned)
                  ++i;
        } else if (!(mo->method(i).attributes() & QMetaMethod::Cloned)) {
            qWarning("QMetaObject::connectSlotsByName: No matching signal for %s", slot);
        }
    }
}


// public methods of the embedded class LanguageChangeEventFilter

CGuiComponentBase::LanguageChangeEventFilter::LanguageChangeEventFilter(CGuiComponentBase& parent)
	:m_parent(parent)
{
}


// 	protected methods
	
// reimplemented (QObject)

bool CGuiComponentBase::LanguageChangeEventFilter::eventFilter(QObject* sourcePtr, QEvent* eventPtr)
{
	if ((eventPtr->type() == QEvent::LanguageChange) && (sourcePtr == &m_parent)){
		m_parent.OnRetranslate();

		if (m_parent.IsGuiCreated()){
			m_parent.OnGuiRetranslate();
		}
	}

	return BaseClass::eventFilter(sourcePtr, eventPtr);
}


} // namespace iqtgui


