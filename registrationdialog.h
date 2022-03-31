#ifndef REGISTRATIONDIALOG_H
#define REGISTRATIONDIALOG_H

#include <QDialog>

namespace Ui {
class registrationDialog;
}

class registrationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit registrationDialog(QWidget *parent = nullptr);
    ~registrationDialog();

private:
    Ui::registrationDialog *ui;
};

#endif // REGISTRATIONDIALOG_H
