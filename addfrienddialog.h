#ifndef ADDFRIENDDIALOG_H
#define ADDFRIENDDIALOG_H

#include <QDialog>
#include <QMessageBox>

namespace Ui {
class AddFriendDialog;
}

class AddFriendDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddFriendDialog(QWidget *parent = 0);
    ~AddFriendDialog();
    void set_labelid(std::string);

signals:
    void sig_addFriend(QString);

private slots:
    void on_btn_cancel_clicked();

    void on_bt_ok_clicked();

private:
    Ui::AddFriendDialog *ui;
};

#endif // ADDFRIENDDIALOG_H
