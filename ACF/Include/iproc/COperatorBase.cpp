#include "iproc/COperatorBase.h"


#include "iser/IArchive.h"

#include "ibase/CMessage.h"

#include "isys/CSectionBlocker.h"


namespace iproc
{		

	
COperatorBase::COperatorBase()
{
	m_state = StateUnknown;

	m_progress = 0.0;
}
	

COperatorBase::~COperatorBase()
{
}


bool COperatorBase::IsAborted() const
{
	return GetProcessingState() == StateAborted;
}


void COperatorBase::SetProcessingState(int processingState)
{
	isys::CSectionBlocker lock(&m_mutex);

	m_state = (StateInfo)processingState;
}


void COperatorBase::AddError(const istd::CString& description)
{
	isys::CSectionBlocker lock(&m_mutex);

	m_log.AddMessage(new ibase::CMessage(ibase::IMessage::Warning, description, GetName()));
}


void COperatorBase::AddWarning(const istd::CString& description)
{
	isys::CSectionBlocker lock(&m_mutex);

	m_log.AddMessage(new ibase::CMessage(ibase::IMessage::Warning, description, GetName()));
}


const ibase::IMessageContainer& COperatorBase::GetLog() const
{
	return m_log;
}


// reimplemented (iproc::IOperator)

IOperator::StateInfo COperatorBase::GetProcessingState() const
{
	IOperator::StateInfo state = StateUnknown;
	
	isys::CSectionBlocker lock(&m_mutex);

	state = m_state;

	return m_state;
}


bool COperatorBase::BeforeProcessing()
{
	SetProcessingState(StateUnknown);

	return true;
}


bool COperatorBase::AfterProcessing()
{
	SetProcessingState(StateFinished);

	return true;
}

void COperatorBase::Cancel()
{
	SetProcessingState(StateAborted);
}


// reimplemented (iproc::IProgressInfo)

double COperatorBase::GetProgress() const
{
	return m_progress;
}


} // namespace iproc

