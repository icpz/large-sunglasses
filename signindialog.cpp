#include "signindialog.h"
#include "ui_signindialog.h"

SignInDialog::SignInDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SignInDialog)
{
    ui->setupUi(this);
    dlg_su = new SignUpDialog(this);
    connect(dlg_su, &SignUpDialog::sig_signup, this, &SignInDialog::on_signup);
}

SignInDialog::~SignInDialog()
{
    delete ui;
    delete dlg_su;
}

void SignInDialog::on_btn_cancel_clicked()
{
    this->close();
}

void SignInDialog::on_btn_signin_clicked()
{
    emit sig_signin(ui->lineEdit->text());
}

void SignInDialog::on_btn_signup_clicked()
{
    dlg_su->show();
}

void SignInDialog::on_signup(QString pw)
{
    emit sig_signup(pw);
}
