#ifndef QtPck_included
#define QtPck_included


#include "icomp/TModelCompWrap.h"
#include "icomp/TMakeComponentWrap.h"

#include "iqt/CPackagesLoaderComp.h"
#include "iqt/CRegistriesManagerComp.h"
#include "iqt/CBitmap.h"
#include "iqt/CBitmapSurface.h"
#include "iqt/CBitmapLoaderComp.h"
#include "iqt/CSettingsSerializerComp.h"
#include "iqt/CApplicationSettingsProviderComp.h"
#include "iqt/CIniSettingsProviderComp.h"
#include "iqt/CFileInfoCopyComp.h"
#include "iqt/CFileListProviderComp.h"
#include "iqt/CClipboardSerializerComp.h"
#include "iqt/CRenderedObjectFileLoaderComp.h"
#include "iqt/CTranslationManagerComp.h"
#include "iqt/CXmlFileReadArchive.h"
#include "iqt/CXmlFileWriteArchive.h"
#include "iqt/CRegistryCodeSaverComp.h"

#include "iqtdoc/CExtendedDocumentTemplateComp.h"
#include "iqtdoc/CStaticHelpFileProviderComp.h"
#include "iqtdoc/CDoxygenHelpFileProviderComp.h"

#include "ibase/TFileSerializerComp.h"


/**
	Standard Qt package.
*/
namespace QtPck
{


typedef iqt::CPackagesLoaderComp PackagesLoader;
typedef iqt::CRegistriesManagerComp RegistriesManager;
typedef icomp::TModelCompWrap<
			icomp::TMakeComponentWrap<
						iqt::CBitmap,
						iimg::IBitmap,
						iimg::IRasterImage,
						ibase::IObjectSnap,
						iser::ISerializable> > Bitmap;
typedef icomp::TModelCompWrap<
			icomp::TMakeComponentWrap<
						iqt::CBitmapSurface,
						iimg::IBitmap,
						iimg::IRasterImage,
						iser::ISerializable,
						imath::ISampledFunction2d> > BitmapSurface;
typedef iqt::CBitmapLoaderComp BitmapLoader;
typedef iqt::CSettingsSerializerComp SettingsSerializer;
typedef iqt::CApplicationSettingsProviderComp ApplicationSettingsProvider;
typedef iqt::CIniSettingsProviderComp IniSettingsProvider;
typedef iqt::CFileInfoCopyComp FileInfoCopy;
typedef icomp::TModelCompWrap<iqt::CFileListProviderComp> FileListProvider;
typedef iqt::CClipboardSerializerComp ClipboardSerializer;
typedef iqt::CRenderedObjectFileLoaderComp RenderedObjectFileLoader;
typedef icomp::TModelCompWrap<iqt::CTranslationManagerComp> TranslationManager;
typedef ibase::TFileSerializerComp<iqt::CXmlFileReadArchive, iqt::CXmlFileWriteArchive> QtXmlFileSerializer;
typedef iqt::CRegistryCodeSaverComp RegistryCodeSaver;

typedef iqtdoc::CExtendedDocumentTemplateComp ExtendedDocumentTemplate;
typedef iqtdoc::CStaticHelpFileProviderComp StaticHelpFileProvider;
typedef iqtdoc::CDoxygenHelpFileProviderComp DoxygenHelpFileProvider;


} // namespace QtPck


#endif // !QtPck_included


