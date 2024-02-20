#pragma once


// ACF includes
#include <icmm/CSubstractiveColorModel.h>


namespace icmm
{


class CCompositeDeviceColorModel: public CSubstractiveColorModel
{
public:
	// reimplemented (icmm::IColorantList)
	virtual QByteArrayList GetColorantIds() const override;
	virtual QString GetColorantName(const QByteArray& colorantId) const override;
	virtual ColorantType GetColorantType(const QByteArray& colorantId) const override;
	virtual ProcessColorantUsage GetProcessColorantUsage(const QByteArray& colorantId) const override;

protected:
	struct ColorantInfo
	{
		QByteArray id;
		QString name;
		ColorantType type = CT_PROCESS;
		ProcessColorantUsage usage = PCU_NONE;
	};

	typedef QVector<ColorantInfo> ColorantInfoList;

private:
	ColorantInfoList m_colorants;
};


} // namespace icmm


