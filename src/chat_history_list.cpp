#include "chat_history_list.h"

#include <QEvent>
#include <QScrollBar>
#include <QWheelEvent>
#include <QtDebug>

ChatHistoryList::ChatHistoryList(QWidget* parent)
  : QListWidget(parent)
{
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  viewport()->installEventFilter(this);
}

bool
ChatHistoryList::eventFilter(QObject* object, QEvent* event)
{
  if (object == viewport()) {
    if (event->type() == QEvent::Enter) {
      setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    } else if (event->type() == QEvent::Leave) {
      setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    } else if (event->type() == QEvent::Wheel) {
      QWheelEvent* wheelEvent = static_cast<QWheelEvent*>(event);
      int degrees = wheelEvent->angleDelta().y() / 8;
      int steps = degrees / 15;

      QScrollBar* scrollBar = verticalScrollBar();
      scrollBar->setValue(scrollBar->value() - steps);

      int maxScrollValue = scrollBar->maximum();
      int curScrollValue = scrollBar->value();
      if (maxScrollValue - curScrollValue <= 0) {
        qDebug() << "[ChatHistoryList::eventFilter] Load more history";
        emit sigLoadingHistory();
      }
      return true;
    }
  }

  return QListWidget::eventFilter(object, event);
}
