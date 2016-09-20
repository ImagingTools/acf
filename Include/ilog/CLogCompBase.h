#ifndef ilog_CLogCompBase_included
#define ilog_CLogCompBase_included


// Qt includes
#include <QtCore/QObject>

// ACF includes
#include <ilog/TMessageDelegatorComp.h>


namespace ilog
{


/**
	Basic implementation for logging using event queue for message transportation.

	\ingroup Logging
*/
class CLogCompBase:
			public QObject,
			public ilog::TMessageDelegatorComp<icomp::CComponentBase>
{
	Q_OBJECT
public:
	typedef ilog::TMessageDelegatorComp<icomp::CComponentBase> BaseClass;
	typedef QObject BaseClass2;

	I_BEGIN_BASE_COMPONENT(CLogCompBase);
	I_END_COMPONENT;

	CLogCompBase();

	// reimplemented (ilog::IMessageConsumer)
	virtual void AddMessage(const MessagePtr& messagePtr);

protected:
	virtual void WriteMessageToLog(const MessagePtr& messagePtr) = 0;

private Q_SLOTS:
	void OnAddMessage(const MessagePtr& messagePtr);

Q_SIGNALS:
	void EmitAddMessage(const MessagePtr& messagePtr);
};


} // namespace ilog


#endif // !ilog_CLogCompBase_included

