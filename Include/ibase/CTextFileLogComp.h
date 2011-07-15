#ifndef ibase_CTextFileLogComp_included
#define ibase_CTextFileLogComp_included


// STL includes
#include <fstream>


// ACF includes
#include "imod/CSingleModelObserverBase.h"

#include "iprm/IFileNameParam.h"

#include "ibase/CStreamLogCompBase.h"


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
		I_ASSIGN(m_fileNameCompPtr, "LogFile", "Name of the log file", true, "LogFile");
		I_ASSIGN_TO(m_fileNameModelCompPtr, m_fileNameCompPtr, true);
		I_ASSIGN(m_isAppendAttrPtr, "AppendToExisting", "Don't overwrite existing log file", false, true);
	I_END_COMPONENT;

	CTextFileLogComp();

protected:
	// reimplemented (CStreamLogCompBase)
	virtual void WriteText(const istd::CString& text);
	virtual void NewLine();

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

private:
	void OpenFileStream();

private:
	I_REF(iprm::IFileNameParam, m_fileNameCompPtr);
	I_REF(imod::IModel, m_fileNameModelCompPtr);
	I_ATTR(bool, m_isAppendAttrPtr);

	std::wofstream m_outputFileStream;

	class FilePathObserver: public imod::CSingleModelObserverBase
	{
	public:
		typedef imod::CSingleModelObserverBase BaseClass;

		FilePathObserver(CTextFileLogComp& parent);

	protected:
		// reimplemented (imod::CSingleModelObserverBase)
		virtual void OnUpdate(int updateFlags, istd::IPolymorphic* updateParamsPtr);

	private:
		CTextFileLogComp& m_parent;
	};

	FilePathObserver m_filePathObserver;
};


} // namespace ibase


#endif // !ibase_CTextFileLogComp_included

