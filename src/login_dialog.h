#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include "global.h"

#include <QDialog>
#include <QMap>

namespace Ui {
class LoginDialog;
}
class QJsonObject;

class LoginDialog : public QDialog
{
  Q_OBJECT
public:
  explicit LoginDialog(QWidget* parent = nullptr);
  ~LoginDialog();

private slots:
  void onLoginClicked();
  void onLoginFinished(RequestID redId, const QString& res, ErrorCode err);

signals:
  void sigSwitchRegister();
  void sigSwitchReset();
  void sigConnectTcp(const ServerInfo&);

private:
  void initAvatar();
  bool checkEmailValid();
  bool checkPassValid();
  void addTipErr(TipError err, const QString& tip);
  void deleteTipErr(TipError err);
  void showTip(const QString& tip, bool isOk);
  void enableBtn(bool enable);
  void onLogin(const QJsonObject& json);

private:
  Ui::LoginDialog* ui;
  int m_uid;
  QString m_token;
  QMap<TipError, QString> m_tips;
};

#endif // LOGINDIALOG_H
