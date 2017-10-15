#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    dlg_si = new SignInDialog(this);
    dlg_si->show();

    connect(dlg_si, &SignInDialog::sig_signin, this, &MainWindow::on_signin);
    connect(dlg_si, &SignInDialog::sig_signup, this, &MainWindow::on_signup);
    connect(&node, &Node::sig_registered, this, &MainWindow::on_registered);
    connect(&node, &Node::sig_friends, this, &MainWindow::on_friends);
}

MainWindow::~MainWindow()
{
    delete ui;
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


    }
}
