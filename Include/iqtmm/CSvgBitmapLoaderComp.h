#ifndef iqtmm_CSvgBitmapLoaderComp_included
#define iqtmm_CSvgBitmapLoaderComp_included


#include "iser/IFileLoader.h"

#include "icomp/CComponentBase.h"

#include "ibase/TLoggerCompWrap.h"

#include "iqt/CBitmap.h"

#include "iqtmm/iqtmm.h"


namespace iqtmm
{


/**
	Load SVG files to bitmap.
*/
class CSvgBitmapLoaderComp:
			public ibase::CLoggerComponentBase,
			virtual public iser::IFileLoader
{
public:
	typedef ibase::CLoggerComponentBase BaseClass;

	I_BEGIN_COMPONENT(CSvgBitmapLoaderComp)
		I_REGISTER_INTERFACE(iser::IFileTypeInfo);
		I_REGISTER_INTERFACE(iser::IFileLoader);
		I_ASSIGN(m_bitmapWidthAttrPtr, "BitmapWidth", "Width of rendered bitmap in pixels", true, 100);
		I_ASSIGN(m_bitmapHeightAttrPtr, "BitmapHeight", "Height of rendered bitmap in pixels", true, 100);
	I_END_COMPONENT

	enum MessageId
	{
		MI_BAD_EXTENSION = 0xa7e0,
		MI_FILE_NOT_EXIST,
		MI_BAD_FORMAT,
		MI_BITMAP_TYPE
	};

	// reimplemented (iser::IFileLoader)
	virtual bool IsOperationSupported(
				const istd::IChangeable* dataObjectPtr,
				const istd::CString* filePathPtr = NULL,
				int flags = 0,
				bool beQuiet = true) const;
	virtual int LoadFromFile(istd::IChangeable& data, const istd::CString& filePath) const;
	virtual int SaveToFile(const istd::IChangeable& data, const istd::CString& filePath) const;

	// reimplemented (iser::IFileTypeInfo)
	virtual bool GetFileExtensions(istd::CStringList& result, int flags = 0, bool doAppend = false) const;
	virtual istd::CString GetTypeDescription(const istd::CString* extensionPtr = NULL) const;

private:
	I_ATTR(int, m_bitmapWidthAttrPtr);
	I_ATTR(int, m_bitmapHeightAttrPtr);

	mutable istd::CString m_lastFilePath;
	mutable iqt::CBitmap m_lastBitmap;
};


} // namespace iqtmm


#endif // !iqtmm_CSvgBitmapLoaderComp_included


