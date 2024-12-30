#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class LoginDialog;
class RegisterDialog;

class MainWindow : public QMainWindow
{
  Q_OBJECT
public:
  explicit MainWindow(QWidget* parent = nullptr);
  ~MainWindow();

private slots:
  void onSwitchRegister();

private:
  Ui::MainWindow* ui;
  LoginDialog* m_loginDlg = nullptr;
  RegisterDialog* m_registerDlg = nullptr;
};

#endif // MAINWINDOW_H
