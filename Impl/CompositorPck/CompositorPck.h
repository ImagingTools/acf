#ifndef CompositorPck_included
#define CompositorPck_included


#include "icomp/TModelCompWrap.h"
#include "icomp/TMakeComponentWrap.h"

#include "icmpstr/CAttributeEditorComp.h"
#include "icmpstr/CPackageOverviewComp.h"
#include "icmpstr/CRegistryLoaderComp.h"
#include "icmpstr/CRegistryPreviewComp.h"
#include "icmpstr/CComponentHelpFileProviderComp.h"
#include "icmpstr/CVisualRegistryComp.h"
#include "icmpstr/CVisualRegistryScenographerComp.h"
#include "icmpstr/CRegistryConsistInfoComp.h"
#include "icmpstr/CRegistryPropEditorComp.h"
#include "icmpstr/CQuickHelpGuiComp.h"
#include "icmpstr/CPackagesLoaderExtComp.h"


/**
	Component manipulation package.
	This package is used to contruct Compositor tool.
*/
namespace CompositorPck
{


typedef icmpstr::CAttributeEditorComp AttributeEditor;
typedef icmpstr::CPackageOverviewComp PackageOverview;
typedef icmpstr::CRegistryLoaderComp RegistryLoader;
typedef icmpstr::CRegistryPreviewComp RegistryPreview;
typedef icmpstr::CComponentHelpFileProviderComp ComponentHelpFileProvider;
typedef icomp::TModelCompWrap<icmpstr::CVisualRegistryComp> VisualRegistry;
typedef icomp::TModelCompWrap<icmpstr::CVisualRegistryScenographerComp> VisualRegistryScenographer;
typedef icomp::TMakeComponentWrap<
			imod::TModelWrap<icomp::CRegistry>,
			icomp::IRegistry,
			iser::ISerializable,
			istd::IChangeable,
			imod::IModel> Registry;
typedef icmpstr::CRegistryConsistInfoComp RegistryConsistInfo;
typedef icmpstr::CRegistryPropEditorComp RegistryPropEditor;
typedef icmpstr::CQuickHelpGuiComp QuickHelpGui;
typedef icomp::TModelCompWrap<icmpstr::CPackagesLoaderExtComp> PackagesLoaderExt;


} // namespace CompositorPck


#endif // !CompositorPck_included


