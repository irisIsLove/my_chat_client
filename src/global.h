#ifndef GLOBAL_H
#define GLOBAL_H

#include <QString>

#include <functional>

class QWidget;

// qss 刷新
extern std::function<void(QWidget*)> repolish;
extern std::function<QString(const QString&)> xorString;

enum class RequestID
{
  ID_GET_VARIFY_CODE,
  ID_REGISTER_USER,
  ID_RESET_PASS,
  ID_USER_LOGIN,
  ID_CHAT_LOGIN,
  ID_CHAT_LOGIN_RSP,
};

enum class Modules
{
  MOD_REGISTER,
  MOD_RESET,
  MOD_LOGIN,
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

struct ServerInfo
{
  QString host;
  QString port;
  QString token;
  int uid;
};

#endif // GLOBAL_H