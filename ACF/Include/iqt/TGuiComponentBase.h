#ifndef iqt_TGuiComponentBase_included
#define iqt_TGuiComponentBase_included


#include <QWidget>
#include "qmetaobject.h"

#include "iqt/CGuiComponentBase.h"


namespace iqt
{


/**	Base class for all Qt GUI componentes.
 */
template <class UI, class WidgetType = QWidget>
class TGuiComponentBase: public QObject, public CGuiComponentBase, public UI
{
public:
	typedef CGuiComponentBase BaseClass;

	// reimplemented (iqt::CGuiComponentBase)
	virtual QWidget* CreateMainWidget(QWidget* parentPtr) const;

protected:
	// reimplemented (iqt::CGuiComponentBase)
	virtual void OnRetranslate();
};


// reimplemented (iqt::CGuiComponentBase)

template <class UI, class WidgetType>
QWidget* TGuiComponentBase<UI, WidgetType>::CreateMainWidget(QWidget* parentPtr) const
{
	WidgetType* widgetPtr = new WidgetType(parentPtr);

	setupUi(widgetPtr);

    const QMetaObject* mo = metaObject();
    Q_ASSERT(mo);
    const QObjectList list = qFindChildren<QObject *>(widgetPtr, QString());
    for (int i = 0; i < mo->methodCount(); ++i) {
        const char *slot = mo->method(i).signature();
        Q_ASSERT(slot);
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
}


// protected methods

// reimplemented (iqt::CGuiComponentBase)

template <class UI, class WidgetType>
void TGuiComponentBase<UI, WidgetType>::OnRetranslate()
{
	retranslateUi(widgetPtr);

	BaseClass::OnRetranslate();
}


} // namespace iqt


#endif //!iqt_TGuiComponentBase_included


