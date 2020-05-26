#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "accounttablemodel.h"
#include "loginwidget.h"
#include "initdb.h"

#include <QMainWindow>
#include <QSharedPointer>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void OpenInterface(int id, int type);

private:
    QSharedPointer<AccountTableModel> readerTable;
    QSharedPointer<AccountTableModel> adminTable;
    //账户模型的共享指针供多个窗口使用
    LoginWidget *loginWidget;
    initDb *initdb;
};
#endif // MAINWINDOW_H
