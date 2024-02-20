#pragma once


// ACF includes
#include <icmm/CCmyColorModel.h>


namespace icmm
{


class CCmykColorModel: public CCmyColorModel
{
public:
	// reimplemented (icmm::IColorantList)
	virtual QByteArrayList GetColorantIds() const override;
};


// public methods

// reimplemented (icmm::IColorantList)

inline QByteArrayList CCmykColorModel::GetColorantIds() const
{
	return {"Cyan", "Magenta", "Yellow", "Black"};
}


} // namespace icmm


