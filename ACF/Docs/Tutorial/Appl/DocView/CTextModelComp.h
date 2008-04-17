#ifndef CTextModelComp_included
#define CTextModelComp_included


#include <QObject>

#include "istd/CString.h"

#include "icomp/CComponentBase.h"

#include "idoc/ICommandsProvider.h"

#include "iqt/CHierarchicalCommand.h"

#include "IText.h"


class CTextModelComp: public QObject, public icomp::CComponentBase, public IText, public idoc::ICommandsProvider
{
	Q_OBJECT

public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CTextModelComp)
		I_ASSIGN(m_defaultTextAttrPtr, "DefaultText", "Default text", false, "Hallo World!")
	I_END_COMPONENT

	CTextModelComp();

	// reimplemented (IText)
	virtual istd::CString GetText() const;
	virtual void SetText(const istd::CString& text);

	// reimplemented (idoc::ICommandsProvider)
	virtual const idoc::IHierarchicalCommand* GetCommands() const;

	// reimplemented (icomp::IComponent)
	virtual void OnComponentCreated();

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

protected slots:
	void OnToLowercase();
	void OnToUppercase();

private:
	istd::CString m_text;

	iqt::CHierarchicalCommand m_rootCommand;
	iqt::CHierarchicalCommand m_editorCommand;
	iqt::CHierarchicalCommand m_lowercaseCommand;
	iqt::CHierarchicalCommand m_uppercaseCommand;

	I_ATTR(istd::CString, m_defaultTextAttrPtr);
};


#endif // !CTextModelComp_included


