#include "main_window.h"
#include "login_dialog.h"
#include "register_dialog.h"
#include "reset_dialog.h"
#include "ui_main_window.h"

MainWindow::MainWindow(QWidget* parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  onSwitchLogin();
}

MainWindow::~MainWindow()
{
  delete ui;
}

void
MainWindow::onSwitchRegister()
{
  RegisterDialog* registerDlg = new RegisterDialog(this);
  registerDlg->setWindowFlags(Qt::CustomizeWindowHint |
                              Qt::FramelessWindowHint);
  connect(registerDlg,
          &RegisterDialog::sigSwitchLogin,
          this,
          &MainWindow::onSwitchLogin);
  setCentralWidget(registerDlg);
}

void
MainWindow::onSwitchLogin()
{
  LoginDialog* loginDlg = new LoginDialog(this);
  loginDlg->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
  connect(loginDlg,
          &LoginDialog::sigSwitchRegister,
          this,
          &MainWindow::onSwitchRegister);
  connect(
    loginDlg, &LoginDialog::sigSwitchReset, this, &MainWindow::onSwitchReset);
  setCentralWidget(loginDlg);
}

void
MainWindow::onSwitchReset()
{
  ResetDialog* resetDlg = new ResetDialog(this);
  resetDlg->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
  connect(
    resetDlg, &ResetDialog::sigSwitchLogin, this, &MainWindow::onSwitchLogin);
  setCentralWidget(resetDlg);
}
