#ifndef iser_IArchive_included
#define iser_IArchive_included


// Qt includes
#include <QtCore/QByteArray>

// Qt includes
#include <QtCore/QString>


// ACF includes
#include <istd/IPolymorphic.h>


namespace iser
{


class IVersionInfo;
class CArchiveTag;


/**
	Represent input/output persistence archive.

	\ingroup Persistence
	\ingroup Main
*/
class IArchive: virtual public istd::IPolymorphic
{
public:
	enum MessageId
	{
		MI_TAG_ERROR = 0x3f320a0,
		MI_TAG_SKIPPED
	};

	/**
		Check if this archive is loading or storing.
	 */
	virtual bool IsStoring() const = 0;

	/**
		Check if skiping to the end of tag on EndTag is supported.
		\sa EndTag.
	*/
	virtual bool IsTagSkippingSupported() const = 0;

	/**
		Get version of archived stream for specific versioning type.
		\param	versionId	versioning type. It allows you to distinguish different version aspects.
							For example version number of framework cann differ from application version.
		\sa VersionId.
	 */
	virtual const IVersionInfo& GetVersionInfo() const = 0;

	/**
		Begin of archive tag.
		\param	tag				tag object.
		\param	useTagSkipping	flag signalling that support of tag skipping should be enabled.
	*/
	virtual bool BeginTag(const CArchiveTag& tag) = 0;

	/**
		Begin of archive tag containing set of subelements of the same type.
		\sa BeginTag.
		\param	tag				main tag.
		\param	subTag			type of subelement tag.
		\param	count			number of subelements.
								If it is read archive this value will be set.
								If it storing archive this is input value.
		\param	useTagSkipping	flag signalling that support of tag skipping should be enabled.
		\return					true if success. In this case you have to call EndTag after serializing of subelements.
	*/
	virtual bool BeginMultiTag(const CArchiveTag& tag, const CArchiveTag& subTag, int& count) = 0;

	/**
		End of archive tag.
		This method should be allways called after BeginTag is successfull called.
		If skipping of tag contains is supported, this will skip to the end of tag while archive reading.
		Otherwise you have to read contains of archive completely. \sa IsTagSkippingSupported.
	*/
	virtual bool EndTag(const CArchiveTag& tag) = 0;

	/**
		Process primitive type.
	 */
	virtual bool Process(bool& value) = 0;

	/**
		Process primitive type.
	 */
	virtual bool Process(char& value) = 0;

	/**
		Process primitive type.
	 */
	virtual bool Process(quint8& value) = 0;

	/**
		Process primitive type.
	 */
	virtual bool Process(qint8& value) = 0;

	/**
		Process primitive type.
	 */
	virtual bool Process(quint16& value) = 0;

	/**
		Process primitive type.
	 */
	virtual bool Process(qint16& value) = 0;

	/**
		Process primitive type.
	 */
	virtual bool Process(quint32& value) = 0;

	/**
		Process primitive type.
	 */
	virtual bool Process(qint32& value) = 0;

	/**
		Process primitive type.
	 */
	virtual bool Process(quint64& value) = 0;

	/**
		Process primitive type.
	 */
	virtual bool Process(qint64& value) = 0;

	/**
		Process primitive type.
	 */
	virtual bool Process(float& value) = 0;

	/**
		Process primitive type.
	 */
	virtual bool Process(double& value) = 0;

	/**
		Process primitive type.
	 */
	virtual bool Process(QByteArray& value) = 0;

	/**
		Process primitive type.
	 */
	virtual bool Process(QString& value) = 0;

	/**
		Process binary data block.
		\param	dataPtr	pointer to memory block.
		\param	size	size of memory block in bytes.
	*/
	virtual bool ProcessData(void* dataPtr, int size) = 0;

	/**
		Process binary data block.
		\param	dataPtr	pointer to memory block.
		\param	bitsCount	number of bits.
		\param	bytesCount	size of memory block in bytes.
	*/
	virtual bool ProcessBits(void* dataPtr, int bitsCount, int bytesCount) = 0;
};


} // namespace iser


#endif // !iser_IArchive_included

