#ifndef iprm_IFileNameParam_included
#define iprm_IFileNameParam_included


#include "iser/ISerializable.h"

#include "iprm/iprm.h"


namespace iprm
{


class IFileNameParam: virtual public iser::ISerializable
{
public:
	virtual const istd::CString& GetDirectory() const = 0;
	virtual void SetDirectory(const istd::CString& directory) = 0;
};


} // namespace iprm


#endif // !iprm_IFileNameParam_included


