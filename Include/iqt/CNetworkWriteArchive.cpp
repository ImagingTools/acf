#include "iqt/CNetworkWriteArchive.h"


// Qt includes
#include <QAuthenticator>


namespace iqt
{


CNetworkWriteArchive::CNetworkWriteArchive(
			const QHostAddress& hostAddress,
			int port,
			const QString& userName,
			const QString& password,
			const iser::IVersionInfo* versionInfoPtr)
	:BaseClass(versionInfoPtr),
	m_userName(userName),
	m_password(password)
{
	m_socket.connectToHost(hostAddress, port);

	m_socket.waitForConnected();

	connect(
				&m_socket,
				SIGNAL(proxyAuthenticationRequired(const QNetworkProxy&, QAuthenticator*)),
				this,
				SLOT(OnAuthentificationtRequired(const QNetworkProxy&, QAuthenticator*)));
}


// reimplemented (iser::IArchive)

bool CNetworkWriteArchive::ProcessData(void* dataPtr, int size)
{
	if (size <= 0){
		return true;
	}

	if (dataPtr == NULL){
		return false;
	}

	if (m_socket.state() != QAbstractSocket::ConnectedState){
		return false;
	}

	QDataStream dataStream(&m_socket);

	dataStream.writeBytes(reinterpret_cast<char*>(dataPtr), size);
	
	if (!m_socket.waitForBytesWritten()){
		return false;
	}

	return m_socket.flush();
}


// private slots

void CNetworkWriteArchive::OnAuthentificationtRequired(const QNetworkProxy&/* proxy*/, QAuthenticator* authenticator) const
{
	if (authenticator != NULL){
		authenticator->setUser(m_userName);
		authenticator->setPassword(m_password);
	}
}


} // namespace iqt


