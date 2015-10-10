#include <iqt/CConsoleReader.h>


// STD includes
#include <stdio.h>


namespace iqt
{


// public methods

CConsoleReader::CConsoleReader(QObject* parentPtr)
	:BaseClass(parentPtr),
	m_inputObserver(*this)
{
}


void CConsoleReader::Start()
{
	m_inputObserver.start();
}


void CConsoleReader::Stop()
{
	m_inputObserver.Stop();
}


bool CConsoleReader::IsRunning() const
{
	return m_inputObserver.isRunning();
}


// public methods of the embedded class InputObserver

CConsoleReader::InputObserver::InputObserver(CConsoleReader& parent)
	:m_parent(parent)
{
}


void CConsoleReader::InputObserver::Stop()
{
	m_shouldBeFinished = true;

	wait();
}


// protected methods of the embedded class InputObserver

// reimplemented (QThread)

void CConsoleReader::InputObserver::run()
{
	while (!m_shouldBeFinished)
	{
		char key = getchar();

		emit m_parent.KeyPressedSignal(key);
	}
}


} // namespace iqt


