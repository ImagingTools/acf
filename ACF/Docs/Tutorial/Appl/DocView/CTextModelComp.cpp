#include "CTextModelComp.h"

#include "iser/IArchive.h"
#include "iser/CArchiveTag.h"

#include "istd/TChangeNotifier.h"


CTextModelComp::CTextModelComp()
:	m_editorCommand("Editor"),
	m_lowercaseCommand("To Lowercase"),
	m_uppercaseCommand("To Uppercase")
{
	m_editorCommand.InsertChild(&m_lowercaseCommand, false);
	m_editorCommand.InsertChild(&m_uppercaseCommand, false);
	m_rootCommand.InsertChild(&m_editorCommand, false);

	connect(&m_lowercaseCommand, SIGNAL(activated()), this, SLOT(OnToLowercase()));
	connect(&m_uppercaseCommand, SIGNAL(activated()), this, SLOT(OnToUppercase()));
}


istd::CString CTextModelComp::GetText() const
{
	return m_text;
}


void CTextModelComp::SetText(const istd::CString& text)
{
	if (m_text != text){
		istd::CChangeNotifier changePtr(this);

		m_text = text;
	}
}


// reimplemented (idoc::ICommandsProvider)
const idoc::IHierarchicalCommand* CTextModelComp::GetCommands() const
{
	return &m_rootCommand;
}


// reimplemented (icomp::IComponent)

void CTextModelComp::OnComponentCreated()
{
	if (m_defaultTextAttrPtr.IsValid()){
		m_text = *m_defaultTextAttrPtr;
	}
}


// reimplemented (iser::ISerializable)

bool CTextModelComp::Serialize(iser::IArchive& archive)
{
	static iser::CArchiveTag textTag("Text", "Text");
	bool retVal = archive.BeginTag(textTag);
	retVal = retVal && archive.Process(m_text);
	retVal = retVal && archive.EndTag(textTag);

	return retVal;
}


// protected slots

void CTextModelComp::OnToLowercase()
{
	istd::CChangeNotifier changePtr(this);

	m_text = m_text.ToLower();
}


void CTextModelComp::OnToUppercase()
{
	istd::CChangeNotifier changePtr(this);

	m_text = m_text.ToUpper();
}

