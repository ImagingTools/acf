#pragma once


// ACF includes
#include <icmm/CSubstractiveColorModel.h>


namespace icmm
{


class CCmyColorModel: public CSubstractiveColorModel
{
public:
	// reimplemented (icmm::IColorantList)
	virtual QByteArrayList GetColorantIds() const override;
	virtual QString GetColorantName(const QByteArray& colorantId) const override;
	virtual ColorantUsage GetColorantUsage(const QByteArray& colorantId) const override;
};


// public methods

// reimplemented (icmm::IColorantList)

QByteArrayList CCmyColorModel::GetColorantIds() const
{
	return {"Cyan", "Magenta", "Yellow"};
}


QString CCmyColorModel::GetColorantName(const QByteArray& colorantId) const
{
	QByteArrayList colorantIds = CCmyColorModel::GetColorantIds();
	if (colorantIds.contains(colorantId)){
		return QString(colorantId);
	}

	return QString();
}


icmm::ColorantUsage CCmyColorModel::GetColorantUsage(const QByteArray& colorantId) const
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

	return CU_NONE;
}


} // namespace icmm


