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

    if(!BlogManager::checkAvailability(ui->txtId->text(), ui->txtBlogTitle->text()))
    {
        QMessageBox::critical(this, "Error", "This blog id or title is already in use!");
        //ui->txtId->setStyleSheet("QLineEdit {border: 1px solid red;}");
        return false;
    }
    return valid;

}

void MainDialog::updateBlogList()
{
    QList<Blog>* blogList = BlogManager::getBlogList();
    ui->lstBlogList->clear();
    ui->cmbBlogList->clear();
    ui->lstAllBlogs->clear();

    for(int i=0; i<blogList->size(); i++)
    {
        if(blogList->at(i).m_ownerId == m_currentUser->getId())
        {
            ui->lstBlogList->addItem(blogList->at(i).m_title);
            ui->cmbBlogList->addItem(blogList->at(i).m_title);
        }
        ui->lstAllBlogs->addItem(blogList->at(i).m_title);
    }
    if(ui->lstBlogList->count() == 0)
    {
        ui->lstBlogList->addItem("<you have no blogs>");
        ui->cmbBlogList->addItem("<you have no blogs>");
        ui->cmbBlogList->setEnabled(false);
        ui->lstBlogList->setEnabled(false);
        if(ui->lstAllBlogs->count() == 0)
        {
            ui->lstAllBlogs->addItem("<there are no blogs to display>");
            ui->lstAllBlogs->setEnabled(false);
        }
        else
            ui->lstAllBlogs->setEnabled(true);
    }
    else
    {
        ui->cmbBlogList->setEnabled(true);
        ui->lstBlogList->setEnabled(true);
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

void MainDialog::displayEntry(const BlogEntry* entry, const User* user, QWidget *wrapper)
{
    QFrame* newEntry = new QFrame(wrapper);
    QVBoxLayout* layout = new QVBoxLayout(newEntry);

    newEntry->setFrameStyle(QFrame::Box);
    newEntry->setLayout(layout);
    newEntry->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);

    QLabel* entryUser = new QLabel(newEntry);
    if(user == nullptr)
    {
        qCritical() << "There is a blog with an invalid owner id!";
        entryUser->setText("Added by: <invalid user id>");
    }
    else
    {
        entryUser->setText("Added by: " + user->m_username);
    }
    entryUser->setFont(QFont("Segoe", 7));
    entryUser->setAlignment(Qt::AlignRight);
    layout->addWidget(entryUser);

    QGroupBox* entryTitle = new QGroupBox(newEntry);
    entryTitle->setLayout(new QVBoxLayout(entryTitle));
    entryTitle->setTitle(entry->m_title);
    entryTitle->setFont(QFont("Segoe", 12));
    newEntry->layout()->addWidget(entryTitle);

    QLabel* entryContents = new QLabel(entryTitle);
    entryContents->setText(entry->m_content);
    entryContents->setWordWrap(true);
    entryContents->setFont(QFont("Segoe", 9));
    entryTitle->layout()->addWidget(entryContents);

    QLabel* entryDate = new QLabel(newEntry);
    entryDate->setText("Added on: " + entry->m_date.toString());
    entryDate->setAlignment(Qt::AlignRight);
    entryDate->setFont(QFont("Segoe", 7));
    layout->addWidget(entryDate);
    //qInfo() << newEntry->layout();

    //newEntry->layout()->addWidget(entryTitle);


    //newEntry->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    //newEntry->setFixedHeight(100);
    newEntry->setContentsMargins(10,10,10,10);

    wrapper->layout()->setAlignment(Qt::AlignTop);
    wrapper->layout()->addWidget(newEntry);
}

void MainDialog::displayBlog(const Blog *blog, QWidget *wrapper)
{
    for(QFrame* entry : wrapper->findChildren<QFrame*>(QString(), Qt::FindDirectChildrenOnly))
    {
        delete(entry);
    }

    for(int i=0; i<blog->m_entryList->size(); i++)
    {
        displayEntry(&blog->m_entryList->at(i), UserManager::getUserById(blog->m_ownerId), wrapper);
    }
}

void MainDialog::on_lstBlogList_itemSelectionChanged()
{
    QString selected = ui->lstBlogList->selectedItems().at(0)->text();

    const Blog* blog = BlogManager::getBlogByTitle(selected);
    displayBlog(blog, ui->wgtBlogEntries);

}


void MainDialog::on_lstAllBlogs_itemSelectionChanged()
{
    QString selected = ui->lstAllBlogs->selectedItems().at(0)->text();

    const Blog* blog = BlogManager::getBlogByTitle(selected);
    displayBlog(blog, ui->wgtAllBlogEntries);
}

