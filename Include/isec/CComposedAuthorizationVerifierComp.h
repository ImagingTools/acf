#ifndef isec_CComposedAuthorizationVerifierComp_included
#define isec_CComposedAuthorizationVerifierComp_included


#include "imod/CMultiModelBridgeBase.h"

#include "icomp/CComponentBase.h"

#include "isec/IAuthorizationVerifier.h"


namespace isec
{


/**
	Allows to connect many authorisation versifiers into one.
*/
class CComposedAuthorizationVerifierComp:
			public icomp::CComponentBase,
			virtual public IAuthorizationVerifier,
			protected imod::CMultiModelBridgeBase
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CComposedAuthorizationVerifierComp);
		I_REGISTER_INTERFACE(IAuthorizationVerifier);
		I_ASSIGN_MULTI_0(m_slaveVerifiersCompPtr, "SlaveVerifiers", "List of slave verifiers", true);
		I_ASSIGN_MULTI_0(m_slaveVerifiersModelCompPtr, "SlaveVerifiers", "List of slave verifiers", true);
	I_END_COMPONENT;

	// reimplemented (isec::IAuthorizationVerifier)
	virtual bool IsAuthorized(const std::string& featureId) const;
	virtual Ids GetKnownFeatureIds() const;

protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

private:
	I_MULTIREF(IAuthorizationVerifier, m_slaveVerifiersCompPtr);
	I_MULTIREF(imod::IModel, m_slaveVerifiersModelCompPtr);
};


} // namespace isec


#endif // !isec_CComposedAuthorizationVerifierComp_included


