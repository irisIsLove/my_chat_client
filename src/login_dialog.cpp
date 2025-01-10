#include "login_dialog.h"
#include "ui_login_dialog.h"

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

  connect(ui->btnRegist,
          &QPushButton::clicked,
          this,
          &LoginDialog::sigSwitchRegister);
  connect(ui->lbForget,
          &ClickedLabel::sigClicked,
          this,
          &LoginDialog::sigSwitchReset);
}

LoginDialog::~LoginDialog()
{
  delete ui;
}
