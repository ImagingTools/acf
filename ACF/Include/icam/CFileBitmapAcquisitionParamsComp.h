#ifndef icam_CFileBitmapAcquisitionParamsComp_included
#define icam_CFileBitmapAcquisitionParamsComp_included


#include "icomp/CComponentBase.h"

#include "icam/IFileBitmapAcquisitionParams.h"


namespace icam
{


class CFileBitmapAcquisitionParamsComp:
			public icomp::CComponentBase,
			virtual public IFileBitmapAcquisitionParams
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CFileBitmapAcquisitionParamsComp)
		I_REGISTER_INTERFACE(iser::ISerializable)
		I_REGISTER_INTERFACE(IFileBitmapAcquisitionParams)
		I_ASSIGN(m_defaultDirAttrPtr, "DefaultDir", "Default directory", false, ".")
	I_END_COMPONENT

	// reimplemented (icomp::IComponent)
	virtual void OnComponentCreated();

	// reimplemented (icam::IFileBitmapAcquisitionParams)
	virtual const istd::CString& GetDirectory() const;
	virtual void SetDirectory(const istd::CString& directory);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

private:
	istd::CString m_directory;

	I_ATTR(istd::CString, m_defaultDirAttrPtr);
};


} // namespace icam


#endif // !icam_CFileBitmapAcquisitionParamsComp_included


