#ifndef iproc_CHierarchicalOperator_included
#define iproc_CHierarchicalOperator_included


#include "iproc/COperatorBase.h"


namespace iproc
{		


class CHierarchicalOperator: public iproc::COperatorBase
{
public:
	typedef iproc::COperatorBase BaseClass;

	CHierarchicalOperator();
	virtual ~CHierarchicalOperator();

	virtual void AddChild(iproc::IOperator* operatorPtr);

	// reimplemented (ibase::IMessageContainer)
	virtual int GetWorstCategory() const;

	// reimplemented (iproc::IOperator)
	virtual bool BeforeProcessing();
	virtual bool Execute();
	virtual bool AfterProcessing();
	virtual void Cancel();

	// reimplemented (iproc::IProgressInfo)
	virtual double GetProgress() const;

	// reimplemented (istd::IHierarchical)
	virtual int GetChildCount() const;
	virtual istd::IPolymorphic* GetChild(int index) const;

protected:
	typedef std::vector<iproc::IOperator*> Childs;

	Childs m_childs;
};


} // namespace acf


#endif // iproc_CHierarchicalOperator_included

