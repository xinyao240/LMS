#include "booktablemodel.h"


BookTableModel::BookTableModel(const QString &type, QObject* parent)
    :QSqlTableModel(parent)
{
    setTable(type);
    select();

    genres->setTable("genres");
    genres->select();
    states->setTable("bookStates");
    states->select();

}

int BookTableModel::addEntry(QString name, int genre)
{
    int newId = rowCount();
    QSqlRecord record = this->record();
    record.setValue("bookId", newId);
    record.setValue("bookName", name);
    record.setValue("genre",genre);
    record.setValue("bookState",BookTableModel::Stored);
    if(!insertRecord(newId,record)||!submitAll()){
        qCritical() << "Fail to insert record into bookTableModel";
        return -1;
    }
    else{
        const QString genreText = genres->data(genres->index(genre,1)).toString();
        const QString stateText = states->data(states->index(BookTableModel::Stored,1)).toString();
        qCritical() << "Add to" << tableName()<<":"
                    <<QString("(%1,%2,%3,%4)").arg(newId).arg(name).arg(genreText).arg(stateText);
        return newId;
    }
}

const QSqlRecord BookTableModel::searchId(int id)
{
    setFilter(QString("bookId = %1").arg(id));
    select();
    if(!(rowCount()==1)) {
        qCritical() << "Error, multiple books assigned with the same ID";
        return this->record();
    }
    QSqlRecord record = this->record(0);
    setFilter("");
    select();//还原
    return record;
}

QList<QSqlRecord> BookTableModel::searchName(const QString &name)
{
    setFilter(QString("bookName like '%1'").arg(QString("%") + name + QString("%")));
    select();
    QList<QSqlRecord> result;
    for(int i = 0; i < rowCount(); ++i){
        result.append(record(i));
    }
    setFilter("");
    select();//还原
    return result;
}

QList<QSqlRecord> BookTableModel::searchAuthor(const QString &authorName)
{
    setFilter(QString("author like '%1'").arg(QString("%") + authorName + QString("%")));
    select();
    QList<QSqlRecord> result;
    for(int i = 0; i < rowCount(); ++i){
        result.append(record(i));
    }
    setFilter("");
    select();//还原
    return result;
}

QList<QSqlRecord> BookTableModel::searchGenre(int genre)
{
    setFilter(QString("genre = %1").arg(genre));
    select();
    QList<QSqlRecord> result;
    for(int i = 0; i < rowCount(); ++i){
        result.append(record(i));
    }
    setFilter("");
    select();//还原
    return result;
}

bool BookTableModel::changeName(int id, const QString &name)
{
    if(!setData(index(id,headerData::bookId),name)||!submitAll())
        return false;
    return true;
}

bool BookTableModel::changeAuthor(int id, const QString& authorName)
{
    if(!setData(index(id,headerData::author),authorName)||!submitAll())
        return false;
    return true;
}

bool BookTableModel::changeGenre(int id, int genre)
{
    if(!setData(index(id,headerData::genre),genre)||!submitAll())
        return false;
    return true;
}

bool BookTableModel::changeState(int id, int state)
{
    if(!setData(index(id,headerData::state),state)||!submitAll())
        return false;
    return true;}
