#include "chat_view.h"

#include <QEvent>
#include <QHBoxLayout>
#include <QPainter>
#include <QScrollArea>
#include <QScrollBar>
#include <QStyleOption>
#include <QTimer>
#include <QVBoxLayout>

ChatView::ChatView(QWidget* parent)
  : QWidget(parent)
  , isAppended(false)
{
  QVBoxLayout* pMainLayout = new QVBoxLayout(this);
  pMainLayout->setMargin(0);

  m_pScrollArea = new QScrollArea(this);
  m_pScrollArea->setObjectName("chat_area");
  pMainLayout->addWidget(m_pScrollArea);

  QWidget* w = new QWidget(this);
  w->setObjectName("chat_back_ground");
  w->setAutoFillBackground(true);

  QVBoxLayout* pLayout = new QVBoxLayout(w);
  pLayout->addWidget(new QWidget(this), 100000);
  m_pScrollArea->setWidget(w);

  m_pScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  QScrollBar* pScrollBar = m_pScrollArea->verticalScrollBar();

  QHBoxLayout* pLayout2 = new QHBoxLayout(this);
  pLayout2->addWidget(pScrollBar, 0, Qt::AlignRight);
  pLayout2->setMargin(0);
  m_pScrollArea->setLayout(pLayout2);
  pScrollBar->setHidden(true);

  m_pScrollArea->setWidgetResizable(true);
  m_pScrollArea->installEventFilter(this);
  initStyleSheet();

  connect(
    pScrollBar, &QScrollBar::rangeChanged, this, &ChatView::onVScrollBarMoved);
}

ChatView::~ChatView() {}

void
ChatView::appendChatItem(QWidget* item)
{
  QVBoxLayout* layout =
    qobject_cast<QVBoxLayout*>(m_pScrollArea->widget()->layout());
  layout->insertWidget(layout->count() - 1, item);
  isAppended = true;
}

void
ChatView::prependChatItem(QWidget* item)
{
}

void
ChatView::insertChatItem(QWidget* before, QWidget* item)
{
}

bool
ChatView::eventFilter(QObject* watched, QEvent* event)
{
  if (event->type() == QEvent::Enter && watched == m_pScrollArea) {
    m_pScrollArea->verticalScrollBar()->setHidden(
      m_pScrollArea->verticalScrollBar()->maximum() == 0);
  } else if (event->type() == QEvent::Leave && watched == m_pScrollArea) {
    m_pScrollArea->verticalScrollBar()->setHidden(true);
  }

  return QWidget::eventFilter(watched, event);
}

void
ChatView::paintEvent(QPaintEvent* event)
{
  QStyleOption opt;
  opt.init(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void
ChatView::onVScrollBarMoved(int min, int max)
{
  if (isAppended) {
    QScrollBar* pScrollBar = m_pScrollArea->verticalScrollBar();
    pScrollBar->setSliderPosition(pScrollBar->maximum());

    QTimer::singleShot(500, [this]() { isAppended = false; });
  }
}

void
ChatView::initStyleSheet()
{
}
