#ifndef CmpstrPck_included
#define CmpstrPck_included


#include "icomp/TModelCompWrap.h"
#include "icomp/TMakeComponentWrap.h"

#include "icmpstr/CAttributeEditorComp.h"
#include "icmpstr/CPackageOverviewComp.h"
#include "icmpstr/CRegistryLoaderComp.h"
#include "icmpstr/CRegistryPreviewComp.h"
#include "icmpstr/CComponentHelpFileProviderComp.h"
#include "icmpstr/CGeometricalRegistryComp.h"
#include "icmpstr/CRegistryGuiComp.h"

#include "icmpstr/CRegistryModelComp.h"	// TODO: remove it when old Compositor concept will be removed
#include "icmpstr/CRegistryViewComp.h"	// TODO: remove it when old Compositor concept will be removed


/**
	Component manipulation package.
	This package is used to contruct Compositor tool.
*/
namespace CmpstrPck
{


typedef icmpstr::CAttributeEditorComp AttributeEditor;
typedef icmpstr::CPackageOverviewComp PackageOverview;
typedef icmpstr::CRegistryLoaderComp RegistryLoader;
typedef icmpstr::CRegistryPreviewComp RegistryPreview;
typedef icmpstr::CComponentHelpFileProviderComp ComponentHelpFileProvider;
typedef icomp::TModelCompWrap<icmpstr::CGeometricalRegistryComp> VisualRegistry;
typedef icmpstr::CRegistryGuiComp VisualRegistryGui;

typedef icomp::TModelCompWrap<icmpstr::CRegistryModelComp> RegistryModel;
typedef icmpstr::CRegistryViewComp RegistryView;


} // namespace CmpstrPck


#endif // !CmpstrPck_included


