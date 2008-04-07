#include "iproc/CJob.h"


namespace iproc
{


CJob::CJob()
{
}


CJob::~CJob()
{
}


// reimplemented (iproc::IJob)

void CJob::RegisterElementFactory(IOperatorFactory* elementFactoryPtr)
{
	RegisterItemFactory(elementFactoryPtr);
}


iproc::IOperator* CJob::AddProcessingElement(const std::string elementFactoryKey, const std::string& elementId)
{
	iproc::IOperator* processingElementPtr = AddElement(elementFactoryKey);
	if (processingElementPtr != NULL){
		istd::TChangeNotifier<iproc::IJob> changePtr(this);

		processingElementPtr->SetName(elementId);

		return processingElementPtr;
	}

	return NULL;
}


// reimplemented (istd::IHierarchical)

int CJob::GetChildCount() const
{
	return GetItemCount();
}


istd::IPolymorphic* CJob::GetChild(int index) const
{
	return GetElement(index);
}


// protected members

// reimplemented (ibase::TFactorisableContainer)

iproc::IOperator* CJob::CreateElement(const std::string& itemKey)
{
	iproc::IOperator* operatorPtr = BaseClass::CreateElement(itemKey);

	iproc::COperatorBase* basePtr = dynamic_cast<iproc::COperatorBase*>(operatorPtr);
	if (basePtr != NULL){
		basePtr->SetParentPtr(this);
	}

	return operatorPtr;
}


// reimplemented (iser::ISerializable)

bool CJob::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag jobTag("Job", "Job");

	bool retVal = archive.BeginTag(jobTag);

	retVal = retVal && SerializeName(archive);
	
	retVal = retVal && BaseClass::Serialize(archive);
	
	retVal = retVal && archive.EndTag(jobTag);

	return retVal;
}


} // namespace iproc

