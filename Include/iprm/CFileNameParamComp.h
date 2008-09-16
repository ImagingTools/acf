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

	I_BEGIN_COMPONENT(CFileNameParamComp)
		I_REGISTER_INTERFACE(iser::ISerializable)
		I_REGISTER_INTERFACE(IFileNameParam)
		I_ASSIGN(m_defaultDirAttrPtr, "DefaultDir", "Default directory", false, ".")
	I_END_COMPONENT

	// reimplemented (icomp::IComponent)
	virtual void OnComponentCreated();

	// reimplemented (iprm::IFileNameParam)
	virtual const istd::CString& GetDirectory() const;
	virtual void SetDirectory(const istd::CString& directory);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

private:
	istd::CString m_directory;

	I_ATTR(istd::CString, m_defaultDirAttrPtr);
};


} // namespace iprm


#endif // !iprm_CFileNameParamComp_included


