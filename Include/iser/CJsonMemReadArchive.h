#pragma once


// Qt includes
#include <QtCore/QBuffer>

// ACF includes
#include <iser/CJsonReadArchiveBase.h>


namespace iser
{


class CJsonMemReadArchive : public iser::CJsonReadArchiveBase
{
public:
	typedef CJsonReadArchiveBase BaseClass;

	CJsonMemReadArchive(const QByteArray& data, bool serializeHeader = true);

private:
	QBuffer m_buffer;
};


} // namespace iser


