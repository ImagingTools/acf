#ifndef icomp_IRegistryGeometryProvider_included
#define icomp_IRegistryGeometryProvider_included


#include "istd/IChangeable.h"
#include "istd/CString.h"
#include "i2d/CVector2d.h"


namespace icomp
{


/**
	Provide geometrical information for components inside registry.
*/
class IRegistryGeometryProvider: virtual public istd::IChangeable
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


} // namespace icomp


#endif // !icomp_IRegistryGeometryProvider_included


