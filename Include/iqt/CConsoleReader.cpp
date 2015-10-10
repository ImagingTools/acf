#include <iqt/CConsoleReader.h>


// STD includes
#include <stdio.h>


namespace iqt
{


// public methods

CConsoleReader::CConsoleReader(QObject* parentPtr)
	:BaseClass(parentPtr)
{
}


void CConsoleReader::Start()
{
	BaseClass::start();
}


void CConsoleReader::Stop()
{
	m_shouldBeFinished = true;

	wait();
}


bool CConsoleReader::IsRunning() const
{
	return BaseClass::isRunning();
}


// protected methods

void CConsoleReader::run()
{
	while (!m_shouldBeFinished)
	{
		char key = getchar();

		emit KeyPressedSignal(key);
	}
}


} // namespace iqt


