#ifndef CTextModel_included
#define CTextModel_included


#include "iser/ISerializable.h"

#include "istd/CString.h"


class CTextModel: public iser::ISerializable
{
public:
	CTextModel();

	virtual istd::CString GetText() const;
	virtual void SetText(const istd::CString& text);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

private:
	istd::CString m_text;
};

#endif // !CTextModel_included
