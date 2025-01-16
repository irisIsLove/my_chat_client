#include "user_manager.h"

void
UserManager::setName(const QString& name)
{
  m_name = name;
}

void
UserManager::setToken(const QString& token)
{
  m_token = token;
}

void
UserManager::setUid(int uid)
{
  m_uid = uid;
}
