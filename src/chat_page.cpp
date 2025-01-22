#include "chat_page.h"
#include "ui_chat_page.h"

#include <QPainter>
#include <QStyleOption>

ChatPage::ChatPage(QWidget* parent)
  : QWidget(parent)
  , ui(new Ui::ChatPage)
{
  ui->setupUi(this);
  ui->btnReceive->setState("normal", "hover", "press");
  ui->btnSend->setState("normal", "hover", "press");
  ui->lbEmo->setState("normal", "hover", "press", "normal", "hover", "press");
  ui->lbFile->setState("normal", "hover", "press", "normal", "hover", "press");
}

ChatPage::~ChatPage()
{
  delete ui;
}

void
ChatPage::paintEvent(QPaintEvent* event)
{
  QStyleOption option;
  option.init(this);

  QPainter painter(this);
  style()->drawPrimitive(QStyle::PE_Widget, &option, &painter, this);
}
