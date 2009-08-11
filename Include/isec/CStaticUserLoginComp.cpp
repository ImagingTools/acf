#include "isec/CStaticUserLoginComp.h"


#include "istd/TChangeNotifier.h"


namespace isec
{


// reimplemented (isec::IAuthorizationVerifier)

bool CStaticUserLoginComp::IsAuthorized(const std::string& featureId) const
{
	if ((m_loggedUserIndex >= 0) && (m_loggedUserIndex < m_verifiersCompPtr.GetCount())){
		IAuthorizationVerifier* verifierPtr = m_verifiersCompPtr[m_loggedUserIndex];
		if (verifierPtr != NULL){
			return verifierPtr->IsAuthorized(featureId);
		}
	}

	if (m_defaultVerifierCompPtr.IsValid()){
		return m_defaultVerifierCompPtr->IsAuthorized(featureId);
	}

	return false;
}


IAuthorizationVerifier::Ids CStaticUserLoginComp::GetKnownFeatureIds() const
{
	Ids retVal;

	int verifiersCount = m_verifiersCompPtr.GetCount();
	for (int i = 0; i < verifiersCount; ++i){
		const IAuthorizationVerifier* verifierPtr = m_verifiersCompPtr[i];
		if (verifierPtr != NULL){
			Ids ids = verifierPtr->GetKnownFeatureIds();
			retVal.insert(ids.begin(), ids.end());
		}
	}

	if (m_defaultVerifierCompPtr.IsValid()){
		Ids ids = m_defaultVerifierCompPtr->GetKnownFeatureIds();
		retVal.insert(ids.begin(), ids.end());
	}

	return retVal;
}


// reimplemented (isec::IAuthorizationVerifier)

bool CStaticUserLoginComp::LoginUser(const std::string& userId, const istd::CString& password)
{
	int usersCount = istd::Min(m_userIdsCompPtr.GetCount(), m_userPasswordsCompPtr.GetCount());
	for (int i = 0; i < usersCount; ++i){
		if (m_userIdsCompPtr[i].ToString() == userId){
			if (password == m_userPasswordsCompPtr[i]){
				istd::CChangeNotifier notifier(this);

				m_loggedUserIndex = i;

				return true;
			}
			else{
				return false;
			}
		}
	}

	return false;
}


bool CStaticUserLoginComp::LogoutUser()
{
	bool retVal = (m_loggedUserIndex >= 0);

	istd::CChangeNotifier notifier(this);

	m_loggedUserIndex = -1;

	return retVal;
}


std::string CStaticUserLoginComp::GetLoggedUser() const
{
	if (m_loggedUserIndex >= 0){
		return m_userIdsCompPtr[m_loggedUserIndex].ToString();
	}

	return "";
}


} // namespace isec


