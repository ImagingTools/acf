#pragma once


// Qt includes
#include <QtCore/QThread>


namespace iqt
{


/**
	User-input reader for command line console.
*/
class CConsoleReader: protected QThread
{
	Q_OBJECT

public:
	typedef QThread BaseClass;

	CConsoleReader(QObject* parentPtr = NULL);

	/**
		Start reading of the user input on the console.
	*/
	virtual void Start();

	/**
		Stop reading of the user input on the console.
	*/
	virtual void Stop();

	/**
		Check if the reader is running.
	*/
	bool IsRunning() const;

signals:
	/**
		Emit typed character.
	*/
	void KeyPressedSignal(char character);

protected:
	// reimplemented (QThread)
	virtual void run();

private:
	bool m_shouldBeFinished;
};


} // namespace iqt


