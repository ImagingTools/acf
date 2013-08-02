#ifndef FilePck_included
#define FilePck_included


#include "icomp/TModelCompWrap.h"
#include "icomp/TMakeComponentWrap.h"

#include "ifile/CFileNameParamComp.h"
#include "ifile/CRelativeFileNameParamComp.h"
#include "ifile/CSystemLocationComp.h"
#include "ifile/CGeneratedFileNameParamComp.h"
#include "ifile/CFileListProviderComp.h"
#include "ifile/CAutoPersistenceComp.h"


/**
	Package for file system primitives management.
*/
namespace FilePck
{


typedef icomp::TModelCompWrap<ifile::CFileNameParamComp> FileNameParam;
typedef icomp::TModelCompWrap<ifile::CRelativeFileNameParamComp> RelativeFileNameParam;
typedef icomp::TModelCompWrap<ifile::CSystemLocationComp> SystemLocation;
typedef icomp::TModelCompWrap<ifile::CGeneratedFileNameParamComp> GeneratedFileNameParam;
typedef icomp::TModelCompWrap<ifile::CFileListProviderComp> FileListProvider;
typedef icomp::TModelCompWrap<ifile::CFileListProviderComp> FileListProvider;
typedef ifile::CAutoPersistenceComp AutoPersistence;


} // namespace FilePck


#endif // !FilePck_included


