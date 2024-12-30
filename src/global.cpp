#include "global.h"

#include <QStyle>
#include <QWidget>


std::function<void(QWidget*)> repolish = [](QWidget* w) {
  w->style()->unpolish(w);
  w->style()->polish(w);
};