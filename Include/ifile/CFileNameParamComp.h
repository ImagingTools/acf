#ifndef ifile_CFileNameParamComp_included
#define ifile_CFileNameParamComp_included


// ACF includes
#include <icomp/CComponentBase.h>
#include <ifile/CFileNameParam.h>


namespace ifile
{


/**
	Basic implementation of interface ifile::IFileNameParam as component.
	The attribute 'DefaultPath' can contain folowing variables:
	* $(TempPath) - Temp path using by application (can be user-specific)
	* $(AppName) - Name of the application
	* $(CompanyName) - Name of the company
	* $(PublicSharedPath) -	Public user folder
	* $(UserHome) -	Current user's home folder
*/
class CFileNameParamComp:
			public icomp::CComponentBase,
			public CFileNameParam
{
public:
	typedef icomp::CComponentBase BaseClass;
	typedef CFileNameParam BaseClass2;

	I_BEGIN_COMPONENT(CFileNameParamComp);
		I_REGISTER_INTERFACE(iser::ISerializable);
		I_REGISTER_INTERFACE(IFileNameParam);
		I_ASSIGN(m_defaultDirAttrPtr, "DefaultPath", "Default path of file or directory", false, ".");
		I_ASSIGN(m_pathTypeAttrPtr, "PathType", "Type of path: 0 - unknown, 1 - PT_FILE, 2 - PT_DIRECTORY, 3 - PT_URL", true, PT_UNKNOWN);
		I_ASSIGN(m_initialPathCompPtr, "InitialPath", "If set, the current path will be initialized using specified file name component", false, "InitalPath");
	I_END_COMPONENT;

	// reimplemented (ifile::IFileNameParam)
	virtual int GetPathType() const;

protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();

private:
	I_ATTR(QString, m_defaultDirAttrPtr);
	I_ATTR(int, m_pathTypeAttrPtr);
	I_REF(ifile::IFileNameParam, m_initialPathCompPtr);

	static QString s_tempPathVariable;
	static QString s_appNameVariable;
	static QString s_companyNameVariable;
	static QString s_publicSharedPathVariable;
	static QString s_userHomePathVariable;
};


} // namespace ifile


#endif // !ifile_CFileNameParamComp_included


