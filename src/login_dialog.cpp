#include "login_dialog.h"
#include "ui_login_dialog.h"

LoginDialog::LoginDialog(QWidget* parent)
  : QDialog(parent)
  , ui(new Ui::LoginDialog)
{
  ui->setupUi(this);
  ui->editPass->setEchoMode(QLineEdit::Password);

  connect(ui->btnRegist,
          &QPushButton::clicked,
          this,
          &LoginDialog::sigSwitchRegister);
}

LoginDialog::~LoginDialog()
{
  delete ui;
}
