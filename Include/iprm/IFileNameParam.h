#ifndef iprm_IFileNameParam_included
#define iprm_IFileNameParam_included


#include "iser/ISerializable.h"

#include "iprm/iprm.h"


namespace iprm
{


class IFileNameParam: virtual public iser::ISerializable
{
public:
	enum PathType
	{
		PT_UNKNOWN,
		PT_FILE,
		PT_DIRECTORY
	};

	virtual int GetPathType() const = 0;
	virtual const istd::CString& GetPath() const = 0;
	virtual void SetPath(const istd::CString& directory) = 0;
};


} // namespace iprm


#endif // !iprm_IFileNameParam_included


