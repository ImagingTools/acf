#ifndef i2d_CAffineTransformation2d_included
#define i2d_CAffineTransformation2d_included


// ACF includes
#include "i2d/ITransformation2d.h"
#include "i2d/CAffine2d.h"


namespace i2d
{


/**
	Affine transformation for 2D spaces.
*/
class CAffineTransformation2d: virtual public ITransformation2d
{
public:
	/**
		Get used transformation object.
	*/
	const i2d::CAffine2d& GetTransformation() const;

	/**
		Set the tranformation object
	*/
	void SetTransformation(const i2d::CAffine2d& transformation);

	/**
		Set to identity transformation.
	*/
	void Reset();

	/**
		Set this transformation to translation.
	*/
	void Reset(const CVector2d& translation);

	/**
		Set this transformation to translation, rotation and scaling.
	*/
	void Reset(const CVector2d& translation, double angle, double scale = 1.0);

	/**
		Set this transformation to translation, rotation and both axis scaling.
	*/
	void Reset(const CVector2d& translation, double angle, const CVector2d& scale);

	// reimplemented (i2d::ITransformation2d)
	virtual int GetTransformationFlags() const;
	virtual bool GetDistance(
				const CVector2d& origPos1,
				const CVector2d& origPos2,
				double& result,
				ExactnessMode mode = EM_NONE) const;
	virtual bool GetPositionAt(
				const CVector2d& origPosition,
				CVector2d& result,
				ExactnessMode mode = EM_NONE) const;
	virtual bool GetInvPositionAt(
				const CVector2d& transfPosition,
				CVector2d& result,
				ExactnessMode mode = EM_NONE) const;
	virtual bool GetLocalTransform(
				const CVector2d& origPosition,
				CAffine2d& result,
				ExactnessMode mode = EM_NONE) const;
	virtual bool GetLocalInvTransform(
				const CVector2d& transfPosition,
				CAffine2d& result,
				ExactnessMode mode = EM_NONE) const;
	virtual const ITransformation2d* CreateCombinedTransformation(const ITransformation2d& transform) const;

	// reimplemented (imath::TISurjectFunction)
	virtual bool GetInvValueAt(const CVector2d& argument, CVector2d& result) const;
	virtual CVector2d GetInvValueAt(const CVector2d& argument) const;

	// reimplemented (imath::TIMathFunction)
	virtual bool GetValueAt(const CVector2d& argument, CVector2d& result) const;
	virtual CVector2d GetValueAt(const CVector2d& argument) const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

protected:
	CAffine2d m_transformation;
};


} // namespace i2d


#endif // !i2d_CAffineTransformation2d_included


