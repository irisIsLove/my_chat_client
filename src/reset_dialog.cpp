#include "reset_dialog.h"
#include "http_manager.h"
#include "ui_reset_dialog.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QRegularExpression>

ResetDialog::ResetDialog(QWidget* parent)
  : QDialog(parent)
  , ui(new Ui::ResetDialog)
{
  ui->setupUi(this);
  ui->editNewPass->setEchoMode(QLineEdit::Password);
  ui->lbErrTip->setProperty("state", "normal");
  ui->lbErrTip->clear();
  ui->lbNewPassVisible->setCursor(Qt::PointingHandCursor);
  ui->lbNewPassVisible->setState(
    "unvisible", "unvisible_hover", "", "visible", "visible_hover", "");
  repolish(ui->lbErrTip);

  initHttpHandler();

  connect(
    ui->btnCancel, &QPushButton::clicked, this, &ResetDialog::sigSwitchLogin);
  connect(ui->btnGetCode,
          &QPushButton::clicked,
          this,
          &ResetDialog::onGetCodeClicked);
  connect(HttpManager::getInstance().get(),
          &HttpManager::sigResetFinished,
          this,
          &ResetDialog::onResetFinished);
  connect(ui->btnConfirm,
          &QPushButton::clicked,
          this,
          &ResetDialog::onConfirmClicked);
  connect(ui->editUser,
          &QLineEdit::editingFinished,
          this,
          &ResetDialog::onUserFinished);
  connect(ui->editNewPass,
          &QLineEdit::editingFinished,
          this,
          &ResetDialog::onPassFinished);
  connect(ui->editEmail,
          &QLineEdit::editingFinished,
          this,
          &ResetDialog::onEmailFinished);
  connect(ui->editCode,
          &QLineEdit::editingFinished,
          this,
          &ResetDialog::onCodeFinished);
  connect(ui->lbNewPassVisible, &ClickedLabel::sigClicked, [this]() {
    auto state = ui->lbNewPassVisible->getState();
    state == ClickedLbState::Normal
      ? ui->editNewPass->setEchoMode(QLineEdit::Password)
      : ui->editNewPass->setEchoMode(QLineEdit::Normal);
    qDebug() << "lbNewPassVisible clicked";
  });
}

ResetDialog::~ResetDialog()
{
  delete ui;
}

void
ResetDialog::onGetCodeClicked()
{
  bool match = onEmailFinished();
  if (match) {
    QJsonObject jsonObj;
    jsonObj["email"] = ui->editEmail->text();
    HttpManager::getInstance()->postHttpRequest(
      QUrl(gateUrlPrefix + "/get_varify_code"),
      jsonObj,
      RequestID::ID_GET_VARIFY_CODE,
      Modules::MOD_RESET);
  }
}

void
ResetDialog::onResetFinished(RequestID redId, const QString& res, ErrorCode err)
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
ResetDialog::onConfirmClicked()
{

  bool match = onUserFinished() && onPassFinished() && onEmailFinished() &&
               onCodeFinished();
  if (!match)
    return;

  QJsonObject jsonObj;
  jsonObj["user"] = ui->editUser->text();
  jsonObj["pass"] = xorString(ui->editNewPass->text());
  jsonObj["email"] = ui->editEmail->text();
  jsonObj["code"] = ui->editCode->text();
  HttpManager::getInstance()->postHttpRequest(
    QUrl(gateUrlPrefix + "/reset_pass"),
    jsonObj,
    RequestID::ID_RESET_PASS,
    Modules::MOD_RESET);
}

bool
ResetDialog::onUserFinished()
{
  if (ui->editUser->text().isEmpty()) {
    addTipErr(TipError::TIP_USER_ERR, "用户名不能为空");
    return false;
  }
  deleteTipErr(TipError::TIP_USER_ERR);
  return true;
}

bool
ResetDialog::onEmailFinished()
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
ResetDialog::onPassFinished()
{
  auto pass = ui->editNewPass->text();
  auto verify = ui->editNewPass->text();

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
ResetDialog::onCodeFinished()
{
  if (ui->editCode->text().isEmpty()) {
    addTipErr(TipError::TIP_CODE_ERR, "验证码不能为空");
    return false;
  }
  deleteTipErr(TipError::TIP_CODE_ERR);
  return true;
}

void
ResetDialog::showTip(const QString& tip, bool isOk)
{
  isOk ? ui->lbErrTip->setProperty("state", "normal")
       : ui->lbErrTip->setProperty("state", "error");
  ui->lbErrTip->setText(tip);
  repolish(ui->lbErrTip);
}

void
ResetDialog::initHttpHandler()
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

  m_handlers.emplace(RequestID::ID_RESET_PASS, [this](const QJsonObject& obj) {
    ErrorCode err = static_cast<ErrorCode>(obj["error"].toInt());
    if (err != ErrorCode::SUCCESS) {
      showTip("参数错误", false);
      return;
    }

    QString email = obj["email"].toString();
    showTip("重置成功,点击“取消”返回登录", true);
    qDebug() << "[ID_RESET_PASS]: email is " << email;
  });
}

void
ResetDialog::addTipErr(TipError err, const QString& tip)
{
  m_tips[err] = tip;
  showTip(tip, false);
}

void
ResetDialog::deleteTipErr(TipError err)
{
  m_tips.remove(err);
  if (m_tips.isEmpty()) {
    ui->lbErrTip->clear();
    return;
  }
  showTip(m_tips.first(), false);
}