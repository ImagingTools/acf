#ifndef CNote_included
#define CNote_included


#include "icmpstr/IComponentNote.h"


namespace icmpstr
{


class CNote: virtual public IComponentNote
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


#endif // !CNote_included

