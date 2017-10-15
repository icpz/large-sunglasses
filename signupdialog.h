#ifndef SIGNUPDIALOG_H
#define SIGNUPDIALOG_H

#include <QDialog>
#include <QMessageBox>

namespace Ui {
class SignUpDialog;
}

class SignUpDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SignUpDialog(QWidget *parent = 0);
    ~SignUpDialog();

signals:
    void sig_signup(QString);

private slots:
    void on_btn_ok_clicked();

    void on_btn_cancel_clicked();

private:
    Ui::SignUpDialog *ui;
};

#endif // SIGNUPDIALOG_H
