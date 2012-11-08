#ifndef iprm_CFileNameParamComp_included
#define iprm_CFileNameParamComp_included


#include "iser/ISerializable.h"

#include "icomp/CComponentBase.h"

#include "ifile/IFileNameParam.h"


namespace iprm
{


/**
	Basic implementation of interface ifile::IFileNameParam as component.
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
};


} // namespace iprm


#endif // !iprm_CFileNameParamComp_included


