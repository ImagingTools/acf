#include "istd/CIdManipBase.h"


namespace istd
{


// static methods

bool CIdManipBase::SplitId(const std::string& complexId, std::string& baseId, std::string& subId)
{
	std::string::size_type separatorPos = complexId.find('/');
	if (separatorPos != std::string::npos){
		baseId = complexId.substr(0, separatorPos);
		subId = complexId.substr(separatorPos + 1);

		return true;
	}
	else{
		baseId = complexId;
		subId = "";

		return false;
	}
}


std::string CIdManipBase::JoinId(const std::string& baseId, const std::string& subId)
{
	if (!subId.empty()){
		return baseId + "/" + subId;
	}
	else{
		return baseId;
	}
}


}//namespace istd


