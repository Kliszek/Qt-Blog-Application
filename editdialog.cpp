#include "editdialog.h"
#include "ui_editdialog.h"

EditDialog::EditDialog(const Blog* parentBlog, int entryIndex, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditDialog),
    m_parentBlog(parentBlog),
    m_entryIndex(entryIndex)
{
    ui->setupUi(this);
    setValidators();
    ui->txtEntryTitle->setText(m_parentBlog->m_entryList->at(m_entryIndex).m_title);
    ui->txtEntryContent->setPlainText(m_parentBlog->m_entryList->at(m_entryIndex).m_content);
}

EditDialog::~EditDialog()
{
    delete ui;
}

void EditDialog::on_buttonBox_accepted()
{
    trimInputs();

    if(!validateEntryData())
    {
        //QMessageBox::critical(this, "Error", "Provided data is not correct!");
        return;
    }

    BlogEntry updatedEntry;
    updatedEntry.m_title = ui->txtEntryTitle->text();
    updatedEntry.m_date = m_parentBlog->m_entryList->at(m_entryIndex).m_date;
    updatedEntry.m_content = ui->txtEntryContent->toPlainText();

    m_parentBlog->m_entryList->replace(m_entryIndex, updatedEntry);

    accept();
}


void EditDialog::on_buttonBox_rejected()
{
    reject();
}

void EditDialog::setValidators()
{
    QRegularExpression rxNormal("^[a-zA-Z0-9 \\?\\!\\'\\\"_\\-]{3,40}$");
    ui->txtEntryTitle->setValidator(new QRegularExpressionValidator(rxNormal, this));
}

bool EditDialog::validateEntryData()
{
    ui->txtEntryTitleErr->setText("Entry title has to be 3-40 characters long!");
    ui->txtEntryTitleErr->setVisible(false);
    ui->txtEntryContentErr->setText("Entry contents have to be at least 3 characters long!");
    ui->txtEntryContentErr->setVisible(false);

    bool valid = true;

    if(!ui->txtEntryTitle->hasAcceptableInput())
    {
        ui->txtEntryTitleErr->setVisible(true);
        valid = false;
    }

    if(ui->txtEntryContent->toPlainText().length() < 3)
    {
        ui->txtEntryContentErr->setVisible(true);
        valid = false;
    }

    return valid;
}

void EditDialog::trimInputs()
{
    ui->txtEntryTitle->setText(ui->txtEntryTitle->text().trimmed());
    ui->txtEntryContent->setPlainText(ui->txtEntryContent->toPlainText().trimmed());
}

