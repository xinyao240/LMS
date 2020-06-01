#ifndef REGISTERWIDGET_H
#define REGISTERWIDGET_H

#include "accounttablemodel.h"

#include <QtWidgets>
#include <QSharedPointer>

QT_FORWARD_DECLARE_CLASS(LoginWidget)

class RegisterWidget : public QDialog
{
    Q_OBJECT
public:
    RegisterWidget(QSharedPointer<AccountTableModel> readerTable, LoginWidget *loginWidget,
                    QWidget *parent = nullptr);
    void showEvent(QShowEvent *) override;

signals:
    void accountCreated(int id, const QString password);

private slots:
    void updateRegisterButton();
    void judgeRegister();
    void clearAndBack();

private:
    QSharedPointer<AccountTableModel> readerTable;
    QLineEdit *nameEdit, *passwordEdit, *reinputEdit;
    QTimer *timer;
    QPushButton *registerButton;
    QMessageBox *registerMessage;
    LoginWidget *loginWidget;
};

#endif // REGISTERWIDGET_H
