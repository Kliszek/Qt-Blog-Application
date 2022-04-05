#include "maindialog.h"
#include "ui_maindialog.h"

MainDialog::MainDialog(const User *user, QWidget *parent) :QDialog(parent), ui(new Ui::MainDialog), m_currentUser(user)
{
    ui->setupUi(this);
    ui->txtUsername->setText(m_currentUser->m_username);
    setValidators();
    updateBlogList();
}

MainDialog::~MainDialog()
{
    qInfo() << "Main window deconstructed";
    delete ui;
}

void MainDialog::on_btnBox_accepted()
{
    BlogManager::loadBlogs();
    BlogManager::saveBlogs();
}


void MainDialog::on_btnBox_rejected()
{
    QSettings settings(QSettings::UserScope);
    settings.clear();
    this->close();
    this->parentWidget()->show();
    delete(this);
}

void MainDialog::setValidators()
{
    QRegularExpression rxNormal("^[a-zA-Z0-9 _\\-]{3,30}$");
    QRegularExpression rxId("^[a-zA-Z0-9]{4,32}$");
    ui->txtId->setValidator(new QRegularExpressionValidator(rxId, this));
    ui->txtBlogTitle->setValidator(new QRegularExpressionValidator(rxNormal, this));
}

bool MainDialog::validateBlogData()
{

    bool valid = true;

    for(QLineEdit* field : ui->boxBlogForm->findChildren<QLineEdit*>())
    {
        if(!field->hasAcceptableInput())
        {
            field->setStyleSheet("QLineEdit {border: 1px solid red;}");
            valid = false;
        }
        else
        {
            field->setStyleSheet("");
        }
    }

    if(!BlogManager::checkAvailability(ui->txtId->text()))
    {
        QMessageBox::critical(this, "Error", "This blog id is already in use!");
        ui->txtId->setStyleSheet("QLineEdit {border: 1px solid red;}");
        return false;
    }
    return valid;

}

void MainDialog::updateBlogList()
{
    QList<Blog>* blogList = BlogManager::getBlogList();
    ui->lstBlogList->clear();
    ui->cmbBlogList->clear();

    for(int i=0; i<blogList->size(); i++)
    {
        if(blogList->at(i).m_ownerId == m_currentUser->getId())
        {
            ui->lstBlogList->addItem(blogList->at(i).m_title);
            ui->cmbBlogList->addItem(blogList->at(i).m_title);
        }
    }
    if(ui->lstBlogList->count() == 0)
    {
        ui->lstBlogList->addItem("<you have no blogs>");
        ui->cmbBlogList->addItem("<you have no blogs>");
    }
    ui->tabWidget->setCurrentIndex(1);
}

void MainDialog::on_btnCreateBlog_clicked()
{
    if(!validateBlogData())
    {
        QMessageBox::critical(this, "Error", "Provided data is not correct!");
        return;
    }
    qInfo()<<"AAAAAAAAAAAA";
    QString blogId = ui->txtId->text();
    QString ownerId = this->m_currentUser->getId();
    QString title = ui->txtBlogTitle->text();

    Blog newBlog(blogId, ownerId, title, new QList<BlogEntry>);

    BlogManager::getBlogList()->append(newBlog);
    BlogManager::saveBlogs();
    updateBlogList();
}


void MainDialog::on_chkAutoId_toggled(bool checked)
{
    if(checked)
    {
        ui->txtId->setEnabled(false);
        ui->txtId->setText(QUuid::createUuid().toString(QUuid::Id128));
        ui->txtId->setStyleSheet("");
    }
    else
    {
        ui->txtId->setEnabled(true);
        ui->txtId->clear();
    }
}

