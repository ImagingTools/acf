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

protected:
	/**
		Extract interface from component.
	*/
	template <class Interface>
	Interface* ExtractInterface(IComponent* componentPtr, const std::string& subId = "") const;
};


// protected template methods

template <class Interface>
Interface* CInterfaceManipBase::ExtractInterface(IComponent* componentPtr, const std::string& subId) const
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


