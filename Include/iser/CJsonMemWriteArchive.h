#pragma once


// ACF includes
#include <iser/CJsonWriteArchiveBase.h>


namespace iser
{


/**
	Implementation of an ACF Archive serializing to JSON string
*/
class CJsonMemWriteArchive: public CJsonWriteArchiveBase
{
public:
	typedef CJsonWriteArchiveBase BaseClass;

	CJsonMemWriteArchive(
				const iser::IVersionInfo* versionInfoPtr = NULL,
				bool serializeHeader = true,
				const iser::CArchiveTag& rootTag = s_acfRootTag);

	/**
		Close the archive and get the created JSON string.
	*/
	const QByteArray& GetData() const;

private:
	QBuffer m_textBuffer;
};


} // namespace iser


