#include "registerwidget.h"
#include "loginwidget.h"
#include "constants.h"

RegisterWidget::RegisterWidget(QSharedPointer<AccountTableModel> readerTable,
                               LoginWidget *loginWidget, QWidget *parent)
    : QDialog(parent), readerTable(readerTable), loginWidget(loginWidget)
{
    QLabel *nameLabel = new QLabel(tr("姓名"), this);
    QLabel *passwordLabel = new QLabel(tr("密码"), this);
    QLabel *reinputLabel = new QLabel(tr("重复密码"), this);

    nameEdit = new QLineEdit(this);
    nameEdit->setPlaceholderText(tr("输入姓名"));
    // To do validator 目前是想实现首尾无空格
    nameEdit->setMaxLength(MAX_NAME_LENGTH);

    passwordEdit = new QLineEdit(this);
    passwordEdit->setPlaceholderText(tr("输入密码"));
    passwordEdit->setAttribute(Qt::WA_InputMethodEnabled, false);
    passwordEdit->setEchoMode(QLineEdit::Password);
    passwordEdit->setValidator(new QRegExpValidator(QRegExp("[\\S]*"), passwordEdit));
    passwordEdit->setMaxLength(MAX_PASSWORD_LENGTH);

    reinputEdit = new QLineEdit(this);
    reinputEdit->setPlaceholderText(tr("重复输入密码"));
    reinputEdit->setAttribute(Qt::WA_InputMethodEnabled, false);
    reinputEdit->setEchoMode(QLineEdit::Password);
    reinputEdit->setValidator(new QRegExpValidator(QRegExp("[\\S]*"), passwordEdit));
    reinputEdit->setMaxLength(MAX_PASSWORD_LENGTH);

    registerButton = new QPushButton(tr("确认注册"), this);
    registerButton->setDisabled(true);
    QPushButton *returnButton = new QPushButton(tr("返回登录"), this);

    QGridLayout *editLayout = new QGridLayout;
    editLayout->addWidget(nameLabel, 0, 0);
    editLayout->addWidget(nameEdit, 0, 1);
    editLayout->addWidget(passwordLabel, 1, 0);
    editLayout->addWidget(passwordEdit, 1, 1);
    editLayout->addWidget(reinputLabel, 2, 0);
    editLayout->addWidget(reinputEdit, 2, 1);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(returnButton, 0, Qt::AlignTop);
    buttonLayout->addWidget(registerButton, 0, Qt::AlignTop);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(editLayout);
    mainLayout->addLayout(buttonLayout);
    // mainLayout->setSizeConstraint(QLayout::SetFixedSize);
    // 使对话框不可放缩

    setLayout(mainLayout);
    setWindowTitle(tr("注册"));
    resize(INITIAL_DIALOG_SIZE);

    timer = new QTimer(this);
    timer->setInterval(100);

    registerMessage = new QMessageBox(QMessageBox::Information, tr("注册成功"), "", QMessageBox::Ok);

    connect(timer, &QTimer::timeout, this, &RegisterWidget::updateRegisterButton);
    connect(registerButton, &QPushButton::clicked, this, &RegisterWidget::judgeRegister);
    connect(returnButton, &QPushButton::clicked, this, &RegisterWidget::clearAndBack);
    connect(this, &RegisterWidget::accountCreated, loginWidget, &LoginWidget::autoFill);
}

//restart timer after shown
//在展示后重新计时
void RegisterWidget::showEvent(QShowEvent * event)
{
    QWidget::showEvent(event);
    timer->start();
}

//set register button enabled while all line editor have text
//仅当所有输入框都有文本时才启用按钮
void RegisterWidget::updateRegisterButton()
{
    if (!nameEdit->text().isEmpty() && !passwordEdit->text().isEmpty()
            && !reinputEdit->text().isEmpty())
        registerButton->setEnabled(true);
    else
        registerButton->setDisabled(true);
}

void RegisterWidget::judgeRegister()
{
    const QString name = nameEdit->text();
    const QString password = passwordEdit->text();
    const QString reinput = reinputEdit->text();

    if (password.length() < MIN_PASSWORD_LENGTH) {
        QMessageBox::warning(this, tr("密码长度过短"), tr("请输入至少6位密码以保证安全性"));
        return;
    }

    if (password != reinput) {
        QMessageBox::warning(this, tr("两次密码输入不一致"), tr("请检查两次输入的密码是否一致"));
        return;
    }

    int newId = readerTable->addEntry(name, password);

    if (newId == -1) {
        QMessageBox::critical(this, tr("程序出错"), tr("注册失败。请联系管理员或开发者获取支持。"));
        return;
    }
    emit accountCreated(newId, password);
    registerMessage->setText(tr("您的ID是：%1").arg(newId));

    if (registerMessage->exec() == QMessageBox::Ok)
        clearAndBack();
}

void RegisterWidget::clearAndBack()
{
    timer->stop(); //停止计时
    nameEdit->clear();
    passwordEdit->clear();
    reinputEdit->clear();
    this->hide();
    loginWidget->show();
}
