#ifndef ifile_CFileNameParam_included
#define ifile_CFileNameParam_included


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
	virtual int GetPathType() const;
	virtual const QString& GetPath() const;
	virtual void SetPath(const QString& path);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

protected:
	QString m_path;
};


} // namespace ifile


#endif // !ifile_CFileNameParam_included


