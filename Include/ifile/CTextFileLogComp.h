#pragma once


// Qt includes
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtCore/QMutex>
#include <QtCore/QTimer>

// ACF includes
#include <imod/IModel.h>
#include <imod/CSingleModelObserverBase.h>
#include <ifile/IFileNameParam.h>
#include <ilog/CStreamLogCompBase.h>


namespace ifile
{


/**
	Message container displaying messages as log list.

	\ingroup Logging
*/
class CTextFileLogComp: public ilog::CStreamLogCompBase
{
	Q_OBJECT
public:
	typedef ilog::CStreamLogCompBase BaseClass;

	I_BEGIN_COMPONENT(CTextFileLogComp);
		I_ASSIGN(m_fileNameCompPtr, "LogFile", "Name of the log file", true, "LogFile");
		I_ASSIGN_TO(m_fileNameModelCompPtr, m_fileNameCompPtr, true);
		I_ASSIGN(m_isAppendAttrPtr, "AppendToExisting", "Don't overwrite existing log file", true, false);
	I_END_COMPONENT;

	CTextFileLogComp();

protected:
	// reimplemented (CStreamLogCompBase)
	virtual void WriteText(const QString& text, istd::IInformationProvider::InformationCategory category) override;

	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated() override;
	virtual void OnComponentDestroyed() override;

private:
	bool OpenFileStream();
	void CloseFileStream();

private Q_SLOTS:
	void OnTryTimer();

private:
	I_REF(ifile::IFileNameParam, m_fileNameCompPtr);
	I_REF(imod::IModel, m_fileNameModelCompPtr);
	I_ATTR(bool, m_isAppendAttrPtr);

	QFile m_outputFile;
	QTextStream m_outputFileStream;
	QTimer m_tryTimer;

	class FilePathObserver: public imod::CSingleModelObserverBase
	{
	public:
		typedef imod::CSingleModelObserverBase BaseClass;

		explicit FilePathObserver(CTextFileLogComp& parent);

	protected:
		// reimplemented (imod::CSingleModelObserverBase)
		virtual void OnUpdate(const istd::IChangeable::ChangeSet& changeSet) override;

	private:
		CTextFileLogComp& m_parent;
	};

	FilePathObserver m_filePathObserver;

	int m_lastDay;
#if QT_VERSION >= 0x060000
	mutable QRecursiveMutex m_mutex;
#else
	mutable QMutex m_mutex;
#endif
};


} // namespace ifile


