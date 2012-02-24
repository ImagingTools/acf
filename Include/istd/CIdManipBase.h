#ifndef istd_CIdManipBase_included
#define istd_CIdManipBase_included


// STL includes
#include <string>


// ACF includes
#include "istd.h"


namespace istd
{


class CIdManipBase
{
public:
	/**
		Split component ID into separated ID's.
	*/
	static bool SplitId(const std::string& complexId, std::string& baseId, std::string& subId);
	/**
		Join base component ID and sub ID into composed component ID.
	*/
	static std::string JoinId(const std::string& baseId, const std::string& subId);
};


}//namespace istd


#endif // !istd_CIdManipBase_included


