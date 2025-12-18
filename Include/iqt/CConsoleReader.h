#pragma once


// Qt includes
#include <QtCore/QThread>


namespace iqt
{


/**
	User-input reader for command line console.
*/
class CConsoleReader: public QObject
{
	Q_OBJECT

public:
	typedef QObject BaseClass;

	explicit CConsoleReader(QObject* parentPtr = NULL);

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
	class InputObserver: public QThread
	{
	public:
		explicit InputObserver(CConsoleReader& parent);

		void Stop();

	protected:
		// reimplemented (QThread)
		virtual void run() override;

	private:
		CConsoleReader& m_parent;
		bool m_shouldBeFinished;
	};

private:
	InputObserver m_inputObserver;
};


} // namespace iqt


