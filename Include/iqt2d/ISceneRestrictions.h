#ifndef iqt2d_ISceneRestrictions_included
#define iqt2d_ISceneRestrictions_included


#include "istd/IPolymorphic.h"

#include "iqt2d/iqt2d.h"


namespace iqt2d
{


/**
	Interface for query the scene manipulation restrictions.
*/
class ISceneRestrictions: virtual public istd::IPolymorphic
{
public:
	enum RestrictionFlags
	{
		/**
			All operations on scene object are allowed.
		*/
		SR_NONE = 0x0,

		/**
			Rotation operaration is prohibited.
		*/
		SR_ROTATE = 0x1,

		/**
			Translation operaration is prohibited.
		*/
		SR_SHIFT = 0x2,

		/**
			Translation operaration is prohibited.
		*/
		SR_SCALE = 0x4,

		/**
			All operations on scene object are prohibited.
		*/
		SR_ALL = 0xff
	};

	/**
		Get the restriction flags, to tell the scene manipulator, what can be changed on the scene.
	*/
	virtual int GetSceneRestrictionsFlags() const = 0;
};


} // namespace iqt2d


#endif // !iqt2d_ISceneRestrictions_included


