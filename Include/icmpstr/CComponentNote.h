#ifndef CComponentNote_included
#define CComponentNote_included


#include "icmpstr/IComponentNote.h"


namespace icmpstr
{


class CComponentNote: virtual public IComponentNote
{
public:
	virtual void SetText(const istd::CString& noteText);
	virtual void SetPosition(const i2d::CVector2d& position);

	// reimplemented (IComponentNote)
	virtual istd::CString GetText() const;
	virtual i2d::CVector2d GetPosition() const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

private:
	istd::CString m_noteText;
	i2d::CVector2d m_position;
};


} // namespace icmpstr


#endif // !CComponentNote_included

