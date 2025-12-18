#ifndef iqt_CNetworkWriteArchive_included
#define iqt_CNetworkWriteArchive_included


// Qt includes
#include <QtCore/QObject>
#include <QtNetwork/QTcpSocket>

// ACF includes
#include <iser/CMemoryWriteArchive.h>
#include <iqt/iqt.h>


namespace iqt
{

	
/**
	A write archive working over network connection.

	\ingroup Persistence
*/
class CNetworkWriteArchive:
			public QObject,
			public iser::CMemoryWriteArchive
{
	Q_OBJECT

public:
	typedef iser::CMemoryWriteArchive BaseClass;

	/**
		Constructor.
		
		\param	hostAddress	   	The host address.
		\param	port		   	The used communication port.
		\param	userName	   	(Optional) name of the user.
		\param	password	   	(Optional) the password.
		\param	versionInfoPtr 	(Optional) pointer to the version information instance.
		\param	serializeHeader	(Optional) true to serialize the archive's header.
	*/
	CNetworkWriteArchive(
				const QString& hostAddress,
				int port,
				const QString& userName = QString(),
				const QString& password = QString(),
				const iser::IVersionInfo* versionInfoPtr = NULL,
				bool serializeHeader = true);

	~CNetworkWriteArchive();

	virtual bool Flush();

	// reimplemented (iser::IArchive)
	virtual bool ProcessData(void* dataPtr, int size) override;
	
private Q_SLOTS:
	void OnAuthentificationtRequired(const QNetworkProxy& proxy, QAuthenticator* authenticator) const;

private:
	QTcpSocket m_socket;
	QString m_userName;
	QString m_password;
};


} // namespace iqt


#endif // !iqt_CNetworkWriteArchive_included


