#include "main_window.h"
#include "login_dialog.h"
#include "register_dialog.h"
#include "ui_main_window.h"

MainWindow::MainWindow(QWidget* parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  m_loginDlg = new LoginDialog(this);
  m_loginDlg->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
  setCentralWidget(m_loginDlg);

  connect(m_loginDlg,
          &LoginDialog::sigSwitchRegister,
          this,
          &MainWindow::onSwitchRegister);
}

MainWindow::~MainWindow()
{
  delete ui;
}

void
MainWindow::onSwitchRegister()
{
  m_registerDlg = new RegisterDialog(this);
  m_registerDlg->setWindowFlags(Qt::CustomizeWindowHint |
                                Qt::FramelessWindowHint);
  connect(m_registerDlg,
          &RegisterDialog::sigSwitchLogin,
          this,
          &MainWindow::onSwitchLogin);
  setCentralWidget(m_registerDlg);
}

void
MainWindow::onSwitchLogin()
{
  m_loginDlg = new LoginDialog(this);
  m_loginDlg->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
  connect(m_loginDlg,
          &LoginDialog::sigSwitchRegister,
          this,
          &MainWindow::onSwitchRegister);
  setCentralWidget(m_loginDlg);
}
