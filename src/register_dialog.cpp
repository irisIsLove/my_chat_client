#include "register_dialog.h"
#include "global.h"
#include "http_manager.h"
#include "ui_register_dialog.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QRegularExpression>
#include <QTimer>

RegisterDialog::RegisterDialog(QWidget* parent)
  : QDialog(parent)
  , ui(new Ui::RegisterDialog)
{
  ui->setupUi(this);
  ui->editPass->setEchoMode(QLineEdit::Password);
  ui->editVerify->setEchoMode(QLineEdit::Password);
  ui->lbErrTip->setProperty("state", "normal");
  ui->lbErrTip->clear();
  ui->lbPassVisible->setCursor(Qt::PointingHandCursor);
  ui->lbVerifyVisible->setCursor(Qt::PointingHandCursor);
  ui->lbPassVisible->setState(
    "unvisible", "unvisible_hover", "", "visible", "visible_hover", "");
  ui->lbVerifyVisible->setState(
    "unvisible", "unvisible_hover", "", "visible", "visible_hover", "");
  repolish(ui->lbErrTip);
  initHttpHandler();

  m_timerCountdown = new QTimer(this);

  connect(ui->btnGetCode,
          &QPushButton::clicked,
          this,
          &RegisterDialog::onGetCodeClicked);
  connect(HttpManager::getInstance().get(),
          &HttpManager::sigRegisterFinished,
          this,
          &RegisterDialog::onRegisterFinished);
  connect(ui->btnConfirm,
          &QPushButton::clicked,
          this,
          &RegisterDialog::onConfirmClicked);
  connect(ui->editUser,
          &QLineEdit::editingFinished,
          this,
          &RegisterDialog::onUserFinished);
  connect(ui->editPass,
          &QLineEdit::editingFinished,
          this,
          &RegisterDialog::onPassFinished);
  connect(ui->editEmail,
          &QLineEdit::editingFinished,
          this,
          &RegisterDialog::onEmailFinished);
  connect(ui->editVerify,
          &QLineEdit::editingFinished,
          this,
          &RegisterDialog::onVerifyFinished);
  connect(ui->editCode,
          &QLineEdit::editingFinished,
          this,
          &RegisterDialog::onCodeFinished);
  connect(ui->btnReturn,
          &QPushButton::clicked,
          this,
          &RegisterDialog::onReturnClicked);
  connect(ui->btnCancel,
          &QPushButton::clicked,
          this,
          &RegisterDialog::sigSwitchLogin);
  connect(ui->lbPassVisible, &ClickedLabel::sigClicked, [this]() {
    auto state = ui->lbPassVisible->getState();
    state == ClickedLbState::Normal
      ? ui->editPass->setEchoMode(QLineEdit::Password)
      : ui->editPass->setEchoMode(QLineEdit::Normal);
    qDebug() << "lbPassVisible clicked";
  });
  connect(ui->lbVerifyVisible, &ClickedLabel::sigClicked, [this]() {
    auto state = ui->lbVerifyVisible->getState();
    state == ClickedLbState::Normal
      ? ui->editVerify->setEchoMode(QLineEdit::Password)
      : ui->editVerify->setEchoMode(QLineEdit::Normal);
    qDebug() << "lbVerifyVisible clicked";
  });
  connect(m_timerCountdown, &QTimer::timeout, [this]() {
    if (m_countdown == 0) {
      m_timerCountdown->stop();
      emit sigSwitchLogin();
      return;
    }
    m_countdown--;
    ui->lbTip->setText(QString("注册成功，%1s后返回登录界面").arg(m_countdown));
  });
}

RegisterDialog::~RegisterDialog()
{
  delete ui;
}

void
RegisterDialog::onGetCodeClicked()
{
  bool match = onEmailFinished();
  if (match) {
    QJsonObject jsonObj;
    jsonObj["email"] = ui->editEmail->text();
    HttpManager::getInstance()->postHttpRequest(
      QUrl(gateUrlPrefix + "/get_varify_code"),
      jsonObj,
      RequestID::ID_GET_VARIFY_CODE,
      Modules::MOD_REGISTER);
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
RegisterDialog::onConfirmClicked()
{

  bool match = onUserFinished() && onPassFinished() && onEmailFinished() &&
               onVerifyFinished() && onCodeFinished();
  if (!match)
    return;

  QJsonObject jsonObj;
  jsonObj["user"] = ui->editUser->text();
  jsonObj["pass"] = xorString(ui->editPass->text());
  jsonObj["email"] = ui->editEmail->text();
  jsonObj["code"] = ui->editCode->text();
  HttpManager::getInstance()->postHttpRequest(
    QUrl(gateUrlPrefix + "/user_register"),
    jsonObj,
    RequestID::ID_REGISTER_USER,
    Modules::MOD_REGISTER);
}

bool
RegisterDialog::onUserFinished()
{
  if (ui->editUser->text().isEmpty()) {
    addTipErr(TipError::TIP_USER_ERR, "用户名不能为空");
    return false;
  }
  deleteTipErr(TipError::TIP_USER_ERR);
  return true;
}

bool
RegisterDialog::onEmailFinished()
{
  auto email = ui->editEmail->text();

  QRegularExpression regex(
    R"(^[a-zA-Z0-9_.+-]+@[a-zA-Z0-9-]+\.[a-zA-Z0-9-.]+$)");

  bool match = regex.match(email).hasMatch();
  if (!match) {
    addTipErr(TipError::TIP_EMAIL_ERR, "邮箱地址不正确");
    return false;
  }
  deleteTipErr(TipError::TIP_EMAIL_ERR);
  return true;
}

bool
RegisterDialog::onPassFinished()
{
  auto pass = ui->editPass->text();
  auto verify = ui->editVerify->text();

  if (pass.length() < 6 || pass.length() > 16) {
    addTipErr(TipError::TIP_PASS_ERR, "密码长度为6-16位");
    return false;
  }

  QRegularExpression regExpr("^[a-zA-Z0-9!@#$%^&*]{6,16}$");
  bool match = regExpr.match(pass).hasMatch();
  if (!match) {
    addTipErr(TipError::TIP_PASS_ERR, "不能包含非法字符");
    return false;
  }
  deleteTipErr(TipError::TIP_PASS_ERR);

  if (pass != verify) {
    addTipErr(TipError::TIP_VERIFY_ERR, "密码不匹配");
    return false;
  }
  deleteTipErr(TipError::TIP_VERIFY_ERR);
  return true;
}

bool
RegisterDialog::onVerifyFinished()
{
  if (ui->editVerify->text() != ui->editPass->text()) {
    addTipErr(TipError::TIP_VERIFY_ERR, "密码不匹配");
    return false;
  }
  deleteTipErr(TipError::TIP_VERIFY_ERR);
  return true;
}

bool
RegisterDialog::onCodeFinished()
{
  if (ui->editCode->text().isEmpty()) {
    addTipErr(TipError::TIP_CODE_ERR, "验证码不能为空");
    return false;
  }
  deleteTipErr(TipError::TIP_CODE_ERR);
  return true;
}

void
RegisterDialog::onReturnClicked()
{
  m_timerCountdown->stop();
  emit sigSwitchLogin();
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
    RequestID::ID_GET_VARIFY_CODE, [this](const QJsonObject& obj) {
      ErrorCode err = static_cast<ErrorCode>(obj["error"].toInt());
      if (err != ErrorCode::SUCCESS) {
        showTip("参数错误", false);
        return;
      }

      QString email = obj["email"].toString();
      showTip("验证码已发送到邮箱，注意查收", true);
      qDebug() << "[ID_GET_VARIFY_CODE]: email is " << email;
    });

  m_handlers.emplace(
    RequestID::ID_REGISTER_USER, [this](const QJsonObject& obj) {
      ErrorCode err = static_cast<ErrorCode>(obj["error"].toInt());
      if (err != ErrorCode::SUCCESS) {
        showTip("参数错误", false);
        return;
      }

      auto email = obj["email"].toString();
      changeTipPage();
      qDebug() << "[ID_REGISTER_USER]: email is " << email;
      qDebug() << "[ID_REGISTER_USER]: uid is " << obj["uid"].toInt();
    });
}

void
RegisterDialog::addTipErr(TipError err, const QString& tip)
{
  m_tips[err] = tip;
  showTip(tip, false);
}

void
RegisterDialog::deleteTipErr(TipError err)
{
  m_tips.remove(err);
  if (m_tips.isEmpty()) {
    ui->lbErrTip->clear();
    return;
  }
  showTip(m_tips.first(), false);
}

void
RegisterDialog::changeTipPage()
{
  m_timerCountdown->stop();
  m_countdown = 5;
  ui->stackedWidget->setCurrentWidget(ui->page_2);

  m_timerCountdown->start(1000);
}
