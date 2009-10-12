#ifndef icmpstr_IRegistryEditController_included
#define icmpstr_IRegistryEditController_included


#include "istd/IChangeable.h"
#include "istd/CString.h"

#include "i2d/CVector2d.h"

#include "icmpstr/icmpstr.h"


namespace icmpstr
{


/**
	Provide geometrical information for components inside registry.
*/
class IRegistryEditController: virtual public istd::IChangeable
{
public:
	/**
		Get the view position for the component \c componentName.
	*/
	virtual i2d::CVector2d GetComponentPosition(const std::string& componentName) const = 0;
	/**
		Set the view position for the component \c componentName.
	*/
	virtual void SetComponentPosition(const std::string& componentName, const i2d::CVector2d& position) = 0;
};


} // namespace icmpstr


#endif // !icmpstr_IRegistryEditController_included


