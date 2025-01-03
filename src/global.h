#ifndef GLOBAL_H
#define GLOBAL_H

#include <functional>
#include <unordered_map>

class QWidget;
class QString;

// qss 刷新
extern std::function<void(QWidget*)> repolish;

enum class RequestID
{
  ID_GET_VARIFY_CODE,
  ID_REGISTER_USER,
};

enum class Modules
{
  MOD_REGISTER,
};

enum class ErrorCode
{
  SUCCESS,
  ERR_JSON,
  ERR_NETWORK,
};

class QJsonObject;
using FuncMap =
  std::unordered_map<RequestID, std::function<void(const QJsonObject&)>>;

extern QString gateUrlPrefix;

#endif // GLOBAL_H