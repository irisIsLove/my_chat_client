#ifndef HTTPMANAGER_H
#define HTTPMANAGER_H

#include "global.h"
#include "singleton.h"

#include <QNetworkAccessManager>

class HttpManager
  : public QObject
  , public Singleton<HttpManager>
  , public std::enable_shared_from_this<HttpManager>
{
  Q_OBJECT
  friend class Singleton<HttpManager>;

public:
  ~HttpManager();
  void postHttpRequest(const QUrl& url,
                       const QJsonObject& json,
                       RequestID reqId,
                       Modules mod);

private slots:
  void onHttpFinished(RequestID reqId,
                      const QString& res,
                      ErrorCode err,
                      Modules mod);

signals:
  void sigHttpFinished(RequestID, const QString&, ErrorCode, Modules);
  void sigRegisterFinished(RequestID, const QString&, ErrorCode);

private:
  HttpManager();

private:
  QNetworkAccessManager m_manager;
};

#endif // HTTPMANAGER_H
