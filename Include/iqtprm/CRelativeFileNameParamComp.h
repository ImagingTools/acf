#ifndef iqtprm_CRelativeFileNameParamComp_included
#define iqtprm_CRelativeFileNameParamComp_included


#include "iprm/CFileNameParamComp.h"

#include "iqtprm/iqtprm.h"


namespace iqtprm
{


class CRelativeFileNameParamComp: public iprm::CFileNameParamComp
{
public:
	typedef iprm::CFileNameParamComp BaseClass;

	I_BEGIN_COMPONENT(CRelativeFileNameParamComp);
		I_ASSIGN(m_relativeToCompPtr, "ReferencePath", "Reference path, relative to this file or directory internal path representation will be calculated", false, "ReferencePath");
	I_END_COMPONENT;

	// reimplemented (iprm::IFileNameParam)
	virtual const istd::CString& GetPath() const;
	virtual void SetPath(const istd::CString& path);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

private:
	I_REF(iprm::IFileNameParam, m_relativeToCompPtr);

	mutable istd::CString m_relativePath;
};


} // namespace iqtprm


#endif // !iqtprm_CRelativeFileNameParamComp_included


