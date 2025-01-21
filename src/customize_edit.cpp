#include "customize_edit.h"

CustomizeEdit::CustomizeEdit(QWidget* parent)
  : QLineEdit(parent)
{
  connect(this, &QLineEdit::textChanged, this, &CustomizeEdit::limitTextLength);
}

void
CustomizeEdit::setMaxLength(int length)
{
  m_maxLength = length;
}

void
CustomizeEdit::focusOutEvent(QFocusEvent* e)
{
  QLineEdit::focusOutEvent(e);
  emit sigFocusOut();
}

void
CustomizeEdit::limitTextLength(const QString& text)
{
  if (m_maxLength <= 0) {
    return;
  }

  QByteArray byteArray = text.toUtf8();
  if (byteArray.size() > m_maxLength) {
    byteArray = byteArray.left(m_maxLength);
    setText(QString::fromUtf8(byteArray));
  }
}
