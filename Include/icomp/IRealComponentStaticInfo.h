#ifndef icomp_IRealComponentStaticInfo_included
#define icomp_IRealComponentStaticInfo_included


// ACF includes
#include "icomp/IComponentStaticInfo.h"
#include "icomp/IComponentInterfaceExtractor.h"


namespace icomp
{


class IComponent;


/**
	Interface adding to component static info functionality existing only for real components.
	Real components are components which can be created in memory.
*/
class IRealComponentStaticInfo:
			virtual public IComponentStaticInfo,
			virtual public IComponentInterfaceExtractor
{
public:
	/**
		Create component instance.
		\return				pointer to created component or NULL if this component cannot be created.
	 */
	virtual IComponent* CreateComponent() const = 0;
};


} // namespace icomp


#endif // !icomp_IRealComponentStaticInfo_included


