#ifndef iproc_IOperator_included
#define iproc_IOperator_included


#include "istd/INamed.h"
#include "istd/IChangeable.h"
#include "istd/IEnableable.h"
#include "istd/IHierarchical.h"

#include "iproc/IProgressInfo.h"
#include "iproc/IProgressEventHandler.h"

#include "ibase/IMessageContainer.h"


namespace iproc
{		


/**
    \interface IOperator IOperator.h
    \brief a common interface for all operator objects.
*/
class IOperator:	virtual public istd::INamed, 
					virtual public istd::IEnableable, 
					virtual public istd::IHierarchical,
					virtual public iproc::IProgessInfo
{
public:

	enum ChangeFlags
	{
		ProgressChanged = 1
	};

	enum StateInfo
	{
		StateError = 1,
		StateFinished = 2,
		StateAborted = 4,
		StateProcessing = 8,
		StatePaused = 16,
		StateUnknown = 0xffffffff
	};

	/**
		Returns the state of the operator.
	*/
	virtual StateInfo GetProcessingState() const = 0;

	/**
		Does the pre-processing of the operator.
	*/
	virtual bool BeforeProcessing() = 0;

	/**
		Executes the operator. 
		Bevor the execution you should check the operator state with IsValid().
	*/
	virtual bool Execute() = 0;

	/**
		Does the post processing of the operator.
	*/
	virtual bool AfterProcessing() = 0;

	/**
		Cancel current processing routine.
	*/
	virtual void Cancel() = 0;
};


} // namespace iproc


#endif // !iproc_IOperator_included

