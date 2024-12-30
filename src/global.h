#ifndef GLOBAL_H
#define GLOBAL_H

#include <functional>

class QWidget;

// qss 刷新
extern std::function<void(QWidget*)> repolish;

#endif // GLOBAL_H