#include "FilePck.h"


#include <icomp/export.h>


namespace FilePck
{


I_EXPORT_PACKAGE(
			"Acf/File",
			"Package for file system primitives management",
			IM_PROJECT("ACF") IM_TAG("File") IM_COMPANY("ImagingTools") IM_AUTHOR("\"Witold Gantzke\" \"Kirill Lepskiy\""));

I_EXPORT_COMPONENT(
			FileNameParam,
			"Parameters for file/directory selection",
			"Name Directory Parameters URL" IM_CATEGORY(I_DATA_MODEL) IM_TAG("Model Parameter"));

I_EXPORT_COMPONENT(
			RelativeFileNameParam,
			"Provide file name relative to some other optional directory",
			"Name Relative Directory Parameters URL" IM_CATEGORY(I_DATA_MODEL) IM_TAG("Model Parameter"));

I_EXPORT_COMPONENT(
			SystemLocation,
			"System locations provider",
			"System Path File Location Temp Documents Media Cache" IM_TAG("Model"));

I_EXPORT_COMPONENT(
			GeneratedFileNameParam,
			"Provide auto generated file or directory name",
			"Name Relative Directory Parameters URL" IM_CATEGORY(I_DATA_MODEL) IM_TAG("Model Parameter"));

I_EXPORT_COMPONENT(
			FileListProvider,
			"Provides list of file synchronized with some directory",
			"List Provider Directory");

I_EXPORT_COMPONENT(
			AutoPersistence,
			"Allows object persistence using files",
			"Free Create Store Restore Load Save" IM_CATEGORY(I_DATA_PERSISTENCE) IM_TAG("Persistence File"));

I_EXPORT_COMPONENT(
			FileSystemInfoProvider,
			"Allows access to information about the local file system",
			"Free System Info" IM_CATEGORY(I_DATA_PERSISTENCE) IM_TAG("File System"));

I_EXPORT_COMPONENT(
			FileTypeInfo,
			"General file type information provider",
			"Type Information Format" IM_TAG("File"));

I_EXPORT_COMPONENT(
			ComposedFilePersistence,
			"Composed loader and saver delegating loading and saving to slave persistence implementations",
			"Composed File Load Save Slave Delegating" IM_CATEGORY(I_DATA_PERSISTENCE) IM_TAG("Composition Loader"));

I_EXPORT_COMPONENT(
			ComposedFileMetaInfoProvider,
			"Composed provider of file's meta informations",
			"File Info Information Provider Composition" IM_CATEGORY(I_DATA_PERSISTENCE) IM_TAG("File"));

I_EXPORT_COMPONENT(
			BinaryFileSerializer,
			"Simple file loader serializing objects using binary files",
			"Binary Load Save Serializer Archive Format" IM_CATEGORY(I_DATA_PERSISTENCE) IM_TAG("Loader File"));

I_EXPORT_COMPONENT(
			SimpleXmlFileSerializer,
			"File loader serializing objects using simplified XML format",
			"Save Serializer Archive Format" IM_CATEGORY(I_DATA_PERSISTENCE) IM_TAG("XML Loader File"));

I_EXPORT_COMPONENT(
			CompactXmlFileSerializer,
			"Stores and loads objects to/from files  using compact XML format",
			"Save Serializer Archive Format Compact Attributes" IM_CATEGORY(I_DATA_PERSISTENCE) IM_TAG("XML Loader File"));

I_EXPORT_COMPONENT(
			CompressedXmlFileSerializer,
			"Stores and loads objects to/from files  using compressed compact XML format",
			"Compress Uncompress Save Serializer Archive Format" IM_CATEGORY(I_DATA_PERSISTENCE) IM_TAG("XML Loader File"));

I_EXPORT_COMPONENT(
			TimeStampFileNameParam, 
			"Timestamp-based file name generation", 
			"Time File Name")

I_EXPORT_COMPONENT(
			ApplicationInfoBasedFileName, 
			"File name based on application attributes", 
			"Application Info Process File Name")


} // namespace FilePck


