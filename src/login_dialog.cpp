#include "login_dialog.h"
#include "http_manager.h"
#include "ui_login_dialog.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QPainter>
#include <QPainterPath>
#include <QRegularExpression>
#include <QtDebug>

LoginDialog::LoginDialog(QWidget* parent)
  : QDialog(parent)
  , ui(new Ui::LoginDialog)
{
  ui->setupUi(this);
  ui->editPass->setEchoMode(QLineEdit::Password);
  ui->lbForget->setState(
    "normal", "hover", "", "selected", "selected_hover", "");
  ui->lbForget->setCursor(Qt::PointingHandCursor);
  ui->lbErrTip->setProperty("state", "normal");
  ui->lbErrTip->clear();
  initAvatar();

  connect(
    ui->btnLogin, &QPushButton::clicked, this, &LoginDialog::onLoginClicked);
  connect(ui->btnRegist,
          &QPushButton::clicked,
          this,
          &LoginDialog::sigSwitchRegister);
  connect(ui->lbForget,
          &ClickedLabel::sigClicked,
          this,
          &LoginDialog::sigSwitchReset);
  connect(HttpManager::getInstance().get(),
          &HttpManager::sigLoginFinished,
          this,
          &LoginDialog::onLoginFinished);
}

LoginDialog::~LoginDialog()
{
  delete ui;
}

void
LoginDialog::onLoginClicked()
{
  qDebug() << "[LoginDialog::onLoginClicked]";
  if (!checkEmailValid() || !checkPassValid()) {
    return;
  }

  enableBtn(false);
  auto email = ui->editEmail->text();
  auto pass = ui->editPass->text();

  QJsonObject json;
  json["email"] = email;
  json["pass"] = xorString(pass);
  HttpManager::getInstance()->postHttpRequest(
    QUrl(gateUrlPrefix + "/user_login"),
    json,
    RequestID::ID_USER_LOGIN,
    Modules::MOD_LOGIN);
}

void
LoginDialog::onLoginFinished(RequestID redId, const QString& res, ErrorCode err)
{
  if (err != ErrorCode::SUCCESS) {
    showTip("网络请求错误", false);
    return;
  }

  QJsonDocument jsonDoc = QJsonDocument::fromJson(res.toUtf8());
  if (jsonDoc.isNull()) {
    showTip("json解析错误", false);
    return;
  }
  onLogin(jsonDoc.object());
}

void
LoginDialog::initAvatar()
{
  QPixmap originalPixmap(":/res/head_1.jpg");
  qDebug() << "[LoginDialog::initAvatar] " << originalPixmap.size()
           << ui->lbAvatar->size();
  originalPixmap = originalPixmap.scaled(
    ui->lbAvatar->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);

  QPixmap roundedPixmap(originalPixmap.size());
  roundedPixmap.fill((Qt::transparent));

  QPainter painter(&roundedPixmap);
  painter.setRenderHints(QPainter::Antialiasing |
                         QPainter::SmoothPixmapTransform);

  QPainterPath path;
  path.addRoundedRect(
    0, 0, originalPixmap.width(), originalPixmap.height(), 10, 10);
  painter.setClipPath(path);

  painter.drawPixmap(0, 0, originalPixmap);
  ui->lbAvatar->setPixmap(roundedPixmap);
}

bool
LoginDialog::checkEmailValid()
{
  auto email = ui->editEmail->text();
  if (email.isEmpty()) {
    qDebug() << "[LoginDialog::checkEmailValid] email is empty";
    addTipErr(TipError::TIP_EMAIL_ERR, "邮箱不能为空");
    return false;
  }
  deleteTipErr(TipError::TIP_EMAIL_ERR);
  return true;
}

bool
LoginDialog::checkPassValid()
{
  auto pass = ui->editPass->text();

  if (pass.length() < 6 || pass.length() > 16) {
    qDebug() << "[LoginDialog::checkPassValid] pass length error";
    addTipErr(TipError::TIP_PASS_ERR, "密码长度为6-16位");
    return false;
  }

  QRegularExpression regExpr("^[a-zA-Z0-9!@#$%^&*]{6,16}$");
  bool match = regExpr.match(pass).hasMatch();
  if (!match) {
    qDebug() << "[LoginDialog::checkPassValid] pass contains illegal chars";
    addTipErr(TipError::TIP_PASS_ERR, "不能包含非法字符");
    return false;
  }
  deleteTipErr(TipError::TIP_PASS_ERR);

  return true;
}

void
LoginDialog::addTipErr(TipError err, const QString& tip)
{
  m_tips[err] = tip;
  showTip(tip, false);
}

void
LoginDialog::deleteTipErr(TipError err)
{
  m_tips.remove(err);
  if (m_tips.isEmpty()) {
    ui->lbErrTip->clear();
    return;
  }
  showTip(m_tips.first(), false);
}

void
LoginDialog::showTip(const QString& tip, bool isOk)
{
  isOk ? ui->lbErrTip->setProperty("state", "normal")
       : ui->lbErrTip->setProperty("state", "error");
  ui->lbErrTip->setText(tip);
  repolish(ui->lbErrTip);
}

void
LoginDialog::enableBtn(bool enable)
{
  ui->btnLogin->setEnabled(enable);
  ui->btnRegist->setEnabled(enable);
}

void
LoginDialog::onLogin(const QJsonObject& json)
{
  ErrorCode error = static_cast<ErrorCode>(json["error"].toInt());
  if (error != ErrorCode::SUCCESS) {
    showTip("参数错误", false);
    enableBtn(true);
    return;
  }
  auto email = json["email"].toString();

  ServerInfo si;
  si.uid = json["uid"].toInt();
  si.host = json["host"].toString();
  si.port = json["port"].toString();
  si.token = json["token"].toString();

  m_uid = si.uid;
  m_token = si.token;
  qDebug() << "[LoginDialog::onLogin] email: " << email << " uid: " << m_uid
           << " host: " << si.host << " port: " << si.port
           << "token: " << m_token;
  emit sigConnectTcp(si);
}
