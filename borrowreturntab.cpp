#include "borrowreturntab.h"

borrowReturnTab::borrowReturnTab(QWidget *parent) : QWidget(parent)
{
    QGroupBox *borrowGroup = new QGroupBox(tr("借阅"));
    QGroupBox *returnGroup = new QGroupBox(tr("归还"));

    idBorrow=new QLineEdit;
    idBorrow->setPlaceholderText(tr("请输入读者ID"));
    bookBorrow=new QLineEdit;
    bookBorrow->setPlaceholderText(tr("请输入书籍ID"));
    idReturn=new QLineEdit;
    idReturn->setPlaceholderText(tr("请输入读者ID"));
    bookReturn=new QLineEdit;
    bookReturn->setPlaceholderText(tr("请输入书籍ID"));

    borrowButton=new QPushButton(tr("确定"));
    returnButton=new QPushButton(tr("确定"));

    connect(borrowButton,&QPushButton::clicked,this,&borrowReturnTab::borrowBook);
    connect(returnButton,&QPushButton::clicked,this,&borrowReturnTab::returnBook);

    QGridLayout *borrowLayout=new QGridLayout;
    borrowLayout->addWidget(idBorrow,0,0);
    borrowLayout->addWidget(bookBorrow,1,0);
    borrowLayout->addWidget(borrowButton,1,1);
    borrowLayout->setColumnStretch(0,3);
    borrowLayout->setColumnStretch(1,1);
    borrowLayout->setSpacing(10);

    QGridLayout *returnLayout=new QGridLayout;
    returnLayout->addWidget(idReturn,0,0);
    returnLayout->addWidget(bookReturn,1,0);
    returnLayout->addWidget(returnButton,1,1);
    returnLayout->setColumnStretch(0,3);
    returnLayout->setColumnStretch(1,1);
    returnLayout->setSpacing(10);

    borrowGroup->setLayout(borrowLayout);
    returnGroup->setLayout(returnLayout);

    QVBoxLayout *mainLayout=new QVBoxLayout;
    mainLayout->addWidget(borrowGroup);
    mainLayout->addWidget(returnGroup);
    setLayout(mainLayout);

}

void borrowReturnTab::borrowBook()
{
    QSqlError error;



    if (error.type() != QSqlError::NoError) {
        qWarning() <<  "Error in borrowBook: " << error;
        // 建议在需要输出调试/错误消息时这样写，方便后面进行日志的格式化与管理
    }
}

void borrowReturnTab::returnBook()
{
    QSqlError error;



    if (error.type() != QSqlError::NoError) {
        qWarning() <<  "Error in returnBook: " << error;
        // 建议在需要输出调试/错误消息时这样写，方便后面进行日志的格式化与管理
    }
}
