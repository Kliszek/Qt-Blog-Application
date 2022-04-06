#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QString>
#include <QMessageBox>
#include <QSettings>
#include <QDir>
#include <QFileDialog>
#include "user.h"

QT_BEGIN_NAMESPACE
namespace Ui { class LoginDialog; }
QT_END_NAMESPACE

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    LoginDialog(const User* loggedUser = nullptr, QWidget *parent = nullptr);
    ~LoginDialog();

private slots:

    void on_btnLogin_clicked();

    void on_btnRegister_clicked();

    void on_btnChangeFolder_clicked();

private:
    Ui::LoginDialog *ui;

    bool tryLogin();

};
#endif // LOGINDIALOG_H
