#ifndef ibase_CTextDocumentComp_included
#define ibase_CTextDocumentComp_included


// ACF includes
#include <QString>

#include "icomp/CComponentBase.h"

#include "ibase/ITextDocument.h"


namespace ibase
{


/**
	Simple implementation of a text document model.
*/
class CTextDocumentComp: public icomp::CComponentBase, virtual public ibase::ITextDocument
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CTextDocumentComp);
		I_REGISTER_INTERFACE(iser::ISerializable);
		I_REGISTER_INTERFACE(ibase::ITextDocument);
		I_ASSIGN(m_defaultTextAttrPtr, "DefaultText", "Default text", false, "Hallo World!");
	I_END_COMPONENT;

	// reimplemented (ibase::ITextDocument)
	virtual QString GetText() const;
	virtual void SetText(const QString& text);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();

private:
	QString m_text;

	I_ATTR(QString, m_defaultTextAttrPtr);
};


} // namespace ibase



#endif // !ibase_CTextDocumentComp_included


