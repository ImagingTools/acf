#ifndef ifile_CRelativeFileNameParamComp_included
#define ifile_CRelativeFileNameParamComp_included


// ACF includes
#include "imod/CMultiModelBridgeBase.h"
#include "ifile/CFileNameParamComp.h"


namespace ifile
{


class CRelativeFileNameParamComp:
			public ifile::CFileNameParamComp,
			protected imod::CMultiModelBridgeBase
{
public:
	typedef ifile::CFileNameParamComp BaseClass;

	I_BEGIN_COMPONENT(CRelativeFileNameParamComp);
		I_ASSIGN(m_relativeToCompPtr, "ReferencePath", "Reference path, relative to this file or directory internal path representation will be calculated", false, "ReferencePath");
		I_ASSIGN_TO(m_relativeToModelCompPtr, m_relativeToCompPtr, false);
	I_END_COMPONENT;

	CRelativeFileNameParamComp();

	// reimplemented (ifile::IFileNameParam)
	virtual const QString& GetPath() const;
	virtual void SetPath(const QString& path);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

private:
	I_REF(ifile::IFileNameParam, m_relativeToCompPtr);
	I_REF(imod::IModel, m_relativeToModelCompPtr);

	mutable QString m_relativePath;
};


} // namespace ifile


#endif // !ifile_CRelativeFileNameParamComp_included


