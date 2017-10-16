#include "addfrienddialog.h"
#include "ui_addfrienddialog.h"

AddFriendDialog::AddFriendDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddFriendDialog)
{
    ui->setupUi(this);
}

AddFriendDialog::~AddFriendDialog()
{
    delete ui;
}

void AddFriendDialog::set_labelid(std::string id)
{
    ui->label_id->setText(tr(id.c_str()));
}

void AddFriendDialog::on_btn_cancel_clicked()
{
    this->close();
}

void AddFriendDialog::on_bt_ok_clicked()
{
    if (ui->lineEdit->text().size() != 40) {
        QMessageBox::critical(this, "Error", "请确认用户ID长度!");
    } else {
        emit sig_addFriend(ui->lineEdit->text());
    }
}
