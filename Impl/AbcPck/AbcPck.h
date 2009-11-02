#ifndef AbcPck_included
#define AbcPck_included


#include "icomp/TModelCompWrap.h"
#include "icomp/TMakeComponentWrap.h"

#include "iabc/CFolderMonitorComp.h"
#include "iabc/CServiceApplicationComp.h"


/**
	Application Business Component package
*/
namespace AbcPck
{


typedef icomp::TModelCompWrap<iabc::CFolderMonitorComp> FolderMonitor;
typedef iabc::CServiceApplicationComp ServiceApplication;


} // namespace AbcPck


#endif // !AbcPck_included


