#pragma once


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
	virtual IComponentUniquePtr CreateComponent() const override;

	//	reimplemented (icomp::IComponentStaticInfo)
	virtual int GetComponentType() const override;
};


// public methods

template <class Component>
TComponentStaticInfo<Component>::TComponentStaticInfo(const IRealComponentStaticInfo* baseComponentPtr)
:	BaseClass(baseComponentPtr)
{
}


// reimplemented (icomp::IRealComponentStaticInfo)

template <class Component>
IComponentUniquePtr TComponentStaticInfo<Component>::CreateComponent() const
{
	TComponentWrap<Component>* componentPtr = new TComponentWrap<Component>();

	return IComponentUniquePtr(componentPtr);
}


//	reimplemented (icomp::IComponentStaticInfo)

template <class Component>
int TComponentStaticInfo<Component>::GetComponentType() const
{
	return CT_REAL;
}


} // namespace icomp


