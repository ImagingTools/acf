#include "iqt/CNetworkWriteArchive.h"


// Qt includes
#include <QAuthenticator>


namespace iqt
{


CNetworkWriteArchive::CNetworkWriteArchive(
			const QString& hostAddress,
			int port,
			const QString& userName,
			const QString& password,
			const iser::IVersionInfo* versionInfoPtr,
			bool serializeHeader)
	:BaseClass(versionInfoPtr, serializeHeader),
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


CNetworkWriteArchive::~CNetworkWriteArchive()
{
	Flush();

	m_socket.disconnectFromHost();
	
	m_socket.waitForDisconnected();
}


bool CNetworkWriteArchive::Flush()
{
	if (m_socket.state() != QAbstractSocket::ConnectedState){
		return false;
	}

	QDataStream dataStream(&m_socket);

	dataStream << int(GetBufferSize());

	int written = dataStream.writeRawData(reinterpret_cast<const char*>(GetBuffer()), GetBufferSize());
	if (written != GetBufferSize()){
		return false;
	}
	
	if (!m_socket.flush()){
		return false;
	}
	
	return m_socket.waitForBytesWritten();
}


// reimplemented (iser::IArchive)

bool CNetworkWriteArchive::ProcessData(void* dataPtr, int size)
{
	if (m_socket.state() != QAbstractSocket::ConnectedState){
		return false;
	}

	return BaseClass::ProcessData(dataPtr, size);
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


