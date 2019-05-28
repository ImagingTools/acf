#include <ilog/CStandardStreamBridgeComp.h>


namespace ilog
{


// protected methods

// reimplemented (icomp::CComponentBase)

void CStandardStreamBridgeComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_slaveMessageConsumerCompPtr.IsValid()){
		m_coutBridgePtr.SetPtr(new CStandardStreamBridge(std::cout, *m_slaveMessageConsumerCompPtr, istd::IInformationProvider::IC_INFO));

		m_cerrBridgePtr.SetPtr(new CStandardStreamBridge(std::cerr, *m_slaveMessageConsumerCompPtr, istd::IInformationProvider::IC_ERROR));
	}
}

void CStandardStreamBridgeComp::OnComponentDestroyed()
{
	m_coutBridgePtr.Reset();
	m_cerrBridgePtr.Reset();

	BaseClass::OnComponentDestroyed();
}


} // namespace ilog


