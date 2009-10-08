#ifndef iqt_CNetworkWriteArchive_included
#define iqt_CNetworkWriteArchive_included


// Qt includes
#include <QTcpSocket>
#include <QHostAddress>


// ACF includes
#include "iser/CMemoryWriteArchive.h"

#include "iqt/iqt.h"


namespace iqt
{


class CNetworkWriteArchive: public QObject, public iser::CMemoryWriteArchive
{
	Q_OBJECT
public:
	typedef iser::CMemoryWriteArchive BaseClass;

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
	virtual bool ProcessData(void* dataPtr, int size);
	
private Q_SLOTS:
	void OnAuthentificationtRequired(const QNetworkProxy& proxy, QAuthenticator* authenticator) const;

private:
	QTcpSocket m_socket;
	QString m_userName;
	QString m_password;
};


} // namespace iqt


#endif // !iqt_CNetworkWriteArchive_included


