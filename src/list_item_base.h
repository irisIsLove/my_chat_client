#ifndef LISTITEMBASE_H
#define LISTITEMBASE_H

#include "global.h"

#include <QWidget>

class ListItemBase : public QWidget
{
public:
  explicit ListItemBase(QWidget* parent = nullptr);
  virtual ~ListItemBase() = default;
  ItemType getItemType() const;
  void setItemType(ItemType type);

protected:
  void paintEvent(QPaintEvent* event) override;

private:
  ItemType m_type;
};

#endif // LISTITEMBASE_H
