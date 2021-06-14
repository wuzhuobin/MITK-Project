#include "session.h"
#include <QGlobalStatic>

namespace
{
    class SessionImpl final : public Session
    {
    public:
        // Make constructor public
        SessionImpl() = default;
    };
    Q_GLOBAL_STATIC(SessionImpl, g_instance);
};  // namespace

void Session::setAccount(const Account& account)
{
    m_account = account;
    m_isLogin = true;
    Q_EMIT accountChanged();
}

void Session::logout()
{
    m_account = {};
    m_isLogin = false;
    Q_EMIT accountChanged();
}

bool Session::isLogin() const
{
    return m_isLogin;
}

const Account& Session::account() const
{
    return m_account;
}

Session* Session::instance()
{
    return g_instance;
}
