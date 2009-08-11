#include "isec/CComposedAuthorizationVerifierComp.h"


namespace isec
{


// reimplemented (icomp::IComponent)

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


} // namespace isec


