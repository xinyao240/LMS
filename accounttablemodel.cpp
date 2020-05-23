#include "accounttablemodel.h"

#include <cassert>

AccountTableModel::AccountTableModel(const QString &type, QObject *parent)
    :QSqlTableModel(parent)
{
    setTable(type);
    select();
}

void AccountTableModel::changeName(int id, const QString &name)
{
    setFilter(QString("id = %1").arg(id));
    select();

    assert(rowCount() == 1);

    setData(index(0, 1), name);
    setFilter("");
    select(); //还原
}

void AccountTableModel::changePassword(int id, const QString &password)
{
    setFilter(QString("id = %1").arg(id));
    select();

    assert(rowCount() == 1);

    setData(index(0, 2), password);
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
    insertRecord(newId, record);
    //submitAll();
    return newId;
}
