#ifndef iqt_CNetworkWriteArchive_included
#define iqt_CNetworkWriteArchive_included


// Qt includes
#include <QTcpSocket>
#include <QHostAddress>


// ACF includes
#include "iser/CBinaryWriteArchiveBase.h"


namespace iqt
{


class CNetworkWriteArchive: public QObject, public iser::CBinaryWriteArchiveBase
{
	Q_OBJECT
public:
	typedef iser::CBinaryWriteArchiveBase BaseClass;

	CNetworkWriteArchive(
				const QHostAddress& hostAddress,
				int port,
				const QString& userName = QString(),
				const QString& password = QString(),
				const iser::IVersionInfo* versionInfoPtr = NULL);

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


