#include <ifile/CTextFileLogComp.h>


// Qt includes
#include <QtCore/QFileInfo>
#include <QtCore/QMutexLocker>

// ACF includes
#include <istd/IInformationProvider.h>
#include <istd/CSystem.h>
#include <imod/IModel.h>


namespace ifile
{


// public methods

CTextFileLogComp:: CTextFileLogComp()
	:m_outputFile(),
	m_outputFileStream(&m_outputFile),
	m_filePathObserver(*this)
#if QT_VERSION < 0x060000
	,m_mutex(QMutex::Recursive)
#endif
{

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
	m_outputFileStream.setCodec("UTF-8");
#else
	m_outputFileStream.setEncoding(QStringConverter::Utf8);
#endif

	m_lastDay = QDate::currentDate().day();
}


// protected methods

// reimplemented (CStreamLogCompBase)

void CTextFileLogComp::WriteText(const QString& text, istd::IInformationProvider::InformationCategory /*category*/)
{
	static const int s_oneMinute = 60000;

	QMutexLocker lock(&m_mutex);

	// check the day overlap
	int day = QDate::currentDate().day();
	if (m_lastDay != day){
		m_lastDay = day;
		bool ok = OpenFileStream();
		if (!ok){
			m_tryTimer.start(5 * s_oneMinute);
		}
	}

	if (m_outputFile.isOpen()){
		try{
			m_outputFileStream << text;
			m_outputFileStream.flush();
		}
		catch(...){
			m_outputFileStream.reset();
			m_outputFile.close();
		}
	}
}


// reimplemented (icomp::CComponentBase)

void CTextFileLogComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	if (m_fileNameCompPtr.IsValid() && m_fileNameModelCompPtr.IsValid()){
		m_fileNameModelCompPtr->AttachObserver(&m_filePathObserver);
	}

	connect(&m_tryTimer, SIGNAL(timeout()), this, SLOT(OnTryTimer()));

	OpenFileStream();
}


void CTextFileLogComp::OnComponentDestroyed()
{
	m_tryTimer.stop();

	if (m_fileNameModelCompPtr.IsValid() && m_fileNameModelCompPtr->IsAttached(&m_filePathObserver)){
		m_fileNameModelCompPtr->DetachObserver(&m_filePathObserver);
	}

	CloseFileStream();

	BaseClass::OnComponentDestroyed();
}


// private methods

bool CTextFileLogComp::OpenFileStream()
{
	QMutexLocker lock(&m_mutex);

	bool success = false;

	if (m_outputFile.isOpen()){
		m_outputFile.flush();

		m_outputFile.close();
	}

	if (m_fileNameCompPtr.IsValid()){
		QString logFilePath = m_fileNameCompPtr->GetPath();

		if (logFilePath.isEmpty()){
			return false;
		}

		QIODevice::OpenMode openMode = QIODevice::Text | QIODevice::WriteOnly;

		if (*m_isAppendAttrPtr){
			openMode |= QIODevice::Append;
		}

		QFileInfo fileInfo(logFilePath);

		istd::CSystem::EnsurePathExists(fileInfo.absolutePath());

		m_outputFile.setFileName(logFilePath);
		success = m_outputFile.open(openMode);
	}

	return success;
}


void CTextFileLogComp::CloseFileStream()
{
	QMutexLocker lock(&m_mutex);

	m_outputFile.close();
}


// private slots

void CTextFileLogComp::OnTryTimer()
{
	OpenFileStream();
}


// embedded class FilePathObserver

// public methods

CTextFileLogComp::FilePathObserver::FilePathObserver(CTextFileLogComp& parent)
	:m_parent(parent)
{
}


// protected methods

// reimplemented (imod::CSingleModelObserverBase)

void CTextFileLogComp::FilePathObserver::OnUpdate(const istd::IChangeable::ChangeSet& /*changeSet*/)
{
	m_parent.OpenFileStream();
}


} // namespace ifile


