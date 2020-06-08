#include "readermanagementwidget.h"

readermanagementwidget::readermanagementwidget(QWidget *parent) : QWidget(parent)
{
    tabWidget= new QTabWidget;
    tabWidget->addTab(new readerInfoTab,tr("读者基本信息"));
    tabWidget->addTab(new newReaderTab,tr("新增读者栏"));
    tabWidget->addTab(new overtimeRecordTab,tr("借阅超时名单"));
    tabWidget->addTab(new borrowReturnTab,tr("借阅/归还"));

    QGridLayout *mainLayout=new QGridLayout;
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->addWidget(tabWidget);
    setLayout(mainLayout);
}
