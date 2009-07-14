#ifndef iqt2d_ISceneController_included
#define iqt2d_ISceneController_included


#include "istd/IPolymorphic.h"

#include "iqt2d/iqt2d.h"


namespace iqt2d
{


class ISceneProvider;


/**
	Interface for components which can control the scene state and visualization parameters.
*/
class ISceneController: virtual public istd::IPolymorphic
{
public:
	/**
		Register the scene provider, from scene controller get the observed graphics scene.
	*/
	virtual void RegisterSceneProvider(const ISceneProvider* sceneProviderPtr) = 0;
};


} // namespace iqt2d


#endif // !iqt2d_ISceneController_included


