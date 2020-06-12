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
    const QString oldFilter = this->filter();

    setFilter(QString("id = %1").arg(id));
    select();

    if (rowCount() == 0) {
        qCritical().noquote() << QString("Fail to change Name: Id %1 not found in %2.").arg(id).arg(tableName());
        return;
    } else if (rowCount() != 1) {
        qCritical().noquote() << QString("Fail to change Name: Id %1 is duplicate in %2.").arg(id).arg(tableName());
        return;
    }

    if (!setData(index(0, 1), name) || !submitAll()) {
        qWarning() << "Fail to change name in" << tableName() + ":" << lastError();
    } else {
        qInfo() << "Account id" << id << "in" << tableName() + ": name changed to " << name;
    }

    setFilter(oldFilter);
    select(); //还原
}

void AccountTableModel::changePassword(int id, const QString &password)
{
    const QString oldFilter = this->filter();

    setFilter(QString("id = %1").arg(id));
    select();

    if (rowCount() == 0) {
        qCritical().noquote() << QString("Fail to change Password: Id %1 not found in %2.").arg(id).arg(tableName());
        return;
    } else if (rowCount() != 1) {
        qCritical().noquote() << QString("Fail to change Password: Id %1 is duplicate in %2.").arg(id).arg(tableName());
        return;
    }

    if (!setData(index(0, 2), password) || !submitAll()) {
        qWarning() << "Fail to change password" << tableName() + ":" << lastError();
    } else {
        qInfo() << "Account id" << id << "in" << tableName() + ": password changed";
    }

    setFilter(oldFilter);
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
        // submitAll(); 
        // If insertion strategy set to onManualSubmit, cancel the comment.
        qInfo().noquote() <<  "Add account to" << tableName() + ":"
                 << QString("(%1, %2)").arg(newId).arg(name);
        // password is hidden for security
    }

    //submitAll();
    return newId;
}

QVariant AccountTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal) {
        if (role == Qt::DisplayRole) {
            if (section >= 0 && section < columnCount()) {
                QStringList header;
                header << tr("id") << tr("密码") << tr("姓名"); // translation
                return QVariant(header.at(section));
            } else {
                qWarning() << "Wrong header index for accountTable" << tableName() + ":"  << section;
                return QVariant();
            }
        } else {
            if (section >= 0 && section < columnCount()) {
                QStringList header;
                header << "id" << "password" << "name";
                return QVariant(header.at(section));
            } else {
                qWarning() << "Wrong header index for accountTable" << tableName() + ":"  << section;
                return QVariant();
            }
        }
    } else {
        return QSqlTableModel::headerData(section, orientation, role);
    }
}
