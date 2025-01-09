#ifndef TIMERBTN_H
#define TIMERBTN_H

#include <QPushButton>

class QTimer;

class TimerBtn : public QPushButton
{
public:
  explicit TimerBtn(QWidget* parent = nullptr);
  ~TimerBtn();

protected:
  void mouseReleaseEvent(QMouseEvent* e) override;

private:
  QTimer* m_timer = nullptr;
  int m_count = 10;
};

#endif // TIMERBTN_H
