#pragma once


// ACF includes
#include <icmm/IColorantList.h>
#include <icmm/IColorModel.h>


namespace icmm
{


class ISubstractiveColorModel: virtual public icmm::IColorModel, virtual public icmm::IColorantList
{
};


} // namespace icmm


