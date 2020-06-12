#ifndef STUDENTINTERFACE_H
#define STUDENTINTERFACE_H

#include <QMainWindow>
#include "passworddialog.h"
#include "userinterface.h"
#include "accounttablemodel.h"
#include "booktablemodel.h"
#include "admininterface.h"
#include "loginwidget.h"

#include <QApplication>
#include <QSharedPointer>
#include "initdb.h"
#include "accounttablemodel.h"
#include "loginwidget.h"
#include "admininterface.h"
#include "recordtablemodel.h"

namespace Ui {
class studentinterface;
}

class studentinterface : public QMainWindow
{
    Q_OBJECT

public:
    explicit studentinterface(int id,QString name,QSharedPointer<AccountTableModel> adminTable,
                              QSharedPointer<BookTableModel> bookTableModel,QSharedPointer<recordtablemodel> recordTable,QWidget *parent = nullptr);
    ~studentinterface();
public slots:
    void updatename(int id, QString name);

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_tabWidget_currentChanged(int index);

    void on_pushButton_5_clicked();

private:
    QVBoxLayout *lay;
    QTableView *view;
    QVBoxLayout *lay2;
    QTableView *view2;
    QWidget *parent;
    int id;
    QString name;
    AdminPasswordDialog *pdialog;
    UserInfoDialog *infodia;
    QSharedPointer<BookTableModel> bookmodel;
    QSharedPointer<AccountTableModel> readermodel;
    QSharedPointer<recordtablemodel> recordmodel;
    Ui::studentinterface *ui;
};

#endif // STUDENTINTERFACE_H
