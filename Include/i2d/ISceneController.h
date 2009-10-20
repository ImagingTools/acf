#ifndef i2d_ISceneController_included
#define i2d_ISceneController_included


#include "istd/IPolymorphic.h"

#include "i2d/i2d.h"


namespace i2d
{


/**
	Interface for scene manipulation and query the restrictions.
*/
class ISceneController: virtual public istd::IPolymorphic
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
		Vieport scale fitting mode.
	*/
	enum FitMode{
		/**
			No fitting will be done.
		*/
		FM_NONE,
		/**
			Isotropic scale will be done, but only scale reduction is allowed.
		*/
		FM_ISOTROPIC_REDUCTION,
		/**
			Complete isotropic scale will be done.
		*/
		FM_ISOTROPIC,
		/**
			Anisotropic scale will be done to fit whole viewport area.
		*/
		FM_ANISOTROPIC
	};

	/**
		Describe mode for setting of scale value.
	*/
	enum ScaleMode
	{
		/**
			Simple set, old scale will be replaced with the new value.
		*/
		SM_SET,
		/**
			Old scale will be mutliplied by the new value.
		*/
		SM_COMBINE,
		/**
			Zoom will be reset, parameter value will be ignored.
		*/
		SM_RESET,
		/**
			Zoom in will be done, parameter value will be ignored.
		*/
		SM_ZOOM_IN,
		/**
			Zoom out will be done, parameter value will be ignored.
		*/
		SM_ZOOM_OUT,
		/**
			Isotropic scale will be done, but only scale reduction is allowed.
		*/
		SM_FIT_ISOTROPIC_REDUCTION,
		/**
			Complete isotropic scale will be done.
		*/
		SM_FIT_ISOTROPIC,
		/**
			Anisotropic scale will be done to fit whole viewport area.
		*/
		SM_FIT_ANISOTROPIC
	};

	/**
		Get the restriction flags, to tell the scene manipulator, what can be changed on the scene.
	*/
	virtual int GetSceneRestrictionFlags() const = 0;

	/**
		Get vieport scale fitting mode.
	*/
	virtual FitMode GetFitMode() const = 0;
	/**
		Set vieport scale fitting mode.
		\return	true if fitting mode was set correctly.
		\sa RestrictionFlags
	*/
	virtual bool SetFitMode(FitMode fitMode) = 0;

	/**
		Check if full screen mode is enabled.
		In full screen mode scene is shown on whole screen area.
	*/
	virtual bool IsFullScreenMode() const = 0;
	/**
		Set full screen mode on/off.
		In full screen mode scene is shown on whole screen area.
		\return	true if full screen mode was set correctly.
		\sa RestrictionFlags
	*/
	virtual bool SetFullScreenMode(bool isFullScreen) = 0;

	/**
		Get actual scale factor.
	*/
	virtual bool GetScale() const = 0;
	/**
		Set or modify scale factor.
		See ScaleMode for possible modes.
	*/
	virtual bool SetScale(int scaleMode = SM_SET, double value = 1.0) = 0;
};


} // namespace i2d


#endif // !i2d_ISceneController_included


