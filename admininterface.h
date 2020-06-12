#ifndef ADMININTERFACE_H
#define ADMININTERFACE_H

#include <QListWidget>
#include <QStackedWidget>
#include <QCloseEvent>

#include "userinterface.h"
#include "bookadminwidget.h"
#include "passworddialog.h"
#include "accounttablemodel.h"
#include "readermanagementwidget.h"

class AdminInformation;
class AdminPasswordDialog;

class AdminInterface : public UserInterface
{
    Q_OBJECT
public:
    enum headerData{
        id, password, adminName
    };
    explicit AdminInterface(int id, const QString& name, QSharedPointer<AccountTableModel> adminTable,
                            QSharedPointer<BookTableModel> bookTableModel, QWidget *parent = nullptr);
    void changePage(QListWidgetItem *current, QListWidgetItem *previous);
private:
    int Id;
    QString name;
    AdminInformation *info;
    QListWidget *tabIcon;
    QStackedWidget *main;
    BookAdminWidget *bookAdmin;
    readermanagementwidget *readerAdmin;
    AdminPasswordDialog *passwordDialog;
    UserInfoDialog *userInfoDlg;

signals:

};
//用于展示用户基本信息
class AdminInformation: public UserInformation
{
    Q_OBJECT
public:
    AdminInformation(int id, const QString& name, QWidget *parent = 0);
    void update(int id, const QString& name);
};
//密码修改对话框
class AdminPasswordDialog : public PasswordDialog
{
    Q_OBJECT
public:
    AdminPasswordDialog(int id, QSharedPointer<AccountTableModel> admin, QWidget *parent = 0);
    void judge();
    void closeEvent(QCloseEvent *event);
private:
    int id;
    QSharedPointer<AccountTableModel> adminTable;
signals:
    void passwordRtned(const QString& newPassword);
};

#endif // ADMININTERFACE_H
