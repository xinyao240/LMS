#include "accounttablemodel.h"

AccountTableModel::AccountTableModel(const QString &type, QObject *parent)
    :QSqlTableModel(parent)
{
    setTable(type);
    setHeaderData(0, Qt::Horizontal, tr("ID"));
    setHeaderData(1, Qt::Horizontal, tr("密码"));
    setHeaderData(2, Qt::Horizontal, tr("姓名"));
    select();
}

void AccountTableModel::changeName(int id, const QString &name)
{
    setFilter(QString("id = %1").arg(id));
    select();

    if (rowCount() == 0) {
        qCritical().noquote() << QString("Fail to change Name: Id %1 not found in %2.").arg(id).arg(tableName());
        return;
    } else if (rowCount() != 1) {
        qCritical().noquote() << QString("Fail to change Name: Id %1 is duplicate in %2.").arg(id).arg(tableName());
        return;
    }

    if (!setData(index(0, 1), name)) {
        qWarning() << "Fail to change name in " << tableName() + ":" << lastError();
    } else {
        qInfo() << "Account id " << id << "in " << tableName() + ": name changed to " << name;
    }

    setFilter("");
    select(); //还原
}

void AccountTableModel::changePassword(int id, const QString &password)
{
    setFilter(QString("id = %1").arg(id));
    select();

    if (rowCount() == 0) {
        qCritical().noquote() << QString("Fail to change Password: Id %1 not found in %2.").arg(id).arg(tableName());
        return;
    } else if (rowCount() != 1) {
        qCritical().noquote() << QString("Fail to change Password: Id %1 is duplicate in %2.").arg(id).arg(tableName());
        return;
    }

    if (!setData(index(0, 2), password)) {
        qWarning() << "Fail to change password " << tableName() + ":" << lastError();
    } else {
        qInfo() << "Account id " << id << "in " << tableName() + ": password changed";
    }

    setFilter("");
    select();
}

int AccountTableModel::addEntry(const QString &name, const QString &password)
{
    int newId = rowCount();
    QSqlRecord record = this->record();
    record.setValue("id", newId);
    record.setValue("name", name);
    record.setValue("password", password);
    if (!insertRecord(newId, record)) {
        qCritical()  << "Fail to insert record to AccountTableModel:" << lastError();
        return -1; // represent failing to create account
    } else  {
        qInfo().noquote() <<  "Add account to" << tableName() + ":"
                 << QString("(%1, %2)").arg(newId).arg(name);
        // password is hidden for security
    }

    //submitAll();
    return newId;
}
