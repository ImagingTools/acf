#pragma once


// ACF includes
#include <icmm/CCmyColorModel.h>


namespace icmm
{


class CCmykColorModel: public CCmyColorModel
{
public:
	typedef CCmyColorModel BaseClass;

	// reimplemented (icmm::IColorantList)
	virtual QByteArrayList GetColorantIds() const override;
	icmm::ColorantUsage GetColorantUsage(const QByteArray& colorantId) const override;
};


// public methods

// reimplemented (icmm::IColorantList)

inline QByteArrayList CCmykColorModel::GetColorantIds() const
{
	return {"Cyan", "Magenta", "Yellow", "Black"};
}


icmm::ColorantUsage CCmykColorModel::GetColorantUsage(const QByteArray& colorantId) const
{
	Q_ASSERT(GetColorantIds().contains(colorantId));

	if (colorantId == "Cyan"){
		return CU_CYAN;
	}
	else if (colorantId == "Magenta"){
		return CU_MAGENTA;
	}
	else if (colorantId == "Yellow"){
		return CU_YELLOW;
	}

	else if (colorantId == "Black"){
		return CU_BLACK;
	}

	return CU_NONE;
}



} // namespace icmm


