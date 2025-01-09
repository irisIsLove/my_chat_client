#include "global.h"

#include <QStyle>
#include <QWidget>

QString gateUrlPrefix = "";

std::function<void(QWidget*)> repolish = [](QWidget* w) {
  w->style()->unpolish(w);
  w->style()->polish(w);
};

std::function<QString(const QString&)> xorString = [](const QString& input) {
  QString result = input;

  int xorCode = input.length() % 255;
  for (int i = 0; i < input.length(); ++i) {
    result[i] = QChar(
      static_cast<ushort>(input[i].unicode() ^ static_cast<ushort>(xorCode)));
  }

  return result;
};