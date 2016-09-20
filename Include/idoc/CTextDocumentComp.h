#ifndef idoc_CTextDocumentComp_included
#define idoc_CTextDocumentComp_included


// Qt includes
#include <QtCore/QString>

// ACF includes
#include <icomp/CComponentBase.h>
#include <idoc/ITextDocument.h>


namespace idoc
{


/**
	Simple implementation of a text document model.
*/
class CTextDocumentComp: public icomp::CComponentBase, virtual public ITextDocument
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CTextDocumentComp);
		I_REGISTER_INTERFACE(iser::ISerializable);
		I_REGISTER_INTERFACE(ITextDocument);
		I_ASSIGN(m_defaultTextAttrPtr, "DefaultText", "Default text", false, "Hallo World!");
	I_END_COMPONENT;

	// reimplemented (idoc::ITextDocument)
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


} // namespace idoc



#endif // !idoc_CTextDocumentComp_included


