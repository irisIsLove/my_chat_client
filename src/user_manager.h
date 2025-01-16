#ifndef USERMANAGER_H
#define USERMANAGER_H

#include "singleton.h"

#include <QString>

class UserManager : public Singleton<UserManager>
{
  friend class Singleton<UserManager>;

public:
  void setName(const QString& name);
  void setToken(const QString& token);
  void setUid(int uid);

private:
  UserManager() = default;

private:
  QString m_name;
  QString m_token;
  int m_uid;
};

#endif // USERMANAGER_H
