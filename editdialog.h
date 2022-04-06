#ifndef EDITDIALOG_H
#define EDITDIALOG_H

#include <QDialog>
#include <QString>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include "blogmanager.h"

namespace Ui {
class EditDialog;
}

class EditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditDialog(const Blog *parentBlog, int entryIndex, QWidget *parent = nullptr);
    ~EditDialog();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::EditDialog *ui;
    void setValidators();
    bool validateEntryData();
    void trimInputs();
    const Blog* m_parentBlog;
    const int m_entryIndex;
};

#endif // EDITDIALOG_H
