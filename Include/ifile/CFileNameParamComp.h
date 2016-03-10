#ifndef ifile_CFileNameParamComp_included
#define ifile_CFileNameParamComp_included


// ACF includes
#include "iser/ISerializable.h"
#include "icomp/CComponentBase.h"
#include "ifile/IFileNameParam.h"


namespace ifile
{


/**
	Basic implementation of interface ifile::IFileNameParam as component.
	The attribute 'DefaultPath' can contain folowing variables:
	* $(TempPath) - Temp path using by application (can be user-specific)
	* $(AppName) - Name of the application
	* $(CompanyName) - Name of the company
*/
class CFileNameParamComp:
			public icomp::CComponentBase,
			virtual public IFileNameParam
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CFileNameParamComp);
		I_REGISTER_INTERFACE(iser::ISerializable);
		I_REGISTER_INTERFACE(IFileNameParam);
		I_ASSIGN(m_defaultDirAttrPtr, "DefaultPath", "Default path of file or directory", false, ".");
		I_ASSIGN(m_pathTypeAttrPtr, "PathType", "Type of path: 0 - unknown, 1 - PT_FILE, 2 - PT_DIRECTORY, 3 - PT_URL", true, PT_UNKNOWN);
	I_END_COMPONENT;

	// reimplemented (ifile::IFileNameParam)
	virtual int GetPathType() const;
	virtual const QString& GetPath() const;
	virtual void SetPath(const QString& path);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();

private:
	QString m_path;

	I_ATTR(QString, m_defaultDirAttrPtr);
	I_ATTR(int, m_pathTypeAttrPtr);

	static QString s_tempPathVariable;
	static QString s_appNameVariable;
	static QString s_companyNameVariable;
	static QString s_publicSharedPathVariable;
};


} // namespace ifile


#endif // !ifile_CFileNameParamComp_included


