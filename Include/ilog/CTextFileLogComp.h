#ifndef ilog_CTextFileLogComp_included
#define ilog_CTextFileLogComp_included


// Qt includes
#include <QtCore/QFile>
#include <QtCore/QTextStream>

// ACF includes
#include "imod/CSingleModelObserverBase.h"
#include "ifile/IFileNameParam.h"
#include "ilog/CStreamLogCompBase.h"


namespace ilog
{


/**
	Message container displaying messages as log list.

	\ingroup Logging
*/
class CTextFileLogComp: public CStreamLogCompBase
{
public:
	typedef CStreamLogCompBase BaseClass;

	I_BEGIN_COMPONENT(CTextFileLogComp);
		I_ASSIGN(m_fileNameCompPtr, "LogFile", "Name of the log file", true, "LogFile");
		I_ASSIGN_TO(m_fileNameModelCompPtr, m_fileNameCompPtr, true);
		I_ASSIGN(m_isAppendAttrPtr, "AppendToExisting", "Don't overwrite existing log file", true, false);
	I_END_COMPONENT;

	CTextFileLogComp();

protected:
	// reimplemented (CStreamLogCompBase)
	virtual void WriteText(const QString& text);
	virtual void NewLine();

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated();
	virtual void OnComponentDestroyed();

private:
	void OpenFileStream();

private:
	I_REF(ifile::IFileNameParam, m_fileNameCompPtr);
	I_REF(imod::IModel, m_fileNameModelCompPtr);
	I_ATTR(bool, m_isAppendAttrPtr);

	QFile m_outputFile;
	QTextStream m_outputFileStream;

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


} // namespace ilog


#endif // !ilog_CTextFileLogComp_included


