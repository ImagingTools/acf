#include "iproc/CHierarchicalOperator.h"


namespace iproc
{		


CHierarchicalOperator::CHierarchicalOperator()
	:BaseClass()
{
}
	

CHierarchicalOperator::~CHierarchicalOperator()
{
}


void CHierarchicalOperator::AddChild(iproc::IOperator* operatorPtr)
{
	iproc::COperatorBase* operatorImplPtr = dynamic_cast<iproc::COperatorBase*>(operatorPtr);
	if (operatorImplPtr != NULL){
		operatorImplPtr->SetParentPtr(this);
		const ibase::IHierarchicalMessageContainer* logPtr = dynamic_cast<const ibase::IHierarchicalMessageContainer*>(&operatorImplPtr->GetLog());
		m_log.AddChildContainer(const_cast<ibase::IHierarchicalMessageContainer*>(logPtr));
	}

	m_childs.push_back(operatorPtr);
}


// reimplemented (iproc::IOperator)

bool CHierarchicalOperator::BeforeProcessing()
{
	bool retVal = BaseClass::BeforeProcessing();

	int childCount = GetChildsCount();

	for (int childIndex = 0; childIndex < childCount; childIndex++){
		iproc::IOperator* operatorPtr = dynamic_cast<iproc::IOperator*>(GetChild(childIndex));
		if (operatorPtr != NULL && operatorPtr->IsEnabled()){
			retVal = operatorPtr->BeforeProcessing();
		}
	}

	return retVal;
}


bool CHierarchicalOperator::Execute()
{
	if (IsAborted()){
		return false;
	}

	bool retVal = true;

	SetProcessingState(StateProcessing);

	int childCount = GetChildsCount();

	for (int childIndex = 0; childIndex < childCount; childIndex++){
		iproc::IOperator* operatorPtr = dynamic_cast<iproc::IOperator*>(GetChild(childIndex));
		if (operatorPtr != NULL && operatorPtr->IsEnabled()){
			retVal = operatorPtr->Execute() && retVal;
			if (IsAborted()){
				return false;
			}
		}
	}

	return retVal;
}


bool CHierarchicalOperator::AfterProcessing()
{
	bool retVal = BaseClass::AfterProcessing();

	int childCount = GetChildsCount();

	for (int childIndex = 0; childIndex < childCount; childIndex++){
		iproc::IOperator* operatorPtr = dynamic_cast<iproc::IOperator*>(GetChild(childIndex));
		I_ASSERT(operatorPtr != NULL);

		if (operatorPtr != NULL && operatorPtr->IsEnabled()){
			retVal = operatorPtr->AfterProcessing() && retVal;
		}
	}

	return retVal;
}


void CHierarchicalOperator::Cancel()
{
	BaseClass::Cancel();

	int childCount = GetChildsCount();

	for (int childIndex = 0; childIndex < childCount; childIndex++){
		iproc::IOperator* operatorPtr = dynamic_cast<iproc::IOperator*>(GetChild(childIndex));
		I_ASSERT(operatorPtr != NULL);

		if (operatorPtr != NULL && operatorPtr->IsEnabled()){
			operatorPtr->Cancel();
		}
	}
}


// reimplemented (iproc::IProgressInfo)

double CHierarchicalOperator::GetProgress() const
{
	double summaryProgress = 0.0;

	int childCount = GetChildsCount();
	int activeOperatorCount = 0;

	for (int childIndex = 0; childIndex < childCount; childIndex++){
		iproc::IOperator* operatorPtr = dynamic_cast<iproc::IOperator*>(GetChild(childIndex));
		I_ASSERT(operatorPtr != NULL);

		if (operatorPtr != NULL && operatorPtr->IsEnabled()){
			activeOperatorCount++;

			summaryProgress += operatorPtr->GetProgress();
		}
	}

	return summaryProgress / activeOperatorCount;
}


// reimplemented (istd::TIHierarchical<istd::IPolymorphic>)

int CHierarchicalOperator::GetChildsCount() const
{
	return m_childs.size();
}


istd::IPolymorphic* CHierarchicalOperator::GetChild(int index) const
{
	if (index < int(m_childs.size()) && index >= 0){
		return m_childs.at(index);
	}

	return NULL;
}


} // namespace iproc

