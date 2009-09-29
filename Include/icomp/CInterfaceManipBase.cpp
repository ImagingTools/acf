#include "icomp/CInterfaceManipBase.h"


namespace icomp
{


// static methods

bool CInterfaceManipBase::SplitId(const std::string& complexId, std::string& baseId, std::string& subId)
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


std::string CInterfaceManipBase::JoinId(const std::string& baseId, const std::string& subId)
{
	if (!subId.empty()){
		return baseId + "/" + subId;
	}
	else{
		return baseId;
	}
}


}//namespace icomp


