/**This class is only provided for reader and administrator
 * model with common behaviours such as the change of
 * name and passward in user interface.
 * */

#ifndef ACCOUNTTABLEMODEL_H
#define ACCOUNTTABLEMODEL_H

#include <QtSql>

class AccountTableModel : public QSqlTableModel
{
    Q_OBJECT
public:
    enum PersonType {
        reader, admin
    };

    AccountTableModel(const QString &type, QObject *parent = nullptr);
    int addEntry(const QString &name, const QString &password); // add an entry and returns the id

public slots:
    void changeName(int id, const QString &name);
    void changePassword(int id, const QString &password);
};

#endif // ACCOUNTTABLEMODEL_H
