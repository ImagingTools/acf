#ifndef icomp_TComponentStaticInfo_included
#define icomp_TComponentStaticInfo_included


// ACF includes
#include <icomp/TComponentWrap.h>
#include <icomp/CBaseComponentStaticInfo.h>


namespace icomp
{


template <class Component>
class TComponentStaticInfo: public CBaseComponentStaticInfo
{
public:
	typedef CBaseComponentStaticInfo BaseClass;

	explicit TComponentStaticInfo(const IRealComponentStaticInfo* baseComponentPtr = NULL);

	// reimplemented (icomp::IRealComponentStaticInfo)
	virtual IComponent* CreateComponent() const;

	//	reimplemented (icomp::IComponentStaticInfo)
	virtual int GetComponentType() const;
};


// public methods

template <class Component>
TComponentStaticInfo<Component>::TComponentStaticInfo(const IRealComponentStaticInfo* baseComponentPtr)
:	BaseClass(baseComponentPtr)
{
}


// reimplemented (icomp::IRealComponentStaticInfo)

template <class Component>
IComponent* TComponentStaticInfo<Component>::CreateComponent() const
{
	TComponentWrap<Component>* componentPtr = new TComponentWrap<Component>();

	return componentPtr;
}


//	reimplemented (icomp::IComponentStaticInfo)

template <class Component>
int TComponentStaticInfo<Component>::GetComponentType() const
{
	return CT_REAL;
}


} // namespace icomp


#endif // !icomp_TComponentStaticInfo_included


