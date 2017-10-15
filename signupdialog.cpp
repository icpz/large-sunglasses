#include "signupdialog.h"
#include "ui_signupdialog.h"

SignUpDialog::SignUpDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SignUpDialog)
{
    ui->setupUi(this);
}

SignUpDialog::~SignUpDialog()
{
    delete ui;
}

void SignUpDialog::on_btn_ok_clicked()
{
    if ((ui->lineEdit->text().size() != 0) && (ui->lineEdit->text() == ui->lineEdit_2->text())) {
        this->close();
        emit this->sig_signup(ui->lineEdit->text());
    } else {
        QMessageBox::warning(this, "Warning", "请输入密码并确认一致！");
    }
}

void SignUpDialog::on_btn_cancel_clicked()
{
    this->close();
}
