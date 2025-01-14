#ifndef TCPMANAGER_H
#define TCPMANAGER_H

#include "global.h"
#include "singleton.h"

#include <QTcpSocket>

#include <unordered_map>

class TcpManager
  : public QObject
  , public Singleton<TcpManager>
{
  Q_OBJECT
  friend class Singleton<TcpManager>;

public slots:
  void onTcpConnect(const ServerInfo& server);

private slots:
  void onSendData(RequestID reqId, const QString& data);

signals:
  void sigConnectSuccess(bool);
  void sigSendData(RequestID, const QString&);
  void sigSwitchChat();
  void sigLoginFailed(ErrorCode);

private:
  TcpManager();
  void initHandler();
  void handleMessage(RequestID id, int length, const QByteArray& data);

private:
  bool m_isRecvPending = false;
  std::uint16_t m_messageId = 0, m_messageLength = 0;
  QTcpSocket m_socket;
  QByteArray m_buffer;
  std::unordered_map<RequestID,
                     std::function<void(RequestID, int, const QByteArray&)>>
    m_handlers;
};

#endif // TCPMANAGER_H
