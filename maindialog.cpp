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
    ui->txtEntryTitle->setValidator(new QRegularExpressionValidator(rxNormal, this));
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

bool MainDialog::validateEntryData()
{

    bool valid = true;

    if(!ui->txtEntryTitle->hasAcceptableInput())
    {
        ui->txtEntryTitle->setStyleSheet("QLineEdit {border: 1px solid red;}");
        valid = false;
    }
    else
    {
        ui->txtEntryTitle->setStyleSheet("");
    }

    if(ui->txtEntryContent->toPlainText().length() < 10)
    {
        ui->txtEntryContent->setStyleSheet("QLineEdit {border: 1px solid red;}");
        valid = false;
    }
    else
    {
        ui->txtEntryContent->setStyleSheet("");
    }

    return valid;

}

void MainDialog::updateBlogList(QString selectedBlog)
{
    QList<Blog>* blogList = BlogManager::getBlogList();
    //QListWidgetItem selected;

    ui->lstBlogList->clear();
    ui->cmbBlogList->clear();
    ui->lstAllBlogs->clear();

    for(int i=blogList->size()-1; i>=0; i--)
    {
        if(blogList->at(i).m_ownerId == m_currentUser->getId())
        {
            //QListWidgetItem item(blogList->at(i).m_title);
            ui->lstBlogList->addItem(blogList->at(i).m_title);
            ui->cmbBlogList->addItem(blogList->at(i).m_title);
            //if(item.text() == selectedBlog)
            //    selected = item;
        }
        ui->lstAllBlogs->addItem(blogList->at(i).m_title);
    }
    if(ui->lstBlogList->count() == 0)
    {
        ui->lstBlogList->addItem("<you have no blogs>");
        ui->cmbBlogList->addItem("<you have no blogs>");
        ui->cmbBlogList->setEnabled(false);
        ui->lstBlogList->setEnabled(false);
        ui->btnCreateEntry->setEnabled(false);
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
        ui->btnCreateEntry->setEnabled(true);
    }
}

void MainDialog::on_btnCreateBlog_clicked()
{
    if(!validateBlogData())
    {
        QMessageBox::critical(this, "Error", "Provided data is not correct!");
        return;
    }

    QString blogId = ui->txtId->text();
    QString ownerId = this->m_currentUser->getId();
    QString title = ui->txtBlogTitle->text();

    Blog newBlog(blogId, ownerId, title, new QList<BlogEntry>);

    BlogManager::getBlogList()->append(newBlog);
    BlogManager::saveBlogs();
    updateBlogList();
    ui->tabWidget->setCurrentIndex(1);

    clearInputs();

    for(QListWidgetItem* item : ui->lstBlogList->findItems(title, Qt::MatchExactly))
    {
        ui->lstBlogList->setCurrentItem(item);
    }
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

    newEntry->setContentsMargins(10,10,10,10);

    wrapper->layout()->setAlignment(Qt::AlignTop);
    wrapper->layout()->addWidget(newEntry);
}

void MainDialog::displayBlog(const Blog *blog, QWidget *wrapper)
{
    clearBlogs(wrapper);

    for(int i=blog->m_entryList->size()-1; i>=0; i--)
    {
        displayEntry(&blog->m_entryList->at(i), UserManager::getUserById(blog->m_ownerId), wrapper);
    }
}

void MainDialog::clearBlogs(QWidget *wrapper)
{
    for(QFrame* entry : wrapper->findChildren<QFrame*>(QString(), Qt::FindDirectChildrenOnly))
    {
        delete(entry);
    }
}

void MainDialog::clearInputs()
{
    ui->txtBlogTitle->clear();
    ui->txtEntryTitle->clear();
    ui->txtEntryContent->clear();
    ui->txtId->clear();
    ui->chkAutoId->setCheckState(Qt::Unchecked);
}

void MainDialog::on_lstBlogList_itemSelectionChanged()
{
    if(ui->lstBlogList->selectedItems().count() == 0)
    {
        clearBlogs(ui->wgtBlogEntries);
        return;
    }
    QString selected = ui->lstBlogList->selectedItems().at(0)->text();

    const Blog* blog = BlogManager::getBlogByTitle(selected);
    displayBlog(blog, ui->wgtBlogEntries);

}


void MainDialog::on_lstAllBlogs_itemSelectionChanged()
{
    if(ui->lstAllBlogs->selectedItems().count() == 0)
    {
        clearBlogs(ui->wgtAllBlogEntries);
        return;
    }
    QString selected = ui->lstAllBlogs->selectedItems().at(0)->text();

    const Blog* blog = BlogManager::getBlogByTitle(selected);
    displayBlog(blog, ui->wgtAllBlogEntries);
}


void MainDialog::on_btnCreateEntry_clicked()
{
    if(!validateEntryData())
    {
        QMessageBox::critical(this, "Error", "Provided data is not correct!");
        return;
    }

    QString entryTitle = ui->txtEntryTitle->text();
    QString entryContent = ui->txtEntryContent->toPlainText();


    BlogEntry newEntry(entryTitle, QDateTime::currentDateTime(), entryContent);

    const Blog* parentBlog = BlogManager::getBlogByTitle(ui->cmbBlogList->currentText());

    parentBlog->m_entryList->append(newEntry);

    BlogManager::saveBlogs();
    updateBlogList();
    ui->tabWidget->setCurrentIndex(1);

    clearInputs();

    for(QListWidgetItem* item : ui->lstBlogList->findItems(parentBlog->m_title, Qt::MatchExactly))
    {
        ui->lstBlogList->setCurrentItem(item);
    }
}

