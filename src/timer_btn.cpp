#include "timer_btn.h"

#include <QMouseEvent>
#include <QTimer>
#include <QtDebug>

TimerBtn::TimerBtn(QWidget* parent)
  : QPushButton(parent)
{
  m_timer = new QTimer(this);

  connect(m_timer, &QTimer::timeout, [this]() {
    --m_count;
    if (m_count <= 0) {
      m_timer->stop();
      m_count = 10;
      setText("获取");
      setEnabled(true);
      return;
    }
    setText(QString::number(m_count));
  });
}

TimerBtn::~TimerBtn()
{
  m_timer->stop();
}

void
TimerBtn::mouseReleaseEvent(QMouseEvent* e)
{
  if (e->button() == Qt::LeftButton) {
    qDebug() << "TimerBtn::released!";
    setEnabled(false);
    setText(QString::number(m_count));
    m_timer->start(1000);
    emit clicked();
  }
  QPushButton::mouseReleaseEvent(e);
}
