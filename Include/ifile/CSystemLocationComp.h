#ifndef ifile_CSystemLocationComp_included
#define ifile_CSystemLocationComp_included


// ACF includes
#include <iser/ISerializable.h>
#include <icomp/CComponentBase.h>
#include <ibase/IApplicationInfo.h>
#include <ifile/IFileTypeInfo.h>
#include <ifile/IFileNameParam.h>


namespace ifile
{


/**
	Provider of system pathes.
*/
class CSystemLocationComp:
			public icomp::CComponentBase,
			virtual public ifile::IFileNameParam
{
public:
	typedef icomp::CComponentBase BaseClass;

	enum SpecialLocations
	{
		/**
			Working directory of the application.
		*/
		SL_WORKING_DIRECTORY = 100,

		/**
			Path to the directory where the executable file is located.
		*/
		SL_EXECUTABLE_DIRECTORY = 101,

		/**
			Path to the exectuable file.
		*/
		SL_EXECUTABLE_FILE = 102,

		/**
			Directory for the application's content.
			Normally the same as SL_EXECUTABLE_DIRECTORY, but for Mac OS this is SL_EXECUTABLE_DIRECTORY/Content folder of the application bundle
		*/
		SL_EXECUTABLE_CONTENT = 103,

		/**
			User- and application-independent company data folder.
			Under Windows $(Install_Drive)\Users\Public\$(CompanyName))
		*/
		SL_SHARED_COMPANY_DIRECTORY = 104,

		/**
			User-independent application's data folder.
			Under Windows $(Install_Drive)\Users\Public\$(CompanyName)[$(ProductName)]\$(ApplicationName))
		*/
		SL_SHARED_APPDATA_DIRECTORY = 105
	};

	I_BEGIN_COMPONENT(CSystemLocationComp);
		I_REGISTER_INTERFACE(iser::ISerializable);
		I_REGISTER_INTERFACE(ifile::IFileNameParam);
		I_ASSIGN(m_locationTypeAttrPtr, "LocationType", "Type of system location:\n0 - user's desktop directory\n1 - user's document\n2 - user's fonts\n3 - user's applications\n4 - users music\n5 - user's movies\n6 - user's movies\n7 - system's temporary directory\n8 - user's home directory\n9 - directory location where persistent application data can be stored\n10 - Returns a directory location where user-specific non-essential (cached) data should be written\n100 - Working directory for the running executable\n101 - Directory of the executable file\n102 - Path to the executable file\n103 - Application's local content folder\n104 - User-indepedent company data folder\n105 - User-indepedent application's data folder", true, 0);
		I_ASSIGN(m_applicationInfoCompPtr, "ApplicationInfo", "Application info", false, "ApplicationInfo");
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
	I_REF(ibase::IApplicationInfo, m_applicationInfoCompPtr);

	I_ATTR(int, m_locationTypeAttrPtr);

	QString m_storagePath;
};


} // namespace ifile


#endif // !ifile_CSystemLocationComp_included


