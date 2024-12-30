#include "register_dialog.h"
#include "global.h"
#include "ui_register_dialog.h"

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

  connect(ui->btnGetCode,
          &QPushButton::clicked,
          this,
          &RegisterDialog::onGetCodeClicked);
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
RegisterDialog::showTip(const QString& tip, bool isOk)
{
  isOk ? ui->lbErrTip->setProperty("state", "normal")
       : ui->lbErrTip->setProperty("state", "error");
  ui->lbErrTip->setText(tip);
  repolish(ui->lbErrTip);
}
