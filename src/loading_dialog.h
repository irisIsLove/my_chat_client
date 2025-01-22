#ifndef LOADINGDIALOG_H
#define LOADINGDIALOG_H

#include <QDialog>

namespace Ui {
class LoadingDialog;
}

class LoadingDialog : public QDialog
{
public:
  explicit LoadingDialog(QWidget* parent = nullptr);
  ~LoadingDialog();

private:
  Ui::LoadingDialog* ui;
};

#endif // LOADINGDIALOG_H
