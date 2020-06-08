#ifndef BORROWRETURNTAB_H
#define BORROWRETURNTAB_H

#include <QObject>
#include <QWidget>
#include<QtWidgets>
#include<QtSql>
#include"constants.h"

class borrowReturnTab : public QWidget
{
    Q_OBJECT
public:
    explicit borrowReturnTab(QWidget *parent = nullptr);

signals:

private slots:
    void borrowBook();
    void returnBook();
private:
    QLineEdit *idBorrow;
    QLineEdit *bookBorrow;
    QLineEdit *idReturn;
    QLineEdit *bookReturn;
    QPushButton *borrowButton;
    QPushButton *returnButton;
};

#endif // BORROWRETURNTAB_H
