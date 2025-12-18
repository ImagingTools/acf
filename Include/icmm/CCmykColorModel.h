#pragma once


// ACF includes
#include <icmm/CCmyColorModel.h>


namespace icmm
{


class CCmykColorModel: public CCmyColorModel
{
public:
	typedef CCmyColorModel BaseClass;

	static ColorantId GetBlack();

	// reimplemented (icmm::IColorantList)
	virtual ColorantIds GetColorantIds() const override;
	virtual icmm::ColorantUsage GetColorantUsage(const ColorantId& colorantId) const override;

	// reimplemented ISubstractiveColorModel
	virtual std::unique_ptr<ISubstractiveColorModel> CreateSubspaceModel(const QStringList& colorantIds) const override;
};


} // namespace icmm


