#include <iqt/CConsoleReader.h>


#ifdef Q_OS_WIN
// STD includes
#include <stdio.h>
#include <conio.h>
#else
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

extern "C" int _kbhit()
{
	struct termios oldt, newt;
	int ch;
	int oldf;

	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
	fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

	ch = getchar();

	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	fcntl(STDIN_FILENO, F_SETFL, oldf);

	if(ch != EOF)
	{
		ungetc(ch, stdin);
		return ch;
	}

	return 0;
}
#endif


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
	:m_parent(parent),
	m_shouldBeFinished(false)
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
	while (!m_shouldBeFinished){
		int hit = _kbhit();
		if (hit != 0){
#ifdef Q_OS_WIN
			emit m_parent.KeyPressedSignal(_getch());
#else
			emit m_parent.KeyPressedSignal(hit);
#endif
			msleep(100);
		}
	}
}


} // namespace iqt


