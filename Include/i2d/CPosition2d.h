#ifndef i2d_CPosition2d_included
#define i2d_CPosition2d_included


#include "i2d/IObject2d.h"
#include "i2d/CVector2d.h"


namespace i2d
{	


/**
	Definition of graphical point object.
*/
class CPosition2d: virtual public IObject2d
{
public:
	CPosition2d();
	CPosition2d(const CVector2d& center);

	const CVector2d& GetPosition() const;
	void SetPosition(const CVector2d& position);

	// reimplemented (i2d::IObject2d)
	virtual CVector2d GetCenter() const;
	virtual void MoveCenterTo(const CVector2d& position);
	virtual bool Transform(
				const ITransformation2d& transformation,
				ITransformation2d::ExactnessMode mode = ITransformation2d::EM_NONE,
				double* errorFactorPtr = NULL);
	virtual bool InvTransform(
				const ITransformation2d& transformation,
				ITransformation2d::ExactnessMode mode = ITransformation2d::EM_NONE,
				double* errorFactorPtr = NULL);
	virtual bool GetTransformed(
				const ITransformation2d& transformation,
				IObject2d& result,
				ITransformation2d::ExactnessMode mode = ITransformation2d::EM_NONE,
				double* errorFactorPtr = NULL) const;
	virtual bool GetInvTransformed(
				const ITransformation2d& transformation,
				IObject2d& result,
				ITransformation2d::ExactnessMode mode = ITransformation2d::EM_NONE,
				double* errorFactorPtr = NULL) const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

protected:
	CVector2d m_position;
};


// inline methods

inline const CVector2d& CPosition2d::GetPosition() const
{
	return m_position;
}


} // namespace i2d


#endif // !i2d_CPosition2d_included

