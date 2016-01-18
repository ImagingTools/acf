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
#include "ifile/CFileSystemInfoProviderComp.h"
#include "ifile/CComposedFilePersistenceComp.h"
#include "ifile/CFileTypeInfoComp.h"
#include "ifile/CComposedFileMetaInfoProviderComp.h"
#include "ifile/TFileSerializerComp.h"
#include "ifile/TXmlFileSerializerComp.h"
#include "ifile/CSimpleXmlFileReadArchive.h"
#include "ifile/CSimpleXmlFileWriteArchive.h"
#include "ifile/CFileReadArchive.h"
#include "ifile/CFileWriteArchive.h"
#include "ifile/CCompactXmlFileReadArchive.h"
#include "ifile/CCompactXmlFileWriteArchive.h"
#include "ifile/CCompressedXmlFileReadArchive.h"
#include "ifile/CCompressedXmlFileWriteArchive.h"
#include "ifile/CTimeStampFileNameParamComp.h"


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
typedef icomp::TModelCompWrap<ifile::CFileSystemInfoProviderComp> FileSystemInfoProvider;
typedef ifile::CComposedFilePersistenceComp ComposedFilePersistence;
typedef ifile::CFileTypeInfoComp FileTypeInfo;
typedef ifile::CComposedFileMetaInfoProviderComp ComposedFileMetaInfoProvider;
typedef ifile::TXmlFileSerializerComp<ifile::CSimpleXmlFileReadArchive, ifile::CSimpleXmlFileWriteArchive> SimpleXmlFileSerializer;
typedef ifile::TFileSerializerComp<ifile::CFileReadArchive, ifile::CFileWriteArchive> BinaryFileSerializer;
typedef ifile::TXmlFileSerializerComp<ifile::CCompactXmlFileReadArchive, ifile::CCompactXmlFileWriteArchive> CompactXmlFileSerializer;
typedef ifile::TXmlFileSerializerComp<ifile::CCompressedXmlFileReadArchive, ifile::CCompressedXmlFileWriteArchive> CompressedXmlFileSerializer;
typedef icomp::TModelCompWrap<ifile::CTimeStampFileNameParamComp> TimeStampFileNameParam;


} // namespace FilePck


#endif // !FilePck_included


