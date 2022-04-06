#include "maindialog.h"
#include "ui_maindialog.h"

MainDialog::MainDialog(const User *user, QWidget *loginDialog, QWidget *parent) :QDialog(parent), ui(new Ui::MainDialog), m_currentUser(user)
{
    ui->setupUi(this);
    m_loginDialog = loginDialog;
    ui->lblUsername->setText(m_currentUser->m_username);
    setValidators();
    updateBlogList();
}

MainDialog::~MainDialog()
{
    qInfo() << "Main window deconstructed";
    delete ui;
}

void MainDialog::on_btnLogOut_clicked()
{
    QSettings settings(QSettings::UserScope);
    settings.clear();
    this->close();
    m_loginDialog->show();
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

void MainDialog::updateBlogList()
{
    QList<Blog>* blogList = BlogManager::getBlogList();

    ui->lstBlogList->clear();
    ui->cmbBlogList->clear();
    ui->cmbDeleteBlog->clear();
    ui->lstAllBlogs->clear();

    for(int i=blogList->size()-1; i>=0; i--)
    {
        if(blogList->at(i).m_ownerId == m_currentUser->getId())
        {
            ui->lstBlogList->addItem(blogList->at(i).m_title);
            ui->cmbBlogList->addItem(blogList->at(i).m_title);
            ui->cmbDeleteBlog->addItem(blogList->at(i).m_title);
        }
        ui->lstAllBlogs->addItem(blogList->at(i).m_title);
    }
    if(ui->lstBlogList->count() == 0)
    {
        ui->lstBlogList->addItem("<you have no blogs>");
        ui->cmbBlogList->addItem("<you have no blogs>");
        ui->cmbDeleteBlog->addItem("<you have no blogs>");
        ui->cmbBlogList->setEnabled(false);
        ui->cmbDeleteBlog->setEnabled(false);
        ui->lstBlogList->setEnabled(false);
        ui->btnCreateEntry->setEnabled(false);
        ui->btnDeleteBlog->setEnabled(false);
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
        ui->cmbDeleteBlog->setEnabled(true);
        ui->lstBlogList->setEnabled(true);
        ui->btnCreateEntry->setEnabled(true);
        ui->btnDeleteBlog->setEnabled(true);
    }
}

void MainDialog::on_btnCreateBlog_clicked()
{
    trimInputs();

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

    QLabel* entryUser = new QLabel(newEntry);
    entryUser->setText("Added by: " + (user ? user->m_username : "<invalid user id>"));
    entryUser->setFont(QFont("Segoe", 7));
    entryUser->setAlignment(Qt::AlignRight);
    layout->addWidget(entryUser);

    QGroupBox* entryTitle = new QGroupBox(newEntry);
    entryTitle->setLayout(new QVBoxLayout(entryTitle));
    entryTitle->setTitle(entry->m_title);
    entryTitle->setFont(QFont("Segoe", 12));
    entryTitle->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::MinimumExpanding);
    layout->addWidget(entryTitle);

    QLabel* entryContents = new QLabel(entryTitle);
    entryContents->setText(entry->m_content);
    entryContents->setWordWrap(true);
    entryContents->setFont(QFont("Segoe", 9));
    entryTitle->layout()->addWidget(entryContents);

    QLabel* entryDate = new QLabel(newEntry);
    entryDate->setText("Added on: " + entry->m_date.toString("ddd, MMMM d yyyy, hh:mm:ss"));
    entryDate->setAlignment(Qt::AlignRight);
    entryDate->setFont(QFont("Segoe", 7));
    layout->addWidget(entryDate);

    wrapper->layout()->setSpacing(30);
    wrapper->layout()->setAlignment(Qt::AlignTop);
    wrapper->layout()->addWidget(newEntry);
}

void MainDialog::displayBlog(const Blog *blog, QWidget *wrapper)
{
    clearBlogs(wrapper);

    const User* user = UserManager::getUserById(blog->m_ownerId);

    if(user == nullptr)
        qCritical() << "There is a blog with an invalid owner id!";

    QFrame* blogHeader = new QFrame(wrapper);
    QVBoxLayout* layout = new QVBoxLayout(blogHeader);
    blogHeader->setLayout(layout);

    QLabel* blogTitle = new QLabel(blogHeader);
    blogTitle->setText(blog->m_title);
    blogTitle->setAlignment(Qt::AlignCenter);
    blogTitle->setFont(QFont("Segoe", 14, QFont::Bold));
    layout->addWidget(blogTitle);

    QLabel* blogOwner = new QLabel(blogHeader);
    blogOwner->setText("A blog by: " + (user ? user->m_username : "<invalid user id>"));
    blogOwner->setAlignment(Qt::AlignLeft);
    blogOwner->setFont(QFont("Segoe", 9));
    layout->addWidget(blogOwner);

    QFrame* hLine = new QFrame(blogHeader);
    //hLine->setFrameStyle(QFrame::StyledPanel);
    hLine->setFrameShape(QFrame::HLine);
    hLine->setFrameShadow(QFrame::Sunken);
    layout->addWidget(hLine);

    blogHeader->setContentsMargins(2,5,2,5);
    wrapper->layout()->setAlignment(Qt::AlignTop);
    wrapper->layout()->addWidget(blogHeader);

    if(blog->m_entryList->empty())
    {
        wrapper->layout()->addWidget(new QLabel("There are no blog entries to display", wrapper));
    }
    else for(int i=blog->m_entryList->size()-1; i>=0; i--)
    {
        displayEntry(&blog->m_entryList->at(i), user, wrapper);
    }

    QFrame* spacer = new QFrame(wrapper);
    QVBoxLayout* spacerLayout = new QVBoxLayout(spacer);
    spacer->setLayout(spacerLayout);
    spacerLayout->addItem(new QSpacerItem(0,0,QSizePolicy::Minimum,QSizePolicy::Expanding));
    wrapper->layout()->addWidget(spacer);
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

void MainDialog::trimInputs()
{
    ui->txtBlogTitle->setText(ui->txtBlogTitle->text().trimmed());
    ui->txtEntryTitle->setText(ui->txtEntryTitle->text().trimmed());
    ui->txtEntryContent->setPlainText(ui->txtEntryContent->toPlainText().trimmed());
    ui->txtId->setText(ui->txtId->text().trimmed());
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
    trimInputs();

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


void MainDialog::on_btnDeleteBlog_clicked()
{
    //const Blog* blog = BlogManager::getBlogByTitle(ui->cmbDeleteBlog->currentText());

    QMessageBox::StandardButton answer = QMessageBox::question(this,"Warning","Are you sure you want to delete blog \""+ui->cmbDeleteBlog->currentText()+"\" and all of its content?");

    if(answer == QMessageBox::No)
        return;

    QList<Blog>* blogList = BlogManager::getBlogList();
    for(int i=0; i<blogList->size(); i++)
    {
        if(blogList->at(i).m_title == ui->cmbDeleteBlog->currentText())
        {
            blogList->removeAt(i);
        }
    }
    BlogManager::saveBlogs();
    updateBlogList();
    ui->tabWidget->setCurrentIndex(1);

    clearInputs();
}

