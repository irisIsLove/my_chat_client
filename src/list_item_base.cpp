#include "list_item_base.h"

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
