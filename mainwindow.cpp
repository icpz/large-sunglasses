#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <qdebug.h>

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
    connect(&node, &Node::sig_recv_msg, this, &MainWindow::on_recvMsg);

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
    node.listenMessage();
    node.listen_friend();
}

void MainWindow::on_friends(bool b)
{
    if (!b) {
        ui->statusbar->showMessage(tr("信息读取失败。。。"));
    } else {
        ui->statusbar->showMessage(tr("信息读取完毕！"));
        for (std::string f: this->node.g_friends()) {
            ui->listWidget->addItem(QString::fromStdString(f));
            msgList.insert(msgList.end(), MsgList(f));
        }
    }
}

void MainWindow::on_addFriend()
{
    dlg_af->show();
}

void MainWindow::on_recvId(QString id)
{
    bool found = 0;
    for (std::string fid: node.g_friends()) {
        if (fid == id.toStdString()) {
            found = 1;
            break;
        }
    }
    if (!found)
        node.search_user(id);
    else
        QMessageBox::information(dlg_af, "Info", "此用户已经是你的好友！");
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

void MainWindow::on_friendAdded(bool b, QString u)
{
    qDebug() << "abcd";
    if (!b) {
        QMessageBox::critical(this, "Error", "添加好友失败。。。");
    } else {
        ui->listWidget->addItem(u);
        msgList.insert(msgList.end(), MsgList(u.toStdString()));
    }
}

void MainWindow::on_btn_send_clicked()
{
    long t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::stringstream ss;
    ss << std::put_time(std::localtime(&t), "%Y-%m-%d %X");
    QString date = QString::fromStdString(ss.str());
    QString id = ui->listWidget->currentItem()->text();
    QString text = ui->plainTextEdit->document()->toPlainText();
    node.sendMessage(id.toStdString(), text, t);
    ui->plainTextEdit->clear();
    ui->textBrowser->append(date);
    ui->textBrowser->append(text);
    ui->textBrowser->append(tr("\n"));

   for (MsgList &ml : msgList) {
       if (ml.getUser() == id.toStdString()) {
           ml.add_msg(text, date);
           break;
       }
   }
}

void MainWindow::on_recvMsg(QString from, QString msg, QString date)
{
    int flag = 0;
    for (MsgList &ml : msgList) {
        if (from.toStdString() == ml.getUser()) {
            flag = 1;
            ml.add_msg(msg, date);
            break;
        }
    }
    if (!flag)
        return;

    if (from == ui->listWidget->currentItem()->data(0)) {
        ui->textBrowser->append(date);
        ui->textBrowser->append(msg);
        ui->textBrowser->append(tr("\n"));
    } else {
        QList<QListWidgetItem *> item = ui->listWidget->findItems(from, 0);
        item.back()->setTextColor(QColor("red"));
    }
}

void MainWindow::on_listWidget_itemClicked(QListWidgetItem *item)
{
    item->setTextColor(QColor("black"));
    QString user = item->data(0).toString();
    for (MsgList &ml : msgList) {
        if (ml.getUser() == user.toStdString()) {
            ui->textBrowser->clear();
            for (MsgList::Msg m : ml.getMsgs()) {
                ui->textBrowser->append(QString::fromStdString(m.date));
                ui->textBrowser->append(QString::fromStdString(m.msg));
                ui->textBrowser->append(tr("\n"));
            }
            break;
        }
    }
}
