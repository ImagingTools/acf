#ifndef icomp_CInterfaceManipBase_included
#define icomp_CInterfaceManipBase_included


// Qt includes
#include <QtCore/QByteArray>

// ACF includes
#include <istd/CIdManipBase.h>
#include <istd/CClassInfo.h>

#include <icomp/IComponent.h>


namespace icomp
{


class CInterfaceManipBase: public istd::CIdManipBase
{
protected:
	/**
		Extract interface from component.
	*/
	template <class Interface>
	static Interface* ExtractInterface(IComponent* componentPtr, const QByteArray& subId = "");
};


// protected template methods

template <class Interface>
Interface* CInterfaceManipBase::ExtractInterface(IComponent* componentPtr, const QByteArray& subId)
{
	if (componentPtr != NULL){
		static istd::CClassInfo info = istd::CClassInfo::GetInfo<Interface>();

		return static_cast<Interface*>(componentPtr->GetInterface(info, subId));
	}
	else{
		return NULL;
	}
}


} // namespace icomp


#endif // !icomp_CInterfaceManipBase_included


