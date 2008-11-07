#ifndef iqt_CBitmapLoaderComp_included
#define iqt_CBitmapLoaderComp_included


// Qt includes
#include <QDir>
#include <QStringList>

#include "iser/IFileLoader.h"

#include "icomp/CComponentBase.h"

#include "ibase/TLoggerCompWrap.h"

#include "iqt/iqt.h"


namespace iqt
{


/**
	Bitmap loader component implementing interfaces \c iser::IFileLoader and \c icam::IBitmapAcquisition.
*/
class CBitmapLoaderComp:
			public ibase::TLoggerCompWrap<icomp::CComponentBase>,
			virtual public iser::IFileLoader
{
public:
	typedef ibase::TLoggerCompWrap<icomp::CComponentBase> BaseClass;

	enum MessageId
	{
		MI_BAD_EXTENSION = 0xa7d0,
		MI_FILE_NOT_EXIST,
		MI_BAD_FORMAT
	};

	I_BEGIN_COMPONENT(CBitmapLoaderComp);
	I_REGISTER_INTERFACE(iser::IFileLoader);
		I_ASSIGN(m_defaultFilePathAttrPtr, "DefaultFilePath", "Default file name used if no path is specified", false, "DefaultImage.png");
		I_ASSIGN_MULTI_3(m_fileExtensionsAttrPtr, "FileExtensions", "List of bitmap file extensions like 'bmp'", false, "png", "bmp", "jpg");
	I_END_COMPONENT;

	// reimplemented (iser::IFileLoader)
	virtual bool IsOperationSupported(
				const istd::IChangeable* dataObjectPtr,
				const istd::CString* filePathPtr = NULL,
				bool forLoading = true,
				bool forSaving = true,
				bool beQuiet = true) const;
	virtual int LoadFromFile(istd::IChangeable& data, const istd::CString& filePath) const;
	virtual int SaveToFile(const istd::IChangeable& data, const istd::CString& filePath) const;
	virtual bool GetFileExtensions(istd::CStringList& result, bool doAppend = false) const;

protected:
	bool IsOperationSupportedIntern(
				const istd::IChangeable* dataObjectPtr,
				const istd::CString* filePathPtr,
				bool forLoading,
				bool forSaving,
				bool useLog) const;

private:
	I_ATTR(istd::CString, m_defaultFilePathAttrPtr);
	I_MULTIATTR(istd::CString, m_fileExtensionsAttrPtr);
};


} // namespace iqt


#endif // !iqt_CBitmapLoaderComp_included


