#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "node.h"
#include "signindialog.h"
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

private:
    Ui::MainWindow *ui;
    SignInDialog* dlg_si;
    Node node;
};

#endif // MAINWINDOW_H
