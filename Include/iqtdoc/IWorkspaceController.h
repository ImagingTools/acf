#ifndef iqtdoc_IWorkspaceController_included
#define iqtdoc_IWorkspaceController_included


#include "istd/IPolymorphic.h"


namespace iqtdoc
{


/**	
	Interface for controlling of views in a multiple document workspace.
*/
class IWorkspaceController: public istd::IPolymorphic
{
public:
	/**
		Do vertical tiling of all views in the workspace area.
	*/
	virtual void TileHorizontally() = 0;

	/**
		Do tiling of all views in the workspace area.
	*/
	virtual void Tile() = 0;

	/**
		Cascade all views in the workspace area.
	*/
	virtual void Cascade() = 0;

	/**
		Close all views in the workspace area.
	*/
	virtual void CloseAllViews() = 0;
};


} // namespace iqtdoc


#endif // !iqtdoc_IWorkspaceController_included

