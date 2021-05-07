#include "account_manager.h"

namespace { AccountManager* g_instance = nullptr; }

void AccountManager::initialize(AccountManager* manager)
{
    delete g_instance;
    g_instance = manager;
}
AccountManager* AccountManager::instance()
{
    return g_instance;
}