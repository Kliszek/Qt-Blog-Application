#ifndef MAINDIALOG_H
#define MAINDIALOG_H

#include <QDialog>
#include <QString>
#include <QList>
#include <QSettings>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include "blogmanager.h"
#include "user.h"

namespace Ui {
class MainDialog;
}

class MainDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MainDialog(const User* user, QWidget *parent = nullptr);
    ~MainDialog();
    const User* m_currentUser;

private:
    QString comboSelected;
    void setCombo(QString id);

    Ui::MainDialog *ui;
    void setValidators();
    bool validateBlogData();
    void updateBlogList();

private slots:
    void on_btnBox_accepted();
    void on_btnBox_rejected();
    void on_btnCreateBlog_clicked();
    void on_chkAutoId_toggled(bool checked);
};

#endif // MAINDIALOG_H
