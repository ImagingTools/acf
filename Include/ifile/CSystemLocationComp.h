#ifndef ifile_CSystemLocationComp_included
#define ifile_CSystemLocationComp_included


#include "icomp/CComponentBase.h"

#include "iser/ISerializable.h"
#include "ifile/IFileTypeInfo.h"

#include "ifile/IFileNameParam.h"


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
		SL_EXECUTABLE_FILE = 102
	};

	I_BEGIN_COMPONENT(CSystemLocationComp);
		I_REGISTER_INTERFACE(iser::ISerializable);
		I_REGISTER_INTERFACE(ifile::IFileNameParam);
		I_ASSIGN(m_locationTypeAttrPtr, "LocationType", "Type of system location:\n0 - user's desktop directory\n1 - user's document\n2 - user's fonts\n3 - user's applications\n4 - users music\n5 - user's movies\n6 - user's movies\n7 - system's temporary directory\n8 - user's home directory\n9 - directory location where persistent application data can be stored\n10 - Returns a directory location where user-specific non-essential (cached) data should be written\n100 - Working directory for the running executable\n101 - Directory of the executable file\n102 - Path to the executable file", true, 0);
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
	I_ATTR(int, m_locationTypeAttrPtr);

	QString m_storagePath;
};


} // namespace ifile


#endif // !ifile_CSystemLocationComp_included


