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
	virtual ColorantType GetColorantType(const QByteArray& colorantId) const override;
	virtual ProcessColorantUsage GetProcessColorantUsage(const QByteArray& colorantId) const override;
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
	if (colorantIds.contains(colorantId)) {
		return QString(colorantId);
	}

	return QString();
}


icmm::ColorantType CCmyColorModel::GetColorantType(const QByteArray& colorantId) const
{
	Q_ASSERT(GetColorantIds().contains(colorantId));

	return CT_PROCESS;
}


icmm::ProcessColorantUsage CCmyColorModel::GetProcessColorantUsage(const QByteArray& colorantId) const
{
	Q_ASSERT(GetColorantIds().contains(colorantId));

	return PCU_NONE;
}


} // namespace icmm


