#ifndef CHATHISTORYWIDGET_H
#define CHATHISTORYWIDGET_H

#include "list_item_base.h"

namespace Ui {
class ChatHistoryWidget;
}

class ChatHistoryWidget : public ListItemBase
{
public:
  explicit ChatHistoryWidget(QWidget* parent = nullptr);
  ~ChatHistoryWidget();
  QSize sizeHint() const override;
  void setInfo(const QString& name,
               const QString& head,
               const QString& message);

private:
  Ui::ChatHistoryWidget* ui;
  QString m_name, m_head, m_message;
};

#endif // CHATHISTORYWIDGET_H
