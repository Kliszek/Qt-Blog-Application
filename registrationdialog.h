#ifndef REGISTRATIONDIALOG_H
#define REGISTRATIONDIALOG_H

#include <QDialog>
#include <QMessageBox>
#include <QString>
#include <QStandardPaths>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QIODevice>
#include <QTextStream>
#include <QList>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QUuid>
#include <QRegularExpression>
#include <QRegularExpressionValidator>

namespace Ui {
class RegistrationDialog;
}

class RegistrationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RegistrationDialog(QWidget *parent = nullptr);
    ~RegistrationDialog();

private slots:
    void on_btnRegister_clicked();

    void on_chkAutoId_toggled(bool checked);

private:
    Ui::RegistrationDialog *ui;
    bool validateCredentials();
    void setValidators();
    void trimInputs();
};

#endif // REGISTRATIONDIALOG_H
