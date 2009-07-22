#ifndef icomp_CInterfaceManipBase_included
#define icomp_CInterfaceManipBase_included


// STL includes
#include <string>

#include "icomp/IComponent.h"


namespace icomp
{


class CInterfaceManipBase
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

protected:
	/**
		Extract interface from component.
	*/
	template <class Interface>
	static Interface* ExtractInterface(IComponent* componentPtr, const std::string& subId = "");
};


// protected template methods

template <class Interface>
Interface* CInterfaceManipBase::ExtractInterface(IComponent* componentPtr, const std::string& subId)
{
	if (componentPtr != NULL){
		static istd::CClassInfo info = istd::CClassInfo::GetInfo<Interface>();

		return static_cast<Interface*>(componentPtr->GetInterface(info, subId));
	}
	else{
		return NULL;
	}
}


}//namespace icomp


#endif // !icomp_CInterfaceManipBase_included


