#ifndef icmpstr_CNote_included
#define icmpstr_CNote_included


#include "icmpstr/IComponentNote.h"


namespace icmpstr
{


class CNote: virtual public IComponentNote
{
public:
	// reimplemented (IComponentNote)
	virtual istd::CString GetText() const;
	virtual void SetText(const istd::CString& noteText);
	virtual i2d::CVector2d GetPosition() const;
	virtual void SetPosition(const i2d::CVector2d& position);

	// reimplemented (ibase::IObject)
	virtual istd::CString& GetFactoryId() const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

private:
	istd::CString m_noteText;
	i2d::CVector2d m_position;
};


} // namespace icmpstr


#endif // !icmpstr_CNote_included

