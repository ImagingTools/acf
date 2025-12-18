#pragma once


// ACF includes
#include <icmm/CRgb.h>
#include <icmm/IColorModel.h>
#include <icmm/IRgbColor.h>
#include <icmm/ITristimulusSpecification.h>


namespace icmm
{


class CRgbColor: virtual public IRgbColor
{
public:
	/**
		Takes a ColorModel (if you want to get particular about the color specification)
	*/
	CRgbColor(const icmm::CRgb& rgb, ColorModelPtr modelPtr);

	/**
		Just pass RGB-values; defaults the ColorModel to D50/2D
	*/
	CRgbColor(const icmm::CRgb& rgb);

	void SetSpecification(const ITristimulusSpecification& spec);

	// reimplemented (icmm::IRgbColor)
	virtual const icmm::CRgb& GetRgb() const override;
	virtual IColorSpecification::ConstColorSpecPtr GetSpecification() const override;

	// reimplemented (icmm::IColorObject)
	virtual icmm::CVarColor GetColor() const override;
	virtual ConstColorModelPtr GetColorModel() const override;
	virtual std::unique_ptr<IColorObject> CloneIntoUniquePtr() const override;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive) override;

private:
	icmm::CRgb m_rgb;
	std::shared_ptr<IColorModel> m_modelPtr;
};


} // namespace icmm


