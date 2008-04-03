#ifndef iser_IArchive_included
#define iser_IArchive_included


#include <string>

#include "istd/IPolymorphic.h"
#include "istd/CString.h"

#include "iser/IVersionInfo.h"


namespace iser
{


class CArchiveTag;


/**
	Represent input/output persistence archive.
*/
class IArchive: virtual public istd::IPolymorphic
{
public:
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
		@sa VersionId.
	 */
	virtual I_DWORD GetVersion(int versionId = IVersionInfo::UserVersionId) const = 0;

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
	virtual bool Process(I_BYTE& value) = 0;

	/**
		Process primitive type.
	 */
	virtual bool Process(I_SBYTE& value) = 0;

	/**
		Process primitive type.
	 */
	virtual bool Process(I_WORD& value) = 0;

	/**
		Process primitive type.
	 */
	virtual bool Process(I_SWORD& value) = 0;

	/**
		Process primitive type.
	 */
	virtual bool Process(I_DWORD& value) = 0;

	/**
		Process primitive type.
	 */
	virtual bool Process(I_SDWORD& value) = 0;

	/**
		Process primitive type.
	 */
	virtual bool Process(I_QWORD& value) = 0;

	/**
		Process primitive type.
	 */
	virtual bool Process(I_SQWORD& value) = 0;

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
	virtual bool Process(std::string& value) = 0;

	/**
		Process primitive type.
	 */
	virtual bool Process(istd::CString& value) = 0;

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


#endif // iser_IArchive_included

