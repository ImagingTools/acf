#pragma once


// ACF includes
#include <imath/IUnitInfo.h>
#include <icmm/IColorTransformation.h>


namespace icmm
{


class IColorModel: virtual public istd::IPolymorphic
{
public:
	enum ModelType
	{
		MT_HSI,
		MT_HSV,
		MT_HSL,
		MI_YIQ,
		MI_YUV,
		MI_YCBCR,
		MT_RGB,
		/**
			Substractive color model based on generic list of colorants.
			This includes CMYK, CMY, combinations with spot colors and ECG.
		*/
		MT_COLORANTS,
		MT_LUV,
		MT_LAB,
		MT_XYZ,

		/**
			Color model is defined by a set of spectral values.
		*/
		MI_SPECTRAL
	};

	enum ModelClass
	{
		MC_DEVICE_DEPENDENT,
		MC_DEVICE_INDEPENDENT
	};

	enum ColorSpaceClass
	{
		CSC_ADDITIVE,
		CSC_SUBSRACTIVE,
		CSC_CYLINDRIC,
		CSC_CARTESIAN
	};

	/**
		Get the type of the color model.
		\sa ModelType
	*/
	virtual ModelType GetModelType() const = 0;

	/**
		Get the classification of the color model.
		\sa ModelType
	*/
	virtual ModelClass GetModelClass() const = 0;

	/**
		Get the classification of the color space defined by this model.
		\sa ModelType
	*/
	virtual ColorSpaceClass GetColorSpaceClass() const = 0;

	/**
		Get dimensionality of the whole color space defined by the color model.
	*/
	virtual int GetColorSpaceDimensionality() const = 0;

	/**
		Get information about the mathematical unit related to the color model component (axis) at the given index.
	*/
	virtual const imath::IUnitInfo* GetColorSpaceComponentInfo(int componentIndex) const = 0;

	/**
		Get human-readable name of the color model component (axis) at the given index.
	*/
	virtual QString GetColorSpaceComponentName(int componentIndex) const = 0;

	/**
		Create color transformation for conversion from this model to another one.
	*/
	virtual const icmm::IColorTransformation* CreateColorTranformation(
				const IColorModel& otherColorModel,
				const QByteArray& transformationId = QByteArray()) const = 0;
};


} // namespace icmm
