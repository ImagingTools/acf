#ifndef i2d_ITransformation2d_included
#define i2d_ITransformation2d_included


// ACF includes
#include "iser/ISerializable.h"

#include "imath/TISurjectFunction.h"

#include "i2d/CVector2d.h"
#include "i2d/CMatrix2d.h"


namespace i2d
{


class CAffine2d;


/**
	Common interface for all calibration objects.
	It is used to work with real world units.
	It provides only two simple operations: distance computation and local
	transformation.
*/
class ITransformation2d:
			virtual public imath::TISurjectFunction<CVector2d, CVector2d>,
			virtual public iser::ISerializable
{
public:
	enum ExactnessMode
	{
		/**
			There is no preferencies, should be automatically selected.
		*/
		EM_NONE,
		/**
			Only precise calculation should be done.
		*/
		EM_EXACT,
		/**
			Only visual similarity of transformation is important.
		*/
		EM_VISUAL,
		/**
			There is only important, that some result will be outputted.
		*/
		EM_ANY
	};

	enum TransformationFlags
	{
		/**
			There are some arguments (inputs) where the transformation function is defined.
		*/
		TF_FORWARD = 1 << 0,
		/**
			There are some outputs where the inverted transformation is defined.
		*/
		TF_INVERTED = 1 << 1,
		/**
			Function in injective, it means for each input values the output value is defined.
		*/
		TF_INJECTIVE = 1 << 2,
		/**
			Function in surjective, it means there is at least one input value for each outputs.
		*/
		TF_SURJECTIVE = 1 << 3,
		/**
			This transformation preserve position [0, 0].
		*/
		TF_PRESERVE_NULL = 1 << 4,
		/**
			This transformation preserve distance.
		*/
		TF_PRESERVE_DISTANCE = 1 << 5,
		/**
			This transformation preserve angles.
		*/
		TF_PRESERVE_ANGLE = 1 << 6,
		/**
			This is affine transformation.
		*/
		TF_AFFINE = 1 << 7,
		/**
			This transformation is represented by continues function.
		*/
		TF_CONTINUES = 1 << 8
	};

	/**
		Get flags of describing type of this transformation.
	*/
	virtual int GetTransformationFlags() const = 0;

	/**
		Compute a shorteness distance in transformed coordinations.
		\param	origPos1	first position in original coordinates.
		\param	origPos2	second position in original coordinates.
		\param	result		object where result distance between positions in transformed coordinates will be stored.
		\param	mode			controls needed transformation exactness.
		\return				true, if calculation done correctly.
	*/
	virtual bool GetDistance(
				const CVector2d& origPos1,
				const CVector2d& origPos2,
				double& result,
				ExactnessMode mode = EM_NONE) const = 0;

	/**
		Get position after transformation.
		\param	origPosition	position in (original coordination system).
		\param	result			result position (transformed coordination system).
		\param	mode			controls needed transformation exactness.
		\return					true, if calculation done correctly.
	 */
	virtual bool GetPositionAt(
				const CVector2d& origPosition,
				CVector2d& result,
				ExactnessMode mode = EM_NONE) const = 0;

	/**
		Get position after transformation.
		\param	transfPosition	position in transformed coordination system.
		\param	result			result position (original coordination system).
		\param	mode			controls needed transformation exactness.
		\return					true, if calculation done correctly.
	 */
	virtual bool GetInvPositionAt(
				const CVector2d& transfPosition,
				CVector2d& result,
				ExactnessMode mode = EM_NONE) const = 0;

	/**
		Get local transformation (from original to transformed coordinate system) at some original position.
		\param	origPosition	position in (original coordination system).
		\param	result			transformation for specified position.
		\param	mode			controls needed transformation exactness.
		\return					true, if calculation done correctly.
	*/
	virtual bool GetLocalTransform(
				const CVector2d& origPosition,
				CAffine2d& result,
				ExactnessMode mode = EM_NONE) const = 0;

	/**
		Get local transformation (from transformed to original coordinate system) at some transformed position.
		\param	transfPosition	position in transformed coordination system.
		\param	result			inverse transformation for specified position.
		\param	mode			controls needed transformation exactness.
		\return					true, if calculation done correctly.
	*/
	virtual bool GetLocalInvTransform(
				const CVector2d& transfPosition,
				CAffine2d& result,
				ExactnessMode mode = EM_NONE) const = 0;
};


} // namespace i2d


#endif // !i2d_ITransformation2d_included


