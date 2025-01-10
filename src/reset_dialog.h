#ifndef RESETDIALOG_H
#define RESETDIALOG_H

#include "global.h"

#include <QDialog>
#include <QMap>

namespace Ui {
class ResetDialog;
}

class ResetDialog : public QDialog
{
  Q_OBJECT
public:
  explicit ResetDialog(QWidget* parent = nullptr);
  ~ResetDialog();

private slots:
  void onGetCodeClicked();
  void onResetFinished(RequestID redId, const QString& res, ErrorCode err);
  void onConfirmClicked();
  bool onUserFinished();
  bool onEmailFinished();
  bool onPassFinished();
  bool onCodeFinished();

signals:
  void sigSwitchLogin();

private:
  void showTip(const QString& tip, bool isOk);
  void initHttpHandler();
  void addTipErr(TipError err, const QString& tip);
  void deleteTipErr(TipError err);

private:
  Ui::ResetDialog* ui;
  FuncMap m_handlers;
  QMap<TipError, QString> m_tips;
};

#endif // RESETDIALOG_H
