#ifndef CLogGuiComp_included
#define CLogGuiComp_included


#include "iqt/iqt.h"


#include <QObject>
#include <QMutex>


#include "ibase/IMessageManager.h"
#include "ibase/TMessageManagerBase.h"

#include "imod/TMultiModelObserverBase.h"


#ifdef OLD_ACF_SUPPORT


#include "iqt/Generated/ui_CLogGuiComp.h"

#include "QtAcf/QtGuiTemplateBasedComponent.h"

#include "Comp/ComponentDependency.h"


namespace iqt
{


class CLogGuiComp:
	public acf::QtGuiTemplateBasedComponent<QWidget, Ui::CLogGuiComp>,
	public ibase::TMessageManagerBase<ibase::IMessageManager>
{
	Q_OBJECT

public:
	typedef acf::QtGuiTemplateBasedComponent<QWidget, Ui::CLogGuiComp> BaseClass;
	typedef ibase::TMessageManagerBase<ibase::IMessageManager> BaseClass2;

	enum
	{
		TimeColumn = 0,
		SourceColumn = 1,
		TextColumn = 2
	};

	CLogGuiComp();

	// reimplemented (ibase::IMessageManager)
	virtual void AddMessage(ibase::IMessage* messagePtr);

protected:
	// reimplemented (QtAbstractGuiComponent)
	virtual void OnGuiCreated();

protected slots:
	void OnAddMessage(ibase::IMessage* messagePtr);
	void OnClear();
	void OnExport();

signals:
	void EmitAddMessage(ibase::IMessage* messagePtr);

private:
	QMutex m_lock;
};


} // namespace iqt


#endif // OLD_ACF_SUPPORT


#endif // CLogGuiComp_included

