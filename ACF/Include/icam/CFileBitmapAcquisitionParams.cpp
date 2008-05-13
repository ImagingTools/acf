#include "icam/CFileBitmapAcquisitionParams.h"


namespace icam
{


// reimplemented (icam::IFileBitmapAcquisitionParams)

istd::CString CFileBitmapAcquisitionParams::GetDirectory() const
{
	return m_directory;
}


} // namespace icam


