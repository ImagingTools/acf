#include "isys/CFileSystemBase.h"


// STL includes
#include <sys/types.h>
#include <fstream>
#include <iostream>


namespace isys
{


// reimplemented (isys::IFileSystem)

istd::CString CFileSystemBase::GetNormalizedPath(const istd::CString& path) const
{
	return path;	// TODO: correct CFileSystemBase::GetNormalizedPath dummy implementation
}


istd::CString CFileSystemBase::GetFileName(const istd::CString& filePath) const
{
	istd::CString::size_type slashPos = filePath.rfind('/');
	istd::CString::size_type backSlashPos = filePath.rfind('\\');

	if (slashPos != istd::CString::npos){
		if (backSlashPos != istd::CString::npos){
			return filePath.substr(istd::Max(slashPos, backSlashPos) + 1);
		}
		else{
			return filePath.substr(slashPos + 1);
		}
	}
	else{
		if (backSlashPos != istd::CString::npos){
			return filePath.substr(backSlashPos + 1);
		}
		else{
			return filePath;
		}
	}
}

istd::CString CFileSystemBase::GetBaseFileName(const istd::CString& filePath) const
{
	istd::CString fileName = GetFileName(filePath);

	istd::CString::size_type dotPos = fileName.find('.');

	if (dotPos != istd::CString::npos){
		return fileName.substr(0, dotPos);
	}

	return fileName;
}


istd::CString CFileSystemBase::GetDirPath(const istd::CString& filePath) const
{
	istd::CString::size_type slashPos = filePath.rfind('/');
	istd::CString::size_type backSlashPos = filePath.rfind('\\');

	if (slashPos != istd::CString::npos){
		if (backSlashPos != istd::CString::npos){
			return filePath.substr(0, istd::Max(slashPos, backSlashPos));
		}
		else{
			return filePath.substr(0, slashPos);
		}
	}
	else{
		if (backSlashPos != istd::CString::npos){
			return filePath.substr(0, backSlashPos);
		}
		else{
			return filePath;
		}
	}

	return filePath;
}


bool CFileSystemBase::HasExtension(const istd::CString& filePath, const istd::CString& extension) const
{
	istd::CString::size_type extensionSize = extension.size();
	istd::CString::size_type filePathSize = filePath.size();

	return (filePathSize > extensionSize + 1) && (filePath.substr(filePathSize - extensionSize - 1) == istd::CString(".") + extension);
}


} // namespace isys


