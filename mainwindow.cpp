#include "mainwindow.h"
#include "initdb.h"

#include <QTableView>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    initDb ini;
    // ini can also be put into the block to be assured deleted before other initializations
    // ini也可以放在下面的代码块里来保证在其它初始化运行前被销毁
    {
        QSqlError error = ini();
        if (error.type() != QSqlError::NoError) {
            qWarning() <<  "Error in SQL initialization: " << error;
            // 建议在需要输出调试/错误消息时这样写，方便后面进行日志的格式化与管理
        }
    }

    readerTable = QSharedPointer<AccountTableModel>(new AccountTableModel("reader"));
    adminTable = QSharedPointer<AccountTableModel>(new AccountTableModel("admin"));

    loginWidget = new LoginWidget(readerTable, adminTable);//pop up the login widget as an independent window

    connect(loginWidget, &LoginWidget::loginSuccessfully, this, &MainWindow::OpenInterface);

    loginWidget->show();
}

void MainWindow::OpenInterface(int id, int type)
{
    if (type == AccountTableModel::reader) {

    } else {

    }
}

MainWindow::~MainWindow()
{
}

