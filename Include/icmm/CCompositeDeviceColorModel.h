#pragma once


// ACF includes
#include <icmm/CSubstractiveColorModelBase.h>


namespace icmm
{


class CCompositeDeviceColorModel: public CSubstractiveColorModelBase
{
public:
	// reimplemented (icmm::IColorantList)
	virtual QByteArrayList GetColorantIds() const override;
	virtual QString GetColorantName(const QByteArray& colorantId) const override;
	virtual ColorantUsage GetColorantUsage(const QByteArray& colorantId) const override;

protected:
	struct ColorantInfo
	{
		QByteArray id;
		QString name;
		ColorantUsage usage = CU_NONE;
	};

	typedef QVector<ColorantInfo> ColorantInfoList;

private:
	ColorantInfoList m_colorants;
};


} // namespace icmm


