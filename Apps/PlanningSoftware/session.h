#ifndef apps_svpk_lib_session_h_
#define apps_svpk_lib_session_h_
#include <QObject>
#include "account_manager/account.h"

class Session : public QObject{
    Q_OBJECT
public:
    void setAccount(const Account& account);
    void logout();
    bool isLogin() const;
    const Account& account() const;

    static Session* instance();
    

signals:
    void accountChanged();
    void caseChanged();
    void segmentChanged();
    void planChanged();
    void boneAnnotationChanged();

protected:
    Session() = default;
    
private:    

    Account m_account;
    bool m_isLogin = false;
};


#endif // apps_svpk_lib_session_h_
 