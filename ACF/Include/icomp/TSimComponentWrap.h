#ifndef icomp_TSimComponentWrap_included
#define icomp_TSimComponentWrap_included


#include "icomp/TComponentWrap.h"
#include "icomp/CSimComponentContextBase.h"


namespace icomp
{


/**
	Simulation wrapper of component.
	It allows to use components directly from static linked libraries, without component framework.
*/
template <class Base>
class TSimComponentWrap: public TComponentWrap<Base>, public CSimComponentContextBase
{
public:
	typedef TComponentWrap<Base> BaseClass;
	typedef CSimComponentContextBase BaseClass2;

	TSimComponentWrap();

	/**
		Initialilze component after setting all its attributes and references.
	*/
	void InitComponent();
};


// public methods

template <class Base>
TSimComponentWrap<Base>::TSimComponentWrap()
:	BaseClass(), BaseClass2(&InitStaticInfo(NULL))
{
}


template <class Base>
void TSimComponentWrap<Base>::InitComponent()
{
	SetComponentContext(this);
}


}//namespace icomp


#endif //!icomp_TSimComponentWrap_included


