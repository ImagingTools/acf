#pragma once


// ACF includes
#include <icmm/ISubstractiveColorModel.h>
#include <icmm/CCieLabColor.h>
#include <icmm/CTristimulusSpecification.h>


namespace icmm
{


class CCieLabColor;


class CSubstractiveColorModelBase: virtual public icmm::ISubstractiveColorModel
{
public:
	CSubstractiveColorModelBase();

	const icmm::CTristimulusSpecification& GetPreviewSpec() const;
	void SetPreviewSpec(const ITristimulusSpecification& previewSpec);
	void SetColorantPreview(const ColorantId& colorantId, const icmm::CLab& preview);

	// reimplemented (icmm::IColorModel)
	virtual ModelType GetModelType() const override;
	virtual ModelClass GetModelClass() const override;
	virtual ColorSpaceClass GetColorSpaceClass() const override;
	virtual int GetColorSpaceDimensionality() const override;
	virtual const imath::IUnitInfo* GetColorSpaceComponentInfo(int componentIndex) const override;
	virtual QString GetColorSpaceComponentName(int componentIndex) const override;
	virtual const icmm::IColorTransformation* CreateColorTranformation(
				const IColorModel& otherColorModel,
				const QByteArray& transformationId = QByteArray()) const override;

	// reimplemented (icmm::ISubstractiveColorModel)
	virtual bool GetColorantVisualInfo(const ColorantId& colorantId, icmm::ICieLabColor& preview) const override;

protected:
	/**
		Tristimulus specification used for all colorants in the color model.
	*/
	icmm::CTristimulusSpecification m_previewSpec;

	/**
		Mapping between a colorant and corresponding Lab value for its solid value.
	*/
	typedef QMap<ColorantId, icmm::CLab> ColorantPreviewMap;
	ColorantPreviewMap m_colorantPreviewMap;
};


} // namespace icmm


