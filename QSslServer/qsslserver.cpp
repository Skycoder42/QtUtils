#include "qsslserver.h"
#include <QFile>
#include <QCoreApplication>
#include <QSslKey>
#include <QSslCertificate>
#include <QSslCipher>

QSslServer::QSslServer(QObject *parent) :
    QTcpServer(parent),
    configuration(QSslConfiguration::defaultConfiguration()),
    lastError(QAbstractSocket::UnknownSocketError),
    lastSslErrors()
{}

QSslSocket *QSslServer::nextPendingSslConnection()
{
    return qobject_cast<QSslSocket*>(this->nextPendingConnection());
}

void QSslServer::addCaCertificate(const QSslCertificate &certificate)
{
    QList<QSslCertificate> certs = this->configuration.caCertificates();
    certs.append(certificate);
    this->configuration.setCaCertificates(certs);
}

bool QSslServer::addCaCertificate(const QString &path, QSsl::EncodingFormat format)
{
    bool ret = false;
    QFile file(path);
    if(file.open(QIODevice::ReadOnly))
    {
        QSslCertificate cert(file.readAll(), format);
        if(!cert.isNull())
        {
            this->addCaCertificate(cert);
            ret = true;
        }
        file.close();
    }

    return ret;
}

void QSslServer::addCaCertificates(const QList<QSslCertificate> &certificates)
{
    QList<QSslCertificate> certs = this->configuration.caCertificates();
    certs.append(certificates);
    this->configuration.setCaCertificates(certs);
}

QList<QSslCertificate> QSslServer::caCertificates() const
{
    return this->configuration.caCertificates();
}

void QSslServer::setCaCertificates(const QList<QSslCertificate> &certificates)
{
    this->configuration.setCaCertificates(certificates);
}

QSslCertificate QSslServer::localCertificate() const
{
    return this->configuration.localCertificate();
}

QList<QSslCertificate> QSslServer::localCertificateChain() const
{
    return this->configuration.localCertificateChain();
}

void QSslServer::setLocalCertificate(const QSslCertificate &certificate)
{
    this->configuration.setLocalCertificate(certificate);
}

bool QSslServer::setLocalCertificate(const QString &path, QSsl::EncodingFormat format)
{
    bool ret = false;
    QFile file(path);
    if(file.open(QIODevice::ReadOnly))
    {
        QSslCertificate cert(file.readAll(), format);
        if(!cert.isNull())
        {
            this->configuration.setLocalCertificate(cert);
            ret = true;
        }
        file.close();
    }

    return ret;
}

void QSslServer::setLocalCertificateChain(const QList<QSslCertificate> &localChain)
{
    this->configuration.setLocalCertificateChain(localChain);
}

QSslKey QSslServer::privateKey() const
{
    return this->configuration.privateKey();
}

void QSslServer::setPrivateKey(const QSslKey &key)
{
    this->configuration.setPrivateKey(key);
}

bool QSslServer::setPrivateKey(const QString &fileName, QSsl::KeyAlgorithm algorithm, QSsl::EncodingFormat format, const QByteArray &passPhrase)
{
    bool ret = false;
    QFile file(fileName);
    if(file.open(QIODevice::ReadOnly))
    {
        QSslKey newKey(file.readAll(), algorithm, format, QSsl::PrivateKey, passPhrase);
        if(!newKey.isNull())
        {
            this->configuration.setPrivateKey(newKey);
            ret = true;
        }
        file.close();
    }

    return ret;
}

QList<QSslCipher> QSslServer::ciphers() const
{
    return this->configuration.ciphers();
}

void QSslServer::setCiphers(const QList<QSslCipher> &ciphers)
{
    this->configuration.setCiphers(ciphers);
}

void QSslServer::setCiphers(const QString &ciphers)
{
    QStringList cphl = ciphers.split(":", QString::SkipEmptyParts);
    QList<QSslCipher> cphs;
    foreach(QString cph, cphl)
    {
        QSslCipher c(cph);
        if(!c.isNull())
            cphs.append(c);
    }
    this->configuration.setCiphers(cphs);
}

QSsl::SslProtocol QSslServer::protocol() const
{
    return this->configuration.protocol();
}

void QSslServer::setProtocol(QSsl::SslProtocol protocol)
{
    this->configuration.setProtocol(protocol);
}

void QSslServer::setSslConfiguration(const QSslConfiguration &configuration)
{
    this->configuration = configuration;
}

QSslConfiguration QSslServer::sslConfiguration() const
{
    return this->configuration;
}

QAbstractSocket::SocketError QSslServer::clientError() const
{
    return this->lastError;
}

QList<QSslError> QSslServer::clientSslErrors() const
{
    return this->lastSslErrors;
}

void QSslServer::incomingConnection(qintptr handle)
{
    //Create Socket
    QSslSocket *socket = new QSslSocket;
    if (!socket)
    {
       qCritical() << "Not enough memory to create new QSslSocket!!!";
       return;
    }
    if (!socket->setSocketDescriptor(handle))
    {
        this->lastError = socket->error();
        delete socket;
        emit clientError(this->lastError);
        return;
    }

    //Connects
    QObject::connect(socket, SIGNAL(encrypted()), this, SLOT(socketReady()));
    QObject::connect(socket, SIGNAL(sslErrors(QList<QSslError>)), this, SLOT(socketErrors(QList<QSslError>)));
    QObject::connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(socketErrors(QAbstractSocket::SocketError)));

    //set ssl data
    socket->setSslConfiguration(this->configuration);
    socket->startServerEncryption();
}

void QSslServer::socketReady()
{
    QSslSocket *socket = qobject_cast<QSslSocket*>(QObject::sender());
    if(socket != NULL)
    {
        socket->disconnect();
        this->addPendingConnection(socket);
        emit newSslConnection();
    }
}

void QSslServer::socketErrors(QList<QSslError> errors)
{
    this->lastSslErrors = errors;
    emit clientSslErrors(errors);
    QSslSocket *socket = qobject_cast<QSslSocket*>(QObject::sender());
    if(socket != NULL)
        socket->deleteLater();
}

void QSslServer::socketErrors(QAbstractSocket::SocketError error)
{
    this->lastError = error;
    emit clientError(error);
    QSslSocket *socket = qobject_cast<QSslSocket*>(QObject::sender());
    if(socket != NULL)
        socket->deleteLater();
}

