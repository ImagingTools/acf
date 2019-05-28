#ifndef ilog_CStandardStreamBridgeComp_included
#define ilog_CStandardStreamBridgeComp_included


// ACF includes
#include <icomp/CComponentBase.h>
#include <ilog/CStandardStreamBridge.h>


namespace ilog
{


/**
	\ingroup Logging
*/
class CStandardStreamBridgeComp:public icomp::CComponentBase
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CStandardStreamBridgeComp);
		I_ASSIGN(m_slaveMessageConsumerCompPtr, "SlaveMessageConsumer", "Slave message consumer", false, "SlaveMessageConsumer");
	I_END_COMPONENT;


protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

private:
	istd::TDelPtr<CStandardStreamBridge> m_coutBridgePtr;
	istd::TDelPtr<CStandardStreamBridge> m_cerrBridgePtr;

	I_REF(ilog::IMessageConsumer, m_slaveMessageConsumerCompPtr);
};


} // namespace ilog


#endif // !ilog_CStandardStreamBridgeComp_included

