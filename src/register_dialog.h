#ifndef REGISTERDIALOG_H
#define REGISTERDIALOG_H

#include "global.h"

#include <QDialog>
#include <QMap>

namespace Ui {
class RegisterDialog;
}

class QTimer;

class RegisterDialog : public QDialog
{
  Q_OBJECT
public:
  explicit RegisterDialog(QWidget* parent = nullptr);
  ~RegisterDialog();

private slots:
  void onGetCodeClicked();
  void onRegisterFinished(RequestID redId, const QString& res, ErrorCode err);
  void onConfirmClicked();
  bool onUserFinished();
  bool onEmailFinished();
  bool onPassFinished();
  bool onVerifyFinished();
  bool onCodeFinished();
  void onReturnClicked();
  void onCancelClicked();

signals:
  void sigSwitchLogin();

private:
  void showTip(const QString& tip, bool isOk);
  void initHttpHandler();
  void addTipErr(TipError err, const QString& tip);
  void deleteTipErr(TipError err);
  void changeTipPage();

private:
  Ui::RegisterDialog* ui;
  FuncMap m_handlers;
  QMap<TipError, QString> m_tips;
  QTimer* m_timerCountdown = nullptr;
  int m_countdown = 5;
};

#endif // REGISTERDIALOG_H
