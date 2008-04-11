#ifndef CTextModelComp_included
#define CTextModelComp_included


#include "istd/CString.h"

#include "icomp/CComponentBase.h"

#include "IText.h"


class CTextModelComp: public icomp::CComponentBase, public IText
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CTextModelComp)
		I_ASSIGN(m_defaultTextAttrPtr, "DefaultText", "Default text", false, "Hallo World!")
	I_END_COMPONENT

	CTextModelComp();

	// reimplemented (IText)
	virtual istd::CString GetText() const;
	virtual void SetText(const istd::CString& text);

	// reimplemented (icomp::IComponent)
	virtual void OnComponentCreated();

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

private:
	istd::CString m_text;

	I_ATTR(istd::CString, m_defaultTextAttrPtr);
};


#endif // !CTextModelComp_included


