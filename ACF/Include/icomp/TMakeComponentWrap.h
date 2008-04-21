#ifndef icomp_TMakeComponentWrap_included
#define icomp_TMakeComponentWrap_included


#include "icomp/CComponentBase.h"


namespace icomp
{


/**
	Simple wrapper making component from non-component class.
	This class must provide single exported interface.
*/
template <class Interface, class Base>
class TMakeComponentWrap: public icomp::CComponentBase, public Base
{
public:
	typedef icomp::CComponentBase BaseClass;
	typedef Base BaseClass2;

	I_BEGIN_COMPONENT(TMakeComponentWrap)
		I_REGISTER_INTERFACE(Interface)
	I_END_COMPONENT
};


}//namespace icomp


#endif // !icomp_TMakeComponentWrap_included


