#ifndef MAINDIALOG_H
#define MAINDIALOG_H

#include <QDialog>
#include <QString>
#include <QList>
#include <QSettings>
#include <QListWidgetItem>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QFrame>
#include <QLabel>
#include <QGroupBox>
#include <QVBoxLayout>
#include "blogmanager.h"
#include "user.h"

namespace Ui {
class MainDialog;
}

class MainDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MainDialog(const User* user, QWidget* loginDialog, QWidget *parent = nullptr);
    ~MainDialog();
    const User* m_currentUser;

private:
    QString comboSelected;
    void setCombo(QString id);

    Ui::MainDialog *ui;
    void setValidators();
    bool validateBlogData();
    bool validateEntryData();
    void updateBlogList();
    void displayEntry(const BlogEntry *entry, const User *user, QWidget *wrapper);
    void displayBlog(const Blog *blog, QWidget* wrapper);
    void clearBlogs(QWidget* wrapper);
    void clearInputs();
    void trimInputs();
    QWidget* m_loginDialog;

private slots:
    void on_btnLogOut_clicked();
    void on_btnCreateBlog_clicked();
    void on_chkAutoId_toggled(bool checked);
    void on_lstBlogList_itemSelectionChanged();
    void on_lstAllBlogs_itemSelectionChanged();
    void on_btnCreateEntry_clicked();
    void on_btnDeleteBlog_clicked();
};

#endif // MAINDIALOG_H
