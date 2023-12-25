#pragma once


// ACF includes
#include <iser/ISerializable.h>
#include <ifile/IFileNameParam.h>


namespace ifile
{


/**
	Basic implementation of interface ifile::IFileNameParam.
*/
class CFileNameParam: virtual public IFileNameParam
{
public:

	// reimplemented (ifile::IFileNameParam)
	virtual int GetPathType() const override;
	virtual const QString& GetPath() const override;
	virtual void SetPath(const QString& path) override;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive) override;

	// reimplemented (istd::IChangeable)
	virtual int GetSupportedOperations() const override;
	virtual bool CopyFrom(const IChangeable& object, CompatibilityMode mode = CM_WITHOUT_REFS) override;
	virtual bool IsEqual(const IChangeable& object) const override;

protected:
	QString m_path;
};


} // namespace ifile


