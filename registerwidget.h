#ifndef REGISTERWIDGET_H
#define REGISTERWIDGET_H

#include "accounttablemodel.h"

#include <QtWidgets>
#include <QSharedPointer>

class RegisterWidget : public QDialog
{
    Q_OBJECT
public:
    RegisterWidget(QWidget *parent = nullptr, QSharedPointer<AccountTableModel> readerTable = nullptr);
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
};

#endif // REGISTERWIDGET_H
