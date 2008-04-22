#ifndef iqt_TDesignerGuiComponentBase_included
#define iqt_TDesignerGuiComponentBase_included


#include "qmetaobject.h"

#include "iqt/TGuiComponentBase.h"


namespace iqt
{


/**
	Base class for all Qt GUI componentes.
 */
template <class UI, class WidgetType = QWidget>
class TDesignerGuiCompBase: public TGuiComponentBase<WidgetType>, public UI
{
public:
	typedef TGuiComponentBase<WidgetType> BaseClass;

	// reimplemented (iqt::CGuiComponentBase)
	virtual QWidget* InitWidgetToParent(QWidget* parentPtr);

protected:
	// reimplemented (iqt::CGuiComponentBase)
	virtual void OnRetranslate();
};


// public methods

// reimplemented (iqt::CGuiComponentBase)

template <class UI, class WidgetType>
QWidget* TDesignerGuiCompBase<UI, WidgetType>::InitWidgetToParent(QWidget* parentPtr)
{
	I_ASSERT(!IsGuiCreated());

	QWidget* widgetPtr = BaseClass::InitWidgetToParent(parentPtr);

	setupUi(widgetPtr);

    const QMetaObject* mo = metaObject();
    I_ASSERT(mo != NULL);
    const QObjectList list = qFindChildren<QObject *>(widgetPtr, QString());
    for (int i = 0; i < mo->methodCount(); ++i) {
        const char *slot = mo->method(i).signature();
        I_ASSERT(slot);
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
        if (!foundIt)
            qWarning("QMetaObject::connectSlotsByName(): No matching signal for %s", slot);
    }

	return widgetPtr;
}


// protected methods

// reimplemented (iqt::CGuiComponentBase)

template <class UI, class WidgetType>
void TDesignerGuiCompBase<UI, WidgetType>::OnRetranslate()
{
	QWidget* widgetPtr = GetWidget();

	if (widgetPtr != NULL){
		retranslateUi(widgetPtr);
	}

	BaseClass::OnRetranslate();
}


} // namespace iqt


#endif //!iqt_TDesignerGuiComponentBase_included


