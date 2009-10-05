#ifndef iprm_CFileNameParamComp_included
#define iprm_CFileNameParamComp_included


#include "icomp/CComponentBase.h"

#include "iprm/IFileNameParam.h"


namespace iprm
{


/**
	Basic implementation of interface iprm::IFileNameParam as component.
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
		I_ASSIGN(m_pathTypeAttrPtr, "PathType", "Type of path: 0 - unknown, 1 - PT_FILE, 2 - PT_DIRECTORY, 3 - PT_URL", true, 0);
	I_END_COMPONENT;

	// reimplemented (icomp::IComponent)
	virtual void OnComponentCreated();

	// reimplemented (iprm::IFileNameParam)
	virtual int GetPathType() const;
	virtual const istd::CString& GetPath() const;
	virtual void SetPath(const istd::CString& path);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

private:
	istd::CString m_path;

	I_ATTR(istd::CString, m_defaultDirAttrPtr);
	I_ATTR(int, m_pathTypeAttrPtr);
};


} // namespace iprm


#endif // !iprm_CFileNameParamComp_included


