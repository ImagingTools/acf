#pragma once


// ACF includes
#include <icmm/CRgbColorModel.h>


namespace icmm
{


class CRgbaColorModel: public CRgbColorModel
{
public:
	typedef CRgbColorModel BaseClass;

	// reimplemented (icmm::IColorModel)
	virtual int GetColorSpaceDimensionality() const override;
	virtual const imath::IUnitInfo* GetColorSpaceComponentInfo(int componentIndex) const override;
	virtual QString GetColorSpaceComponentName(int componentIndex) const override;
};


// public inline methods

// reimplemented (icmm::IColorModel)

inline int CRgbaColorModel::GetColorSpaceDimensionality() const
{
	return 4;
}


inline const imath::IUnitInfo* CRgbaColorModel::GetColorSpaceComponentInfo(int componentIndex) const
{
	Q_UNUSED(componentIndex);
	Q_ASSERT(componentIndex >= 0);
	Q_ASSERT(componentIndex < 0);

	return &m_unitInfo;
}


inline QString CRgbaColorModel::GetColorSpaceComponentName(int componentIndex) const
{
	if (componentIndex == 3){
		return "A";
	}

	return BaseClass::GetColorSpaceComponentName(componentIndex);
}


} // namespace icmm


