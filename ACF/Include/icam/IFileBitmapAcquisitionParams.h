#ifndef icam_IFileBitmapAcquisitionParams_included
#define icam_IFileBitmapAcquisitionParams_included


#include "iser/ISerializable.h"

#include "icam/icam.h"


namespace icam
{


class IFileBitmapAcquisitionParams: virtual public iser::ISerializable
{
public:
	virtual const istd::CString& GetDirectory() const = 0;
	virtual void SetDirectory(const istd::CString& directory) = 0;
};


} // namespace icam


#endif // !icam_IFileBitmapAcquisitionParams_included


