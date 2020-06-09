#include "overtimerecordtab.h"

overtimeRecordTab::overtimeRecordTab(QWidget *parent) : QWidget(parent)
{
    model= new QSqlTableModel(this);
    model->setTable("overtimeRecord");

    view=new QTableView(this);
    view->setModel(model);
    //view->setColumnHidden(1,true);
    view->resizeColumnsToContents();
    view->setEditTriggers(QAbstractItemView::NoEditTriggers);
    view->setSelectionBehavior(QAbstractItemView::SelectRows);
    view->setSelectionMode(QAbstractItemView::SingleSelection);

    idSearch=new QLineEdit(this);
    idSearch->setPlaceholderText("请输入bookId");
    idSearch->setValidator(new QIntValidator(idSearch));
    idSearch->setAttribute(Qt::WA_InputMethodEnabled,false);

    nameSearch=new QLineEdit(this);
    nameSearch->setPlaceholderText("请输入书名");
    nameSearch->setMaxLength(MAX_NAME_LENGTH);
    nameSearch->setAttribute(Qt::WA_InputMethodEnabled,false);
    nameSearch->setValidator(new QRegExpValidator(QRegExp("[\\S]*"), nameSearch));

    idSearchButton=new QPushButton(tr("ID查询"),this);
    nameSearchButton=new QPushButton(tr("书名查询"),this);
    fulltableButton=new QPushButton(tr("显示全部"),this);
    connect(idSearchButton,&QPushButton::clicked,this,&overtimeRecordTab::searchID);
    connect(nameSearchButton,&QPushButton::clicked,this,&overtimeRecordTab::searchName);
    connect(fulltableButton,&QPushButton::clicked,this,&overtimeRecordTab::fulltable);

    QGridLayout *searchLayout=new QGridLayout;
    searchLayout->addWidget(idSearch,0,0);
    searchLayout->addWidget(idSearchButton,0,1);
    searchLayout->addWidget(nameSearch,1,0);
    searchLayout->addWidget(nameSearchButton,1,1);
    searchLayout->addWidget(fulltableButton,1,3);
    searchLayout->setColumnStretch(0,3);
    searchLayout->setColumnStretch(1,1);
    searchLayout->setColumnStretch(2,1);
    searchLayout->setColumnStretch(3,1);
    searchLayout->setSpacing(10);

    QGridLayout *mainLayout=new QGridLayout;
    mainLayout->addWidget(view,0,0);
    mainLayout->addLayout(searchLayout,1,0);
    mainLayout->setRowStretch(0,5);
    mainLayout->setRowStretch(1,1);
    mainLayout->setSpacing(20);

    setLayout(mainLayout);
}

void overtimeRecordTab::searchID()
{
    int id=idSearch->text().toInt();
    model->setFilter(QString("bookId='%1'").arg(id));
    model->select();

    if(model->rowCount()==0)
    {
        qCritical().noquote()<<QString("Fail to find : ID '%1'").arg(id);
    }

    return;
}

void overtimeRecordTab::searchName()
{
    QString name=nameSearch->text();
    model->setFilter(QString("bookName='%1'").arg(name));
    model->select();

    if(model->rowCount()==0)
    {
        qCritical().noquote()<<QString("Fail to find : Name '%1'").arg(name);
    }

    return;
}

void overtimeRecordTab::fulltable()
{
    model->setTable("overtimeRecord");
    model->select();
}
