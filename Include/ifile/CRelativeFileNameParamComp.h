#pragma once


// ACF includes
#include <imod/CModelUpdateBridge.h>
#include <ifile/CFileNameParamComp.h>
#include <ifile/IRelativeFilePath.h>


namespace ifile
{


class CRelativeFileNameParamComp:
			virtual public IRelativeFilePath,
			public ifile::CFileNameParamComp
{
public:
	typedef ifile::CFileNameParamComp BaseClass;

	I_BEGIN_COMPONENT(CRelativeFileNameParamComp);
		I_REGISTER_INTERFACE(IRelativeFilePath);
		I_ASSIGN(m_relativeToCompPtr, "ReferencePath", "Reference path, relative to this file or directory internal path representation will be calculated", false, "ReferencePath");
		I_ASSIGN_TO(m_relativeToModelCompPtr, m_relativeToCompPtr, false);
	I_END_COMPONENT;

	CRelativeFileNameParamComp();

	// reimplemented (ifile::IRelativeFilePath)
	virtual QString GetBasePath() const override;

	// reimplemented (ifile::IFileNameParam)
	virtual const QString& GetPath() const override;
	virtual void SetPath(const QString& path) override;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive) override;

protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated() override;
	virtual void OnComponentDestroyed() override;

private:
	I_REF(ifile::IFileNameParam, m_relativeToCompPtr);
	I_REF(imod::IModel, m_relativeToModelCompPtr);

	mutable QString m_relativePath;

	imod::CModelUpdateBridge m_updateBridge;
};


} // namespace ifile


