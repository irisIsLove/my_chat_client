#ifndef REGISTERDIALOG_H
#define REGISTERDIALOG_H

#include <QDialog>

namespace Ui {
class RegisterDialog;
}

class RegisterDialog : public QDialog
{
  Q_OBJECT
public:
  explicit RegisterDialog(QWidget* parent = nullptr);
  ~RegisterDialog();

private slots:
  void onGetCodeClicked();

private:
  void showTip(const QString& tip, bool isOk);

private:
  Ui::RegisterDialog* ui;
};

#endif // REGISTERDIALOG_H
