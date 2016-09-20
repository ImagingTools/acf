#ifndef ifile_CGeneratedFileNameParamComp_included
#define ifile_CGeneratedFileNameParamComp_included


// ACF includes
#include <ilog/TLoggerCompWrap.h>
#include <ifile/IFileTypeInfo.h>
#include <ifile/CRelativeFileNameParamComp.h>


namespace ifile
{


/**
	Component for auto generated file path.
*/
class CGeneratedFileNameParamComp: public ilog::TLoggerCompWrap<ifile::CRelativeFileNameParamComp>
{
public:
	typedef ilog::TLoggerCompWrap<ifile::CRelativeFileNameParamComp> BaseClass;

	I_BEGIN_COMPONENT(CGeneratedFileNameParamComp);
		I_ASSIGN(m_autoDeleteAttrPtr, "AutoDelete", "If enabled, the file or directory specified by this object will be removed during component deinitialization", false, false);
		I_ASSIGN(m_ensureDirectoryCreatedAttrPtr, "EnsureDirectoryCreated", "If enabled, the directory path will be created", false, false);
		I_ASSIGN(m_fileTypeInfoCompPtr, "FileTypeInfo", "File type information used for specifing the file extension", false, "FileTypeInfo");
	I_END_COMPONENT;

	// reimplemented (ifile::IFileNameParam)
	virtual void SetPath(const QString& path);

protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

private:
	I_ATTR(bool, m_autoDeleteAttrPtr);
	I_ATTR(bool, m_ensureDirectoryCreatedAttrPtr);
	I_REF(ifile::IFileTypeInfo, m_fileTypeInfoCompPtr);
};


} // namespace ifile


#endif // !ifile_CGeneratedFileNameParamComp_included


