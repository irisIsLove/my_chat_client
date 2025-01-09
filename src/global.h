#ifndef GLOBAL_H
#define GLOBAL_H

#include <functional>
#include <unordered_map>

class QWidget;
class QString;

// qss 刷新
extern std::function<void(QWidget*)> repolish;
extern std::function<QString(const QString&)> xorString;

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
  ERR_VARIFY_EXPIRED,
  ERR_VARIFY_CODE,
  ERR_USER_EXIST,
  ERR_PASS_ERR,
  ERR_EMAIL_NOT_MATCH,
  ERR_PASS_UP_FAIL,
  ERR_PASS_INVALID,
};

enum class TipError
{
  TIP_SUCCESS,
  TIP_USER_ERR,
  TIP_EMAIL_ERR,
  TIP_PASS_ERR,
  TIP_VERIFY_ERR,
  TIP_CODE_ERR,
};

class QJsonObject;
using FuncMap =
  std::unordered_map<RequestID, std::function<void(const QJsonObject&)>>;

extern QString gateUrlPrefix;

#endif // GLOBAL_H