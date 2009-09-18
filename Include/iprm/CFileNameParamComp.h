#ifndef iprm_CFileNameParamComp_included
#define iprm_CFileNameParamComp_included


#include "icomp/CComponentBase.h"

#include "iprm/IFileNameParam.h"


namespace iprm
{


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
		I_ASSIGN(m_isFileAttrPtr, "IsFile", "Path is for files only", true, false);
		I_ASSIGN(m_isDirAttrPtr, "IsDirectory", "Path is for directories only", true, false);
	I_END_COMPONENT;

	// reimplemented (icomp::IComponent)
	virtual void OnComponentCreated();

	// reimplemented (iprm::IFileNameParam)
	virtual int GetPathType() const;
	virtual const istd::CString& GetPath() const;
	virtual void SetPath(const istd::CString& directory);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

private:
	istd::CString m_directory;

	I_ATTR(istd::CString, m_defaultDirAttrPtr);
	I_ATTR(bool, m_isFileAttrPtr);
	I_ATTR(bool, m_isDirAttrPtr);
};


} // namespace iprm


#endif // !iprm_CFileNameParamComp_included


