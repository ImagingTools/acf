#ifndef CJob_included
#define CJob_included


#include "ibase/TFactorisableContainer.h"

#include "iproc/IOperator.h"
#include "iproc/IJob.h"
#include "iproc/CHierarchicalOperator.h"


namespace iproc
{

/**
	Standard implementation of an job.
*/
class CJob:	public iproc::IJob, 
			public ibase::TFactorisableContainer<iproc::IOperator>,
			public iproc::CHierarchicalOperator
{	
public:
	typedef ibase::TFactorisableContainer<iproc::IOperator> BaseClass;
	typedef iproc::CHierarchicalOperator BaseClass2;

	CJob();
	virtual ~CJob();

	// reimplemented (iproc::IJob)
	virtual void RegisterElementFactory(IOperatorFactory* elementFactoryPtr);
	virtual iproc::IOperator* AddProcessingElement(const std::string elementFactoryKey, const std::string& elementId);

	// reimplemented (istd::IHierarchical)
	virtual int GetChildCount() const;
	virtual istd::IPolymorphic* GetChild(int index) const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

protected:
	// reimplemented (ibase::TFactorisableContainer)
	virtual iproc::IOperator* CreateElement(const std::string& itemKey);
};


} // namespace iproc


#endif // CJob_included
