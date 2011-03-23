#include "isec/CComposedAuthorizationVerifierComp.h"


namespace isec
{


// reimplemented (isec::IAuthorizationVerifier)

bool CComposedAuthorizationVerifierComp::IsAuthorized(const std::string& featureId) const
{
	int slavesCount = m_slaveVerifiersCompPtr.GetCount();
	for (int i = 0; i < slavesCount; ++i){
		const IAuthorizationVerifier* verifierPtr = m_slaveVerifiersCompPtr[i];
		if (verifierPtr != NULL){
			if (verifierPtr->IsAuthorized(featureId)){
				return true;
			}
		}
	}

	return false;
}


IAuthorizationVerifier::Ids CComposedAuthorizationVerifierComp::GetKnownFeatureIds() const
{
	Ids retVal;

	int slavesCount = m_slaveVerifiersCompPtr.GetCount();
	for (int i = 0; i < slavesCount; ++i){
		const IAuthorizationVerifier* verifierPtr = m_slaveVerifiersCompPtr[i];
		if (verifierPtr != NULL){
			Ids ids = verifierPtr->GetKnownFeatureIds();
			retVal.insert(ids.begin(), ids.end());
		}
	}

	return retVal;
}


// protected methods

// reimplemented (icomp::CComponentBase)

void CComposedAuthorizationVerifierComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	int modelsCount = m_slaveVerifiersModelCompPtr.GetCount();

	for (int i = 0; i < modelsCount; ++i){
		imod::IModel* modelPtr = m_slaveVerifiersModelCompPtr[i];
		if (modelPtr != NULL){
			modelPtr->AttachObserver(this);
		}
	}
}


void CComposedAuthorizationVerifierComp::OnComponentDestroyed()
{
	int modelsCount = m_slaveVerifiersModelCompPtr.GetCount();

	for (int i = 0; i < modelsCount; ++i){
		imod::IModel* modelPtr = m_slaveVerifiersModelCompPtr[i];
		if ((modelPtr != NULL) && modelPtr->IsAttached(this)){
			modelPtr->DetachObserver(this);
		}
	}

	BaseClass::OnComponentDestroyed();
}


} // namespace isec


