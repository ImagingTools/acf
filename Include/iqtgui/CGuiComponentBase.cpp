#include "iqtgui/CGuiComponentBase.h"


#include <QLayout>
#include <QEvent>
#include <QMetaMethod>

#include <iostream>


namespace iqtgui
{


CGuiComponentBase::CGuiComponentBase()
:	m_widgetPtr(NULL), m_isGuiShown(false)
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

			MakeAutoSlotConnection();

			OnGuiCreated();

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
		break;

	default:
		break;
	}

	return false;
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


} // namespace iqtgui

