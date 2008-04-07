#ifndef COperatorBase_included
#define COperatorBase_included



#include "iproc/IOperator.h"
#include "iproc/IProcessingEventHandler.h"

#include "iwin/CCriticalSection.h"

#include "ibase/THierarchicalBase.h"
#include "ibase/TEnableableWrap.h"
#include "ibase/TNamedBase.h"
#include "ibase/TMessageContainerBase.h"


namespace iproc
{		


/**
	A base implementation of the operator interface.

	This is a common class for all operator classes that implement the operator interface. 
	It is advisable to use this implementation by programming of the own operators. 
*/

class COperatorBase:	public ibase::CMessageContainer,
						public ibase::TEnableableWrap<
								ibase::THierarchicalBase<
									ibase::TNamedBase<iproc::IOperator> 
								> 
							> 
{
public :
	COperatorBase();
	virtual ~COperatorBase();

	virtual bool IsAborted() const;
	virtual void SetProcessingState(int processingState);
	virtual void AddError(const istd::CString& description);
	virtual void AddWarning(const istd::CString& description);

	// reimplemented (iproc::IOperator)
	virtual StateInfo GetProcessingState() const;
	virtual bool BeforeProcessing();
	virtual bool AfterProcessing();
	virtual void Cancel();

	// reimplemented (iproc::IProgressInfo)
	virtual double GetProgress() const;

protected:
	double m_progress;

private:
	StateInfo m_state;
	mutable iwin::CCriticalSection m_mutex; 
};


} // namespace iproc


#endif // COperatorBase_included

