#ifndef isec_CStaticUserLoginComp_included
#define isec_CStaticUserLoginComp_included


#include "icomp/CComponentBase.h"

#include "isec/IAuthorizationVerifier.h"
#include "isec/IUserLogin.h"


namespace isec
{


class CStaticUserLoginComp:
			public icomp::CComponentBase,
			virtual public IAuthorizationVerifier,
			virtual public IUserLogin
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CStaticUserLoginComp);
		I_REGISTER_INTERFACE(IUserLogin);
		I_ASSIGN_MULTI_0(m_userIdsCompPtr, "UserIds", "List of user ID's", true);
		I_ASSIGN_MULTI_0(m_userPasswordsCompPtr, "UserPasswords", "List of user passwords", true);
		I_ASSIGN_MULTI_0(m_verifiersCompPtr, "Verifiers", "List of user authorization verifiers", true);
		I_ASSIGN(m_defaultVerifierCompPtr, "DefaultVerifier", "Authorization verifier used if no user is logged", true, "DefaultVerifier");
	I_END_COMPONENT;

	// reimplemented (isec::IAuthorizationVerifier)
	virtual bool IsAuthorized(const std::string& featureId) const;
	virtual Ids GetKnownFeatureIds() const;

	// reimplemented (isec::IUserLogin)
	virtual bool LoginUser(const std::string& userId, const istd::CString& password);
	virtual bool LogoutUser();
	virtual std::string GetLoggedUser() const;

private:
	I_MULTIATTR(istd::CString, m_userIdsCompPtr);
	I_MULTIATTR(istd::CString, m_userPasswordsCompPtr);
	I_MULTIREF(IAuthorizationVerifier, m_verifiersCompPtr);
	I_REF(IAuthorizationVerifier, m_defaultVerifierCompPtr);

	int m_loggedUserIndex;
};


} // namespace isec


#endif // !isec_CStaticUserLoginComp_included


