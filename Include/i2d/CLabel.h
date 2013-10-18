#ifndef i2d_CLabel_included
#define i2d_CLabel_included


// ACF includes
#include "istd/CIndex2d.h"

#include "i2d/CPosition2d.h"


namespace i2d
{


/**
	Data model for the label shape.
*/
class CLabel: public i2d::CPosition2d
{
public:
	typedef i2d::CPosition2d BaseClass;

	/**
		Construct the label object with predefined parameters.
		\param position	Center position of the label.
		\param labelText Label text.	
	*/
	CLabel(const i2d::CVector2d& position = i2d::CVector2d(0, 0), const QString& labelText = "Label");

	/**
		Get label text.
	*/
	virtual const QString& GetText() const;

	/**
		Set label text.
	*/
	virtual void SetText(const QString& labelText);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

	// reimplemented istd::IChangeable
	virtual int GetSupportedOperations() const;
	virtual bool CopyFrom(const IChangeable& object, CompatibilityMode mode = CM_WITHOUT_REFS);
	virtual istd::IChangeable* CloneMe(CompatibilityMode mode = CM_WITHOUT_REFS) const;

private:
	QString m_text;
};


// public inline methods

inline const QString& CLabel::GetText() const
{
	return m_text;
}


} // namespace i2d


#endif // !i2d_CLabel_included


