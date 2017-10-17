#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "node.h"
#include "signindialog.h"
#include "addfrienddialog.h"
#include <QMessageBox>
#include <QListWidgetItem>
#include "msglist.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_signin(QString);
    void on_signup(QString);
    void on_registered(bool);
    void on_friends(bool);
    void on_addFriend();
    void on_recvId(QString);
    void on_userFound(bool, QString);
    void on_friendAdded(bool, QString);
    void on_recvMsg(QString, QString, QString);
    void on_btn_send_clicked();

    void on_listWidget_itemClicked(QListWidgetItem *item);

private:
    Ui::MainWindow *ui;
    SignInDialog* dlg_si;
    Node node;
    QAction *add_friend;
    AddFriendDialog *dlg_af;
    std::vector<MsgList> msgList;
};

#endif // MAINWINDOW_H
