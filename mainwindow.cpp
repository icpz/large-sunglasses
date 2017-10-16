#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    add_friend = new QAction(QIcon(":/add-contact.jpg"), tr("添加好友"), this);
    add_friend->setStatusTip("添加好友");
    ui->toolBar->addAction(add_friend);
    dlg_af = new AddFriendDialog(this);
    dlg_si = new SignInDialog(this);
    dlg_si->show();

    connect(dlg_si, &SignInDialog::sig_signin, this, &MainWindow::on_signin);
    connect(dlg_si, &SignInDialog::sig_signup, this, &MainWindow::on_signup);
    connect(&node, &Node::sig_registered, this, &MainWindow::on_registered);
    connect(&node, &Node::sig_friends, this, &MainWindow::on_friends);
    connect(add_friend, &QAction::triggered, this, &MainWindow::on_addFriend);
    connect(dlg_af, &AddFriendDialog::sig_addFriend, this, &MainWindow::on_recvId);
    connect(&node, &Node::sig_user_found, this, &MainWindow::on_userFound);
    connect(&node, &Node::sig_addFriend, this, &MainWindow::on_friendAdded);

}

MainWindow::~MainWindow()
{
    delete ui;
    delete add_friend;
}

void MainWindow::on_signin(QString pw)
{
    if (node.checkid(pw)) {
        dlg_si->close();
        delete dlg_si;
        node.conn();
        ui->statusbar->showMessage(tr("正在读取账号信息。。。"));
        this->show();
    } else {
        QMessageBox::critical(dlg_si, "错误", "登录凭据密码错误！");
    }
}

void MainWindow::on_signup(QString pw)
{
    if (node.signup(pw)) {
        QMessageBox::information(dlg_si, "操作成功", "恭喜，注册成功！请妥善保管登陆凭据。");
    } else {
        QMessageBox::critical(dlg_si, "Error", "很抱歉，注册失败。。。");
    }
}

void MainWindow::on_registered(bool b)
{
    dlg_af->set_labelid(node.getId());
    if (!b) {
        ui->statusbar->showMessage(tr("信息读取完毕！"));
    }
}

void MainWindow::on_friends(bool b)
{
    if (!b) {
        ui->statusbar->showMessage(tr("信息读取失败。。。"));
    } else {
        ui->statusbar->showMessage(tr("信息读取完毕！"));
        for (std::string f: this->node.g_friends()) {
            ui->listWidget->addItem(QString::fromStdString(f));
        }
    }
}

void MainWindow::on_addFriend()
{
    dlg_af->show();
}

void MainWindow::on_recvId(QString id)
{
    node.search_user(id);
}

void MainWindow::on_userFound(bool b, QString id)
{
    if (!b) {
        QMessageBox::critical(dlg_af, "Error", "未找到此用户！");
    } else {
        node.add_friend(id);
        dlg_af->close();
    }
}

void MainWindow::on_friendAdded(bool b)
{
    if (!b) {
        QMessageBox::critical(this, "Error", "添加好友失败。。。");
    } else {
        qDebug("abc");
        ui->listWidget->addItem(QString::fromStdString(node.g_friends().back()));
    }
}
