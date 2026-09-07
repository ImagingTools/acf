// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#pragma once


// Qt includes
#include <QtCore/QString>

// ACF includes
#include <icomp/CComponentBase.h>
#include <idoc/ITextDocument.h>


namespace idoc
{


/**
	Simple implementation of a text document model.
	
	This component provides a complete implementation of the ITextDocument interface,
	storing plain text content. It supports serialization and can be used as the document
	model in text editing applications.
	
	\par Component Attributes
	- \b DefaultText - Initial text content when document is created (default: "Hallo World!")
	
	\par Registered Interfaces
	- iser::ISerializable - For saving/loading document state
	- ITextDocument - For text access and manipulation
	
	\par Configuration Example
	\code
	CTextDocumentComp {
		DefaultText = "Type your text here..."
	}
	\endcode
	
	\par Usage with Document Template
	\code
	CSingleDocumentTemplateComp {
		DocumentTypeId = "TextDocument"
		DocumentFactory = CTextDocumentComp {
			DefaultText = "New Document"
		}
		ViewFactory = TextEditorView
		DocumentLoader = CTextFileLoaderComp
	}
	\endcode
	
	\sa ITextDocument, CTextFileLoaderComp
	\ingroup DocumentBasedFramework
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
	virtual QString GetText() const override;
	virtual void SetText(const QString& text) override;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive) override;

protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated() override;

private:
	QString m_text;

	I_ATTR(QString, m_defaultTextAttrPtr);
};


} // namespace idoc





