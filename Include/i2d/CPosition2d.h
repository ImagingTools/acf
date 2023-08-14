#ifndef i2d_CPosition2d_included
#define i2d_CPosition2d_included


// ACF includes
#include <i2d/CObject2dBase.h>
#include <i2d/CVector2d.h>


namespace i2d
{


/**
	Definition of graphical point object.
*/
class CPosition2d: public CObject2dBase
{
public:
	static QByteArray GetTypeName();

	CPosition2d();
	CPosition2d(const CVector2d& center);

	const CVector2d& GetPosition() const;
	void SetPosition(const CVector2d& position);
	void Translate(const i2d::CVector2d& vector); 
	CPosition2d GetTranslated(const i2d::CVector2d& vector) const; 

	// reimplemented (i2d::IObject2d)
	virtual CVector2d GetCenter() const override;
	virtual void MoveCenterTo(const CVector2d& position) override;
	virtual CRectangle GetBoundingBox() const override;
	virtual bool Transform(
				const ITransformation2d& transformation,
				ITransformation2d::ExactnessMode mode = ITransformation2d::EM_NONE,
				double* errorFactorPtr = NULL) override;
	virtual bool InvTransform(
				const ITransformation2d& transformation,
				ITransformation2d::ExactnessMode mode = ITransformation2d::EM_NONE,
				double* errorFactorPtr = NULL) override;
	virtual bool GetTransformed(
				const ITransformation2d& transformation,
				IObject2d& result,
				ITransformation2d::ExactnessMode mode = ITransformation2d::EM_NONE,
				double* errorFactorPtr = NULL) const override;
	virtual bool GetInvTransformed(
				const ITransformation2d& transformation,
				IObject2d& result,
				ITransformation2d::ExactnessMode mode = ITransformation2d::EM_NONE,
				double* errorFactorPtr = NULL) const override;

	// reimplemented (iser::IObject)
	virtual QByteArray GetFactoryId() const override;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive) override;

	// reimplemented (istd::IChangeable)
	virtual int GetSupportedOperations() const override;
	virtual bool CopyFrom(const IChangeable& object, CompatibilityMode mode = CM_WITHOUT_REFS) override;
	virtual bool IsEqual(const istd::IChangeable& object) const override;
	virtual istd::IChangeable* CloneMe(CompatibilityMode mode = CM_WITHOUT_REFS) const override;

protected:
	CVector2d m_position;
};


// inline methods

inline const CVector2d& CPosition2d::GetPosition() const
{
	return m_position;
}


inline void CPosition2d::Translate(const i2d::CVector2d& vector)
{
	m_position += vector;
}


inline CPosition2d CPosition2d::GetTranslated(const i2d::CVector2d& vector) const
{
	CPosition2d translatedPosition = *this;

	translatedPosition.Translate(vector);

	return translatedPosition;
}


} // namespace i2d


#endif // !i2d_CPosition2d_included

