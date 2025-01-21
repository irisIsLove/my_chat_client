#include "chat_history_widget.h"
#include "list_item_base.h"
#include "ui_chat_history_widget.h"

ChatHistoryWidget::ChatHistoryWidget(QWidget* parent)
  : ListItemBase(parent)
  , ui(new Ui::ChatHistoryWidget)
{
  ui->setupUi(this);
  setItemType(ItemType::ITEM_CHAT_USER);
}

ChatHistoryWidget::~ChatHistoryWidget()
{
  delete ui;
}

QSize
ChatHistoryWidget::sizeHint() const
{
  return QSize(250, 70);
}

void
ChatHistoryWidget::setInfo(const QString& name,
                           const QString& head,
                           const QString& message)
{
  m_name = name;
  m_head = head;
  m_message = message;

  QPixmap pixmap(m_head);
  ui->lbIcon->setPixmap(pixmap.scaled(
    ui->lbIcon->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
  ui->lbIcon->setScaledContents(true);
  ui->lbName->setText(m_name);
  ui->lbInfo->setText(m_message);
}
