#ifndef READERMANAGEMENTWIDGET_H
#define READERMANAGEMENTWIDGET_H

#include <QObject>
#include <QWidget>
#include<QTabWidget>
#include"readerinfotab.h"
#include"newreadertab.h"
#include"overtimerecordtab.h"
#include "borrowreturntab.h"

class readermanagementwidget : public QWidget
{
    Q_OBJECT
public:
    explicit readermanagementwidget(QWidget *parent = nullptr);

signals:

private:
    QTabWidget *tabWidget;

};

#endif // READERMANAGEMENTWIDGET_H
