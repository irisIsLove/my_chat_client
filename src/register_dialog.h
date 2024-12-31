#ifndef REGISTERDIALOG_H
#define REGISTERDIALOG_H

#include "global.h"

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
  void onRegisterFinished(RequestID redId, const QString& res, ErrorCode err);

private:
  void showTip(const QString& tip, bool isOk);
  void initHttpHandler();

private:
  Ui::RegisterDialog* ui;
  FuncMap m_handlers;
};

#endif // REGISTERDIALOG_H
