#ifndef CHATDIALOG_H
#define CHATDIALOG_H

#include "global.h"

#include <QDialog>

namespace Ui {
class ChatDialog;
}

class ChatDialog : public QDialog
{
public:
  explicit ChatDialog(QWidget* parent = nullptr);
  ~ChatDialog();
  void addTestList();

private:
  void showUserSearch(bool isSearch = false);

private:
  bool m_isLoading = false;
  ChatUiMode m_mode = ChatUiMode::MODE_CHAT;
  ChatUiMode m_state = ChatUiMode::MODE_CHAT;
  Ui::ChatDialog* ui;
};

#endif // CHATDIALOG_H
