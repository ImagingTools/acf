#pragma once


// ACF includes
#include <icmm/IColorantList.h>
#include <icmm/IColorModel.h>

// std
#include <memory>


namespace icmm
{


class ICieLabColor;


class ISubstractiveColorModel: virtual public icmm::IColorModel, virtual public icmm::IColorantList
{
public:
	/**
		Get visual information/preview for the given colorant.
	*/
	virtual bool GetColorantVisualInfo(const ColorantId& colorantId, icmm::ICieLabColor& preview) const = 0;

	/**
		Create ColorModel of the subspace
	*/
	virtual std::unique_ptr<ISubstractiveColorModel> CreateSubspaceModel(const QStringList& colorantIds) const = 0;
};


} // namespace icmm


