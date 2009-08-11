#ifndef isec_IUserLogin_included
#define isec_IUserLogin_included


#include "istd/IChangeable.h"
#include "istd/CString.h"


namespace isec
{


class IUserLogin: virtual public istd::IChangeable
{
public:
	virtual bool LoginUser(const std::string& userId, const istd::CString& password) = 0;
	virtual bool LogoutUser() = 0;
	virtual std::string GetLoggedUser() const = 0;
};


} // namespace isec


#endif // !isec_IUserLogin_included


