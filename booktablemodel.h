#ifndef BOOKTABLEMODEL_H
#define BOOKTABLEMODEL_H
#include <QtSql>

class BookTableModel: public QSqlTableModel
{
public:
    enum bookStates {
        Stored, Borrowed, Lost
    };
    enum headerData{
        bookId, bookName, author, genre, state
    };

    BookTableModel(const QString &type, QObject* parent = 0);
    int addEntry(QString name, int genre);
    const QSqlRecord searchId(int id);
    QList<QSqlRecord> searchName(const QString& name);
    QList<QSqlRecord> searchAuthor(const QString& authorName);
    QList<QSqlRecord> searchGenre(int genre);
    bool changeName(int id, const QString& name);
    bool changeAuthor(int id, const QString& authorName);
    bool changeGenre(int id, int genre);
    bool changeState(int id, int state);
private:
    QSqlTableModel * genres;
    QSqlTableModel * states;
};

#endif // BOOKTABLEMODEL_H
