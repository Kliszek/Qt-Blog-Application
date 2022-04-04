#ifndef MAINDIALOG_H
#define MAINDIALOG_H

#include <QDialog>
#include <QString>
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
    Ui::MainDialog *ui;

private slots:
    void on_buttonBox_accepted();
};

#endif // MAINDIALOG_H
