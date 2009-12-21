#ifndef ibase_CTextFileLogComp_included
#define ibase_CTextFileLogComp_included


#include "ibase/CStreamLogCompBase.h"


// STL includes
#include <fstream>


namespace ibase
{


/**
	Message container displaying messages as log list.
*/
class CTextFileLogComp: public CStreamLogCompBase
{
public:
	typedef CStreamLogCompBase BaseClass;

	I_BEGIN_COMPONENT(CTextFileLogComp);
		I_ASSIGN(m_fileNameAttrPtr, "LogFile", "Name of the log file", true, "Log.txt")
		I_ASSIGN(m_isAppendAttrPtr, "AppendToExisting", "Don't overwrite existing log file", false, true)
	I_END_COMPONENT;

protected:
	// reimplemented (CStreamLogCompBase)
	virtual void WriteText(const istd::CString& text);
	virtual void NewLine();

	// reimplemented (icomp::IComponent)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

private:
	I_ATTR(istd::CString, m_fileNameAttrPtr);
	I_ATTR(bool, m_isAppendAttrPtr);

	std::wofstream m_outputFileStream;
};


} // namespace ibase


#endif // !ibase_CTextFileLogComp_included

