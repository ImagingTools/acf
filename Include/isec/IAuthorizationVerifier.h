#ifndef isec_IAuthorizationVerifier_included
#define isec_IAuthorizationVerifier_included


// STL includes
#include <set>
#include <string>

#include "istd/IChangeable.h"


namespace isec
{


class IAuthorizationVerifier: virtual public istd::IChangeable
{
	public:
	typedef std::set<std::string> Ids;
	/**
		Check if some feature is authorized.
		\param	featureId	ID of checked feature or empty string, if default state should be returned.
		\return	true, if this feature is authorised.
	*/
	virtual bool IsAuthorized(const std::string& featureId) const = 0;
	virtual Ids GetKnownFeatureIds() const = 0;
};


} // namespace isec


#endif // !isec_IAuthorizationVerifier_included


