#include "register_dialog.h"
#include "global.h"
#include "http_manager.h"
#include "ui_register_dialog.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QRegularExpression>

RegisterDialog::RegisterDialog(QWidget* parent)
  : QDialog(parent)
  , ui(new Ui::RegisterDialog)
{
  ui->setupUi(this);
  ui->editPass->setEchoMode(QLineEdit::Password);
  ui->editVerify->setEchoMode(QLineEdit::Password);
  ui->lbErrTip->setProperty("state", "normal");
  repolish(ui->lbErrTip);

  initHttpHandler();

  connect(ui->btnGetCode,
          &QPushButton::clicked,
          this,
          &RegisterDialog::onGetCodeClicked);
  connect(HttpManager::getInstance(),
          &HttpManager::sigRegisterFinished,
          this,
          &RegisterDialog::onRegisterFinished);
}

RegisterDialog::~RegisterDialog()
{
  delete ui;
}

void
RegisterDialog::onGetCodeClicked()
{
  QString email = ui->editEmail->text();
  QRegularExpression regex(
    R"(^[a-zA-Z0-9_.+-]+@[a-zA-Z0-9-]+\.[a-zA-Z0-9-.]+$)");

  bool match = regex.match(email).hasMatch();
  if (match) {
    // TODO: http send code
  } else {
    showTip("邮箱地址不正确", false);
  }
}

void
RegisterDialog::onRegisterFinished(RequestID redId,
                                   const QString& res,
                                   ErrorCode err)
{
  if (err != ErrorCode::SUCCESS) {
    showTip("网络请求错误", false);
    return;
  }

  QJsonDocument jsonDoc = QJsonDocument::fromJson(res.toUtf8());
  if (jsonDoc.isNull() || !jsonDoc.isObject()) {
    showTip("Json解析失败", false);
    return;
  }
  m_handlers[redId](jsonDoc.object());
}

void
RegisterDialog::showTip(const QString& tip, bool isOk)
{
  isOk ? ui->lbErrTip->setProperty("state", "normal")
       : ui->lbErrTip->setProperty("state", "error");
  ui->lbErrTip->setText(tip);
  repolish(ui->lbErrTip);
}

void
RegisterDialog::initHttpHandler()
{
  m_handlers.emplace(
    RequestID::ID_GET_VERIFY_CODE, [this](const QJsonObject& obj) {
      ErrorCode err = static_cast<ErrorCode>(obj["error"].toInt());
      if (err == ErrorCode::SUCCESS) {
        showTip("参数错误", false);
        return;
      }

      QString email = obj["email"].toString();
      showTip("验证码已发送到邮箱，注意查收", true);
      qDebug() << "email is " << email;
    });
}
