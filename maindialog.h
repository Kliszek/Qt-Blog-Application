#ifndef MAINDIALOG_H
#define MAINDIALOG_H

#include <QDialog>
#include "blogmanager.h"

namespace Ui {
class MainDialog;
}

class MainDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MainDialog(QWidget *parent = nullptr);
    ~MainDialog();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::MainDialog *ui;
};

#endif // MAINDIALOG_H
