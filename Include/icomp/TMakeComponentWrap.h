#ifndef icomp_TMakeComponentWrap_included
#define icomp_TMakeComponentWrap_included


#include "icomp/CComponentBase.h"


namespace icomp
{


/**
	Simple wrapper making component from non-component class.
*/
template <	class Base,
			typename Interface1 = void,
			typename Interface2 = void,
			typename Interface3 = void,
			typename Interface4 = void,
			typename Interface5 = void,
			typename Interface6 = void,
			typename Interface7 = void,
			typename Interface8 = void,
			typename Interface9 = void,
			typename Interface10 = void>
class TMakeComponentWrap: public icomp::CComponentBase, public Base
{
public:
	typedef icomp::CComponentBase BaseClass;
	typedef Base BaseClass2;

	I_BEGIN_COMPONENT(TMakeComponentWrap);
		I_REGISTER_INTERFACE(Interface1);
		I_REGISTER_INTERFACE(Interface2);
		I_REGISTER_INTERFACE(Interface3);
		I_REGISTER_INTERFACE(Interface4);
		I_REGISTER_INTERFACE(Interface5);
		I_REGISTER_INTERFACE(Interface6);
		I_REGISTER_INTERFACE(Interface7);
		I_REGISTER_INTERFACE(Interface8);
		I_REGISTER_INTERFACE(Interface9);
		I_REGISTER_INTERFACE(Interface10);
	I_END_COMPONENT;
};


}//namespace icomp


#endif // !icomp_TMakeComponentWrap_included


