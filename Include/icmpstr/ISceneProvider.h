#ifndef icmpstr_ISceneProvider_included
#define icmpstr_ISceneProvider_included


// Qt includes
#include <QtGui/QGraphicsScene>

// ACF includes
#include "istd/IPolymorphic.h"

#include "icmpstr/icmpstr.h"


namespace icmpstr
{


/**
	Interface for GUI objects managing scene.
*/
class ISceneProvider: virtual public istd::IPolymorphic
{
public:

	enum SceneFlags
	{
		/**
			Scene has a fixed size and cannot be resized by user.
		*/
		SF_FIXED_SIZE = 1,

		/**
			Scene has a grid alignment
			\sa GetSceneAlignment
		*/
		SF_ALIGNED = 2
	};


	/**
		Get ID indentifing this scene.
		Typically this ID is 0 for first scene, 1 for the second etc.
	*/
	virtual int GetSceneId() const = 0;

	/**
		Called when items should be removed from specified scene.
	*/
	virtual QGraphicsScene* GetScene() const = 0;

	/**
		Get parameters of grid alignment.
		\param	distance	distance between nearest alignment positions.
		\return	true if grid alignment is enabled.
		\sa SceneFlags
	*/
	virtual bool GetSceneAlignment(double& distance) const = 0;

	/**
		Get scene's paremeter info.
		\sa SceneFlags
	*/
	virtual int GetSceneFlags() const = 0;
};


} // namespace icmpstr


#endif // !icmpstr_ISceneProvider_included


