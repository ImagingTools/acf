#pragma once


// ACF includes
#include <istd/IPolymorphic.h>


namespace iqt2d
{


class IViewProvider;


class IViewActivationHandler : virtual public istd::IPolymorphic
{
public:
	virtual void OnViewActivated(const iqt2d::IViewProvider* viewPtr) = 0;
	virtual void OnViewDeactivated(const iqt2d::IViewProvider* viewPtr) = 0;
};


} // namespace iqt2d


