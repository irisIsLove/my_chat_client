#include "http_manager.h"

#include <QJsonDocument>
#include <QNetworkReply>

HttpManager::~HttpManager() {}

void
HttpManager::onHttpFinished(RequestID reqId,
                            const QString& res,
                            ErrorCode err,
                            Modules mod)
{
  if (mod == Modules::MOD_REGISTER) {
    emit sigRegisterFinished(reqId, res, err);
  }
}

HttpManager::HttpManager()
{
  connect(
    this, &HttpManager::sigHttpFinished, this, &HttpManager::onHttpFinished);
}

void
HttpManager::PostHttpRequest(const QUrl& url,
                             const QJsonObject& json,
                             RequestID reqId,
                             Modules mod)
{
  QByteArray data = QJsonDocument(json).toJson();

  QNetworkRequest request(url);
  request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
  request.setHeader(QNetworkRequest::ContentLengthHeader,
                    QByteArray::number(data.size()));

  auto self = shared_from_this();
  QNetworkReply* reply = m_manager.post(request, data);
  connect(reply, &QNetworkReply::finished, [self, reply, reqId, mod]() {
    if (reply->error() != QNetworkReply::NoError) {
      qDebug() << reply->errorString();
      emit self->sigHttpFinished(reqId, "", ErrorCode::ERR_NETWORK, mod);
      return;
    }

    QString res = reply->readAll();
    emit self->sigHttpFinished(reqId, res, ErrorCode::SUCCESS, mod);
    reply->deleteLater();
  });
}
