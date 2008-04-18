#ifndef COperatorBase_included
#define COperatorBase_included



#include "iproc/IOperator.h"
#include "iproc/IProcessingEventHandler.h"

#include "iwin/CCriticalSection.h"

#include "ibase/THierarchicalBase.h"
#include "ibase/TEnableableWrap.h"
#include "ibase/TNamedBase.h"
#include "ibase/TMessageContainerWrap.h"


namespace iproc
{		


/**
	A base implementation of the operator interface.

	This is a common class for all operator classes that implement the operator interface. 
	It is advisable to use this implementation by programming of the own operators. 
*/

class COperatorBase: public ibase::TEnableableWrap< ibase::THierarchicalBase< ibase::TNamedBase<iproc::IOperator> > >
{
public :
	COperatorBase();
	virtual ~COperatorBase();

	virtual bool IsAborted() const;
	virtual void SetProcessingState(int processingState);
	virtual void AddError(const istd::CString& description);
	virtual void AddWarning(const istd::CString& description);
	virtual const ibase::IMessageContainer& GetLog() const;

	// reimplemented (iproc::IOperator)
	virtual StateInfo GetProcessingState() const;
	virtual bool BeforeProcessing();
	virtual bool AfterProcessing();
	virtual void Cancel();

	// reimplemented (iproc::IProgressInfo)
	virtual double GetProgress() const;

protected:
	double m_progress;

	ibase::CMessageContainer m_log;

private:
	StateInfo m_state;

	// TODO: replace by system independent service
	mutable iwin::CCriticalSection m_mutex; 
};


} // namespace iproc


#endif // COperatorBase_included

