#include "chat_dialog.h"
#include "ui_chat_dialog.h"

ChatDialog::ChatDialog(QWidget* parent)
  : QDialog(parent)
  , ui(new Ui::ChatDialog)
{
  ui->setupUi(this);
  ui->btnSearch->setState("normal", "hover", "press");
}

ChatDialog::~ChatDialog()
{
  delete ui;
}
