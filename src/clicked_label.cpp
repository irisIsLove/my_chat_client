#include "clicked_label.h"
#include "global.h"

#include <QMouseEvent>
#include <QtDebug>

ClickedLabel::ClickedLabel(QWidget* parent)
  : QLabel(parent)
{
}

void
ClickedLabel::setState(const QString& normal,
                       const QString& hover,
                       const QString& clicked,
                       const QString& selected,
                       const QString& selectedHover,
                       const QString& selectedClicked)
{
  m_qsNorml = normal;
  m_qsHover = hover;
  m_qsClicked = clicked;

  m_qsSelected = selected;
  m_qsSelectedHover = selectedHover;
  m_qsSelectedClicked = selectedClicked;

  setProperty("state", normal);
  repolish(this);
}

ClickedLbState
ClickedLabel::getState() const
{
  return m_curState;
}

void
ClickedLabel::enterEvent(QEvent* event)
{
  if (m_curState == ClickedLbState::Normal) {
    qDebug() << objectName() << "::enterEvent: Normal -> Hover";
    setProperty("state", m_qsHover);
    repolish(this);
  } else {
    qDebug() << objectName() << "::enterEvent: Selected -> SelectedHover";
    setProperty("state", m_qsSelectedHover);
    repolish(this);
  }
  update();
  QLabel::enterEvent(event);
}

void
ClickedLabel::leaveEvent(QEvent* event)
{
  if (m_curState == ClickedLbState::Normal) {
    qDebug() << objectName() << "::leaveEvent: Hover -> Normal";
    setProperty("state", m_qsNorml);
    repolish(this);
  } else {
    qDebug() << objectName() << "::leaveEvent: SelectedHover -> Selected";
    setProperty("state", m_qsSelected);
    repolish(this);
  }
  update();
  QLabel::leaveEvent(event);
}

void
ClickedLabel::mousePressEvent(QMouseEvent* ev)
{
  if (ev->button() == Qt::LeftButton) {
    if (m_curState == ClickedLbState::Normal) {
      qDebug() << objectName() << "::mousePressEvent: Normal -> Selected";
      m_curState = ClickedLbState::Selected;
      setProperty("state", m_qsSelectedHover);
      repolish(this);
    } else {
      qDebug() << objectName() << "::mousePressEvent: Selected -> Normal";
      m_curState = ClickedLbState::Normal;
      setProperty("state", m_qsHover);
      repolish(this);
    }
    update();
    emit sigClicked();
  }
  QLabel::mousePressEvent(ev);
}
