#pragma once


// Qt includes
#include <QtCore/QFile>
#include <QtCore/QBuffer>

// ACF includes
#include <iser/CJsonWriteArchiveBase.h>


namespace ifile
{


/**
	Implementation of an ACF Archive serializing to JSON string
*/
class CJsonFileWriteArchive: public iser::CJsonWriteArchiveBase
{
public:
	CJsonFileWriteArchive(const QString& filePath = "", const iser::IVersionInfo* infoPtr = nullptr);
	~CJsonFileWriteArchive();
	bool OpenFile(const QString& filePath, bool serializeHeader);

private:
	QBuffer m_buffer;
	QFile m_file;
};


} // namespace ifile


