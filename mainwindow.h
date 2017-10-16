#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "node.h"
#include "signindialog.h"
#include "addfrienddialog.h"
#include <QMessageBox>

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
    void on_friendAdded(bool);

private:
    Ui::MainWindow *ui;
    SignInDialog* dlg_si;
    Node node;
    QAction *add_friend;
    AddFriendDialog *dlg_af;
};

#endif // MAINWINDOW_H
