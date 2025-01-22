#include "list_item_base.h"

#include <QPainter>
#include <QStyleOption>

ListItemBase::ListItemBase(QWidget* parent)
  : QWidget(parent)
{
}

ItemType
ListItemBase::getItemType() const
{
  return m_type;
}

void
ListItemBase::setItemType(ItemType type)
{
  m_type = type;
}

void ListItemBase::paintEvent(QPaintEvent* event) {
    QStyleOption option;
  option.init(this);

  QPainter painter(this);
  style()->drawPrimitive(QStyle::PE_Widget, &option, &painter, this);
}
