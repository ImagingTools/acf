#ifndef iqt_CRenderedObjectFileLoaderComp_included
#define iqt_CRenderedObjectFileLoaderComp_included


// Qt includes
#include <QDateTime>


// ACF includes
#include "istd/TSmartPtr.h"

#include "iser/IFileLoader.h"

#include "icomp/CComponentBase.h"

#include "ibase/IObjectSnap.h"

#include "iqt/CBitmap.h"


namespace iqt
{


class CRenderedObjectFileLoaderComp:
			public icomp::CComponentBase,
			virtual public iser::IFileLoader
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CRenderedObjectFileLoaderComp);
		I_REGISTER_INTERFACE(iser::IFileLoader);
		I_ASSIGN(m_fileLoaderCompPtr, "FileLoader", "File loader, which will used to open the input file", true, "FileLoader");
		I_ASSIGN(m_fileDataCompPtr, "FileData", "Data component for the loader", true, "FileData");
		I_ASSIGN(m_objectSnapCompPtr, "ObjectSnap", "Component for bitmap snap of a concrete object", true, "ObjectSnap");
		I_ASSIGN(m_widthAttrPtr, "BitmapWidth", "Width of the generated preview bitmap", true, 128);
		I_ASSIGN(m_heightAttrPtr, "BitmapHeight", "Height of the generated preview bitmap", true, 128);
	I_END_COMPONENT();

	// reimplemented (iser::IFileLoader)
	virtual bool IsOperationSupported(
				const istd::IChangeable* dataObjectPtr,
				const istd::CString* filePathPtr = NULL,
				int flags = 0,
				bool beQuiet = true) const;
	virtual int LoadFromFile(istd::IChangeable& data, const istd::CString& filePath = istd::CString()) const;
	virtual int SaveToFile(const istd::IChangeable& data, const istd::CString& filePath = istd::CString()) const;

	// reimplemented (iser::IFileTypeInfo)
	virtual bool GetFileExtensions(istd::CStringList& result, int flags = 0, bool doAppend = false) const;
	virtual istd::CString GetTypeDescription(const istd::CString* extensionPtr = NULL) const;

private:
	I_REF(iser::IFileLoader, m_fileLoaderCompPtr);
	I_REF(istd::IChangeable, m_fileDataCompPtr);
	I_REF(ibase::IObjectSnap, m_objectSnapCompPtr);
	I_ATTR(int, m_widthAttrPtr);
	I_ATTR(int, m_heightAttrPtr);

	struct FileInfo
	{
		QDateTime fileTimeStamp;
		istd::TSmartPtr<iqt::CBitmap> fileBitmapPtr;
	};

	typedef std::map<QString, FileInfo> PreviewCache;

	mutable PreviewCache m_previewCache;
};


} // namespace iqt


#endif // !iqt_CRenderedObjectFileLoaderComp_included


