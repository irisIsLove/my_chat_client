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

  m_registerDlg = new RegisterDialog(this);
  m_registerDlg->setWindowFlags(Qt::CustomizeWindowHint |
                                Qt::FramelessWindowHint);

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
  setCentralWidget(m_registerDlg);
}
