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
  void onSwitchLogin();
  void onSwitchReset();
  void onSwitchChat();

private:
  Ui::MainWindow* ui;
};

#endif // MAINWINDOW_H
