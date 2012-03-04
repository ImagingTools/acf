#ifndef iqt2d_ISceneExtender_included
#define iqt2d_ISceneExtender_included


#include "istd/IPolymorphic.h"

#include "iqt2d/IViewProvider.h"


namespace iqt2d
{


class IViewProvider;


/**
	Interface for GUI objects presenting its results using extern view objects.
*/
class IViewExtender: virtual public istd::IPolymorphic
{
public:
	/**
		Standard flags for items .
	*/
	enum SceneFlags{
		/**
			This flag is active if only shown elements will be used as background for other elements.
		*/
		SF_BACKGROUND = 1,
		/**
			This flag is set if this objects are added in direct call and will be reseted by delegation call to slave GUI's.
		 */
		SF_DIRECT = 2,
	};

	/**
		Called when items should be added to specified scene.
	*/
	virtual void AddItemsToScene(IViewProvider* providerPtr, int flags) = 0;

	/**
		Called when items should be removed from specified scene.
	*/
	virtual void RemoveItemsFromScene(IViewProvider* providerPtr) = 0;
};


} // namespace iqt2d


#endif // !iqt2d_ISceneExtender_included


