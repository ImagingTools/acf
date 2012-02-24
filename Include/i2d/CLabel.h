#ifndef i2d_CLabel_included
#define i2d_CLabel_included


// ACF includes
#include "istd/CIndex2d.h"

#include "iser/CArchiveTag.h"

#include "i2d/CPosition2d.h"


namespace i2d
{


class CLabel: public i2d::CPosition2d
{
public:
	typedef i2d::CPosition2d BaseClass;

	CLabel(const i2d::CVector2d& position = i2d::CVector2d(0, 0), const QString& labelText = "Label");

	virtual const QString& GetText() const;
	virtual void SetText(const QString& labelText);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

private:
	QString m_text;
};


// inline methods

inline const QString& CLabel::GetText() const
{
	return m_text;
}


} // namespace i2d


#endif // !i2d_CLabel_included


