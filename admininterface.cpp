#include "admininterface.h"
#include <QVBoxLayout>
#include <QListWidgetItem>
#include <QWidget>
#include <QAction>
#include <QPushButton>
#include <QDialog>
#include <QHBoxLayout>
#include <QMessageBox>

AdminInterface::AdminInterface(int id, const QString& name, QSharedPointer<AccountTableModel> adminTable,
                               QSharedPointer<BookTableModel> bookTableModel, QWidget *parent)
    : UserInterface(id, name, parent)
{
    this->setWindowTitle("管理界面");
    info = new AdminInformation(id,name);
    main = new QStackedWidget;
    bookAdmin = new BookAdminWidget(bookTableModel);
    readerAdmin = new readermanagementwidget;
    passwordDialog = new AdminPasswordDialog(id,adminTable,this);
    userInfoDlg = new UserInfoDialog(id, name,this);
    QWidget *mainWidget = new QWidget;
    this->setCentralWidget(mainWidget);
    passwordDialog->setWindowIcon(QIcon(":/image/password.png"));

    tabIcon = new QListWidget;
    tabIcon->setViewMode(QListView::IconMode);
    tabIcon->setMovement(QListView::Static);
    tabIcon->setSpacing(20);

    QListWidgetItem *bookItem = new QListWidgetItem(QIcon(":/image/bookIcon.png"),"图书管理");
    QListWidgetItem *readerItem = new QListWidgetItem(QIcon(":/image/adminIcon.png"),"读者管理");
    bookItem->setTextAlignment(Qt::AlignCenter);
    bookItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    readerItem->setTextAlignment(Qt::AlignHCenter);
    readerItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    tabIcon->addItem(bookItem);
    tabIcon->addItem(readerItem);

    QVBoxLayout *tabLayout = new QVBoxLayout;
    tabLayout->addWidget(tabIcon);
    QGroupBox *tabBox = new QGroupBox;
    tabBox->setFixedWidth(140);
    tabBox->setMinimumHeight(250);
    tabBox->setTitle("图书/读者管理");
    tabBox->setLayout(tabLayout);

    QVBoxLayout *left = new QVBoxLayout;
    QPushButton *modifyInfo = new QPushButton;
    modifyInfo->setText("修改个人信息");
    modifyInfo->setIcon(QIcon(":/image/homeIcon.png"));
    QPushButton *modifyPassword = new QPushButton;
    modifyPassword->setText("修改密码");
    modifyPassword->setIcon(QIcon(":/image/password.png"));
    left->addSpacing(10);
    left->addWidget(info);
    left->addWidget(modifyInfo);
    left->addWidget(modifyPassword);
    left->addSpacing(20);
    left->addWidget(tabBox);
    left->setStretch(0,2);
    left->setStretch(1,8);
    left->addStretch(1);

    QVBoxLayout *right = new QVBoxLayout;
    main->addWidget(bookAdmin);
    main->addWidget(readerAdmin);
    main->setCurrentIndex(0);
    right->addWidget(main);
    right->addSpacing(10);
    /**/

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addLayout(left);
    mainLayout->addLayout(right);
    this->centralWidget()->setLayout(mainLayout);


    //    QHBoxLayout *nameBuddy = new QHBoxLayout;
    //    QLabel *nameLabel = new QLabel;
    //    nameLabel->setText("姓名：");
    //    QLineEdit *nameLine = new QLineEdit;
    //    nameLine->setPlaceholderText(name);

    connect(modifyPassword,&QAbstractButton::clicked,passwordDialog,&AdminPasswordDialog::show);
    connect(modifyInfo, &QAbstractButton::clicked,userInfoDlg,&QDialog::show);
    connect(userInfoDlg, &UserInfoDialog::infoRtned, &*adminTable, &AccountTableModel::changeName);
    connect(userInfoDlg, &UserInfoDialog::infoRtned, info, &AdminInformation::update);

    connect(tabIcon, &QListWidget::currentItemChanged,this, &AdminInterface::changePage);

}

void AdminInterface::changePage(QListWidgetItem *current, QListWidgetItem *previous)
{
    if(!current){
        current = previous;
    }
    main->setCurrentIndex(tabIcon->row(current));
}

AdminInformation::AdminInformation(int id, const QString &name, QWidget *parent)
    :UserInformation(id, name, parent)
{
    this->setTitle("管理员信息：");
}

void AdminInformation::update(int id, const QString &name)
{
    Q_UNUSED(id);
    UserInformation::changeName(name);
}

AdminPasswordDialog::AdminPasswordDialog(int id, QSharedPointer<AccountTableModel> admin, QWidget *parent)
    :PasswordDialog(parent), id(id), adminTable(admin)
{
    connect(PasswordDialog::cancelBtn,&QAbstractButton::clicked,this,&AdminPasswordDialog::close);
    connect(PasswordDialog::saveBtn, &QAbstractButton::clicked, this, &AdminPasswordDialog::judge);
}

void AdminPasswordDialog::judge()
{
    if(PasswordDialog::oldPasswordLine->text().isEmpty()){
        QMessageBox::information(this,"消息提示","未输入原密码");
        return;
    }
    if(PasswordDialog::newPasswordLine->text().isEmpty()){
        QMessageBox::information(this,"消息提示","未输入新密码");
        return;
    }
    if(PasswordDialog::newPasswordAgainLine->text().isEmpty()){
        QMessageBox::information(this,"消息提示","请输入新密码两次");
        return;
    }

    const QString& inputOldPassword = PasswordDialog::oldPasswordLine->text();
    adminTable->setFilter(QString("id = %1").arg(id));
    adminTable->select();
    if(adminTable->rowCount()!=1){
        QMessageBox::warning(this,"错误","系统错误");
        adminTable->setFilter("");
        adminTable->select();
        return;
    }
    const QString& correctPassword = adminTable->data(adminTable->index(0,AdminInterface::password)).toString();

    if(inputOldPassword!=correctPassword){
        QMessageBox::information(this,"消息提示","原密码错误");
        adminTable->setFilter("");
        adminTable->select();
        this->close();
        return;
    }

    const QString& newPassword = PasswordDialog::newPasswordLine->text();
    const QString& newPasswordAgain = PasswordDialog::newPasswordAgainLine->text();

    if(newPassword!=newPasswordAgain){
        QMessageBox::information(this,"消息提示","两次输入密码不相同，请重试");
        newPasswordLine->clear();
        newPasswordAgainLine->clear();
        adminTable->setFilter("");
        adminTable->select();
        return;
    }

    if(newPassword==newPasswordAgain){
        adminTable->changePassword(id,newPassword);
        QMessageBox::information(this,"消息提示","修改成功");
        adminTable->setFilter("");
        adminTable->select();
        this->close();
        return;
    }
}

void AdminPasswordDialog::closeEvent(QCloseEvent *event)
{
    oldPasswordLine->clear();
    newPasswordLine->clear();
    newPasswordAgainLine->clear();
    return PasswordDialog::closeEvent(event);
}
