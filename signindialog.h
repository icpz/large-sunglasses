#ifndef SIGNINDIALOG_H
#define SIGNINDIALOG_H

#include <QDialog>
#include "signupdialog.h"

namespace Ui {
class SignInDialog;
}

class SignInDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SignInDialog(QWidget *parent = 0);
    ~SignInDialog();

signals:
    void sig_signin(QString);
    void sig_signup(QString);

private slots:
    void on_btn_cancel_clicked();
    void on_btn_signin_clicked();
    void on_btn_signup_clicked();
    void on_signup(QString);

private:
    SignUpDialog *dlg_su;
    Ui::SignInDialog *ui;
};

#endif // SIGNINDIALOG_H
