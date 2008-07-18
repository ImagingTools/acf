#ifndef iqt2d_ISceneProvider_included
#define iqt2d_ISceneProvider_included


#include <QGraphicsScene>

#include "istd/IPolymorphic.h"

#include "iqt2d/iqt2d.h"


namespace iqt2d
{


/**
	Interface for GUI objects managing scene.
*/
class ISceneProvider: virtual public istd::IPolymorphic
{
public:
	/**
		Get ID indentifing this scene.
		Typically this ID is 0 for first scene, 1 for the second etc.
	*/
	virtual int GetSceneId() const = 0;

	/**
		Called when items should be removed from specified scene.
	*/
	virtual QGraphicsScene* GetScene() const = 0;
};


} // namespace iqt2d


#endif // !iqt2d_ISceneProvider_included


