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
#include <QHBoxLayout>
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
    bool validateEntryData();
    void updateBlogList(QString selectedBlog = "");
    void displayEntry(const BlogEntry *entry, const User *user, QWidget *wrapper);
    void displayBlog(const Blog *blog, QWidget* wrapper);
    void clearBlogs(QWidget* wrapper);
    void clearInputs();

private slots:
    void on_btnBox_accepted();
    void on_btnBox_rejected();
    void on_btnCreateBlog_clicked();
    void on_chkAutoId_toggled(bool checked);
    void on_lstBlogList_itemSelectionChanged();
    void on_lstAllBlogs_itemSelectionChanged();
    void on_btnCreateEntry_clicked();
};

#endif // MAINDIALOG_H
