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
        QMessageBox::critical(this, "Error", "Provided data is not correct!");
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
    QRegularExpression rxNormal("^[a-zA-Z0-9 _\\-]{3,30}$");
    ui->txtEntryTitle->setValidator(new QRegularExpressionValidator(rxNormal, this));
}

bool EditDialog::validateEntryData()
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

void EditDialog::trimInputs()
{
    ui->txtEntryTitle->setText(ui->txtEntryTitle->text().trimmed());
    ui->txtEntryContent->setPlainText(ui->txtEntryContent->toPlainText().trimmed());
}

