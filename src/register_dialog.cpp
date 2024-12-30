#include "register_dialog.h"
#include "ui_register_dialog.h"

RegisterDialog::RegisterDialog(QWidget* parent)
  : QDialog(parent)
  , ui(new Ui::RegisterDialog)
{
  ui->setupUi(this);
}

RegisterDialog::~RegisterDialog()
{
  delete ui;
}
