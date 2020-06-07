
#include "loginwidget.h"

#include <QApplication>
#include <QSharedPointer>
#include "initdb.h"
#include "accounttablemodel.h"
#include "loginwidget.h"
#include "admininterface.h"

initDb ini;

LoginWidget *loginWidget;
UserInterface *mainInterface;

void initial();
void clear();

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("图书管理系统");
    initial();

    loginWidget->show();
    mainInterface->show();
    if(a.exec()){
        clear();
    }
    return 0;
}
void initial(){
    QSqlError error = ini();
    if (error.type() != QSqlError::NoError) {
        qWarning() <<  "Error in SQL initialization: " << error;
        // 建议在需要输出调试/错误消息时这样写，方便后面进行日志的格式化与管理
    }
    QSharedPointer<AccountTableModel> readerTable = (QSharedPointer<AccountTableModel>)new AccountTableModel("reader");
    QSharedPointer<AccountTableModel> adminTable = (QSharedPointer<AccountTableModel>)new AccountTableModel("admin");

    loginWidget = new LoginWidget(readerTable, adminTable);
    mainInterface = new AdminInterface(0,"dd");
}
void clear(){
    delete loginWidget;
}
