#include "tcp_manager.h"

#include <QDataStream>
#include <QJsonDocument>
#include <QJsonObject>

void
TcpManager::onTcpConnect(const ServerInfo& server)
{
  qDebug() << "[TcpManager::onTcpConnect]: Receive tcp connect signal\n"
           << "Connecting to server...";
  m_socket.connectToHost(server.host, server.port.toInt());
}

void
TcpManager::onSendData(RequestID reqId, const QString& data)
{
  std::uint16_t id = static_cast<std::uint16_t>(reqId);
  QByteArray message = data.toUtf8();
  std::uint16_t length = static_cast<std::uint16_t>(message.length());

  QByteArray packet;
  QDataStream stream(&packet, QIODevice::WriteOnly);
  stream.setByteOrder(QDataStream::BigEndian);
  stream << id << length;
  packet.append(message);

  m_socket.write(packet);
}

TcpManager::TcpManager()
{
  connect(&m_socket, &QTcpSocket::connected, [this]() {
    emit sigConnectSuccess(true);
  });
  connect(&m_socket, &QTcpSocket::readyRead, [this]() {
    m_buffer.append(m_socket.readAll());

    QDataStream stream(&m_buffer, QIODevice::ReadOnly);
    stream.setVersion(QDataStream::Qt_5_0);
    for (;;) {
      if (!m_isRecvPending) {
        if (m_buffer.size() < sizeof(std::uint16_t) * 2) {
          return;
        }

        stream >> m_messageId >> m_messageLength;
        m_buffer = m_buffer.mid(sizeof(std::uint16_t) * 2);
        qDebug() << "[TcpManager::(readyRead)]: MessageId:" << m_messageId
                 << "Length:" << m_messageLength;
      }

      if (m_buffer.size() < m_messageLength) {
        m_isRecvPending = true;
        return;
      }
      m_isRecvPending = false;
      QByteArray data = m_buffer.mid(0, m_messageLength);
      qDebug() << "[TcpManager::(readyRead)]: Recv data:" << data;
      m_buffer = m_buffer.mid(m_messageLength);
      handleMessage(static_cast<RequestID>(m_messageId), m_messageLength, data);
    }
  });
  connect(&m_socket,
          &QTcpSocket::errorOccurred,
          [this](QAbstractSocket::SocketError error) {
            qDebug() << "[TcpManager::(errorOccurred)]: "
                     << m_socket.errorString();
            emit sigConnectSuccess(false);
          });
  connect(&m_socket, &QTcpSocket::disconnected, this, [&]() {
    qDebug() << "[TcpManager::(disconnected)]";
  });
  connect(this, &TcpManager::sigSendData, this, &TcpManager::onSendData);
  initHandler();
}

void
TcpManager::initHandler()
{
  m_handlers.emplace(
    RequestID::ID_CHAT_LOGIN_RSP,
    [this](RequestID reqId, int length, const QByteArray& data) {
      qDebug() << "[TcpManager::initHandler(ID_CHAT_LOGIN_RSP)]:"
               << "Handle id is" << static_cast<int>(reqId) << "data is"
               << data;
      QJsonDocument jsonDoc = QJsonDocument::fromJson(data);

      if (jsonDoc.isNull()) {
        qDebug() << "[TcpManager::initHandler(ID_CHAT_LOGIN_RSP)]:"
                 << "Failed to create QJsonDocument";
        return;
      }

      QJsonObject json = jsonDoc.object();
      if (!json.contains("error")) {
        qDebug() << "[TcpManager::initHandler(ID_CHAT_LOGIN_RSP)]:"
                 << "Login failed, error is Json Parse Error";
        emit sigLoginFailed(ErrorCode::ERR_JSON);
        return;
      }

      ErrorCode err = static_cast<ErrorCode>(json["error"].toInt());
      if (err != ErrorCode::SUCCESS) {
        qDebug() << "[TcpManager::initHandler(ID_CHAT_LOGIN_RSP)]:"
                 << "Login failed, error is " << json["error"].toInt();
        emit sigLoginFailed(err);
        return;
      }

      // TODO: UserManager
      emit sigSwitchChat();
    });
}

void
TcpManager::handleMessage(RequestID id, int length, const QByteArray& data)
{
  auto iter = m_handlers.find(id);
  if (iter != m_handlers.end()) {
    qDebug() << "[TcpManager::handleMessage]: Not found id"
             << static_cast<int>(id);
    return;
  }
  iter->second(id, length, data);
}
