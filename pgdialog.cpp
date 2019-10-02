#include "pgdialog.h"
#include "ui_pgdialog.h"

PgDialog::PgDialog(const QString title, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PgDialog)
{
    ui->setupUi(this);
    this->setWindowTitle(title);
    ui->edtHost->setText("192.168.1.10");
    ui->groupBox->setVisible(false);
    db = QSqlDatabase::addDatabase("QPSQL");
    connect(ui->cmdShowOpt,SIGNAL(clicked(bool)),ui->groupBox,SLOT(setVisible(bool)));
    connect(ui->cmdConnect,SIGNAL(clicked()),this,SLOT(dBconnect()));
}

PgDialog::~PgDialog()
{
    delete ui;
    if (db.isOpen()) db.close();
}

void PgDialog::dBconnect()
{
    db.setDatabaseName("neo_rtx");
    db.setHostName(ui->edtHost->text());
    db.setPort(ui->edtPort->text().toInt());
    db.setUserName(ui->edtUser->text());
    db.setPassword(ui->edtPasswd->text());
    if (!db.open()) {
        QMessageBox::critical(NULL,"Error",db.lastError().text(),QMessageBox::Ok);
    } else this->accept();
}
