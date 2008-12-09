#ifndef ibase_CTextDocumentComp_included
#define ibase_CTextDocumentComp_included


// ACF includes
#include "istd/CString.h"

#include "icomp/CComponentBase.h"

#include "ibase/ITextDocument.h"


namespace ibase
{


class CTextDocumentComp: public icomp::CComponentBase, virtual public ibase::ITextDocument
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CTextDocumentComp)
		I_REGISTER_INTERFACE(iser::ISerializable);
		I_ASSIGN(m_defaultTextAttrPtr, "DefaultText", "Default text", false, "Hallo World!")
	I_END_COMPONENT

	// reimplemented (ibase::ITextDocument)
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


} // namespace ibase



#endif // !ibase_CTextDocumentComp_included


