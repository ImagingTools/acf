#ifndef CLogGuiComp_included
#define CLogGuiComp_included


#include <QMutex>


#include "ibase/IMessageManager.h"
#include "ibase/TMessageManagerBase.h"

#include "iser/IFileSerializer.h"

#include "imod/TMultiModelObserverBase.h"

#include "iqt/Generated/ui_CLogGuiComp.h"

#include "iqt/TDesignerGuiComponentBase.h"


namespace iqt
{


class CLogGuiComp:
	public iqt::TDesignerGuiComponentBase<Ui::CLogGuiComp>,
	public ibase::TMessageManagerBase<ibase::IMessageManager>
{
	Q_OBJECT

public:
	typedef iqt::TDesignerGuiComponentBase<Ui::CLogGuiComp> BaseClass;
	typedef ibase::TMessageManagerBase<ibase::IMessageManager> BaseClass2;

	enum
	{
		TimeColumn = 0,
		SourceColumn = 1,
		TextColumn = 2
	};

	// reimplemented (ibase::IMessageManager)
	virtual void AddMessage(ibase::IMessage* messagePtr);

protected:
	// reimplemented (CGuiComponentBase)
	virtual void OnGuiCreated();

protected slots:
	void OnAddMessage(ibase::IMessage* messagePtr);
	void OnClear();
	void OnExport();

signals:
	void EmitAddMessage(ibase::IMessage* messagePtr);

private:
	I_REF(iser::IFileSerializer, m_fileSerializerCompPtr);

	QMutex m_lock;
};


} // namespace iqt


#endif // CLogGuiComp_included

