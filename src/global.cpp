#include "global.h"

#include <QStyle>
#include <QWidget>

QString gateUrlPrefix = "";

std::function<void(QWidget*)> repolish = [](QWidget* w) {
  w->style()->unpolish(w);
  w->style()->polish(w);
};