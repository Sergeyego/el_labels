#include "mainwidget.h"
#include "ui_mainwidget.h"
#include <stdio.h>

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);
    ui->dateEditBeg->setDate(QDate::currentDate().addDays(-QDate::currentDate().dayOfYear()+1));
    ui->dateEditEnd->setDate(QDate::currentDate());
    modelPart = new ModelPart(this);
    modelPart->setDBeg(ui->dateEditBeg->date());
    modelPart->setDEnd(ui->dateEditEnd->date());
    modelPart->refresh();
    modelTu = new ModelTu(this);
    ui->listViewTu->setModel(modelTu);
    mapper = new QDataWidgetMapper(this);
    modelSert = new ModelSert(this);
    modelSert->refresh(0);
    ui->listViewVed->setModel(modelSert);
    //ui->listViewVed->setModelColumn(2);
    mapper->setModel(modelPart);
    mapper->addMapping(ui->lineEditPart,1);
    mapper->addMapping(ui->lineEditMark,3);
    mapper->addMapping(ui->doubleSpinBoxDiam,4);
    mapper->addMapping(ui->dateEdit,2);
    mapper->addMapping(ui->lineEditType,6);
    mapper->addMapping(ui->lineEditAws,7);
    mapper->addMapping(ui->doubleSpinBoxMas,8);
    ui->tableViewPart->verticalHeader()->setDefaultSectionSize(ui->tableViewPart->verticalHeader()->fontMetrics().height()*1.5);
    ui->tableViewPart->verticalHeader()->hide();
    ui->tableViewPart->setModel(modelPart);
    ui->tableViewPart->setColumnHidden(0,true);
    ui->tableViewPart->setColumnWidth(1,60);
    ui->tableViewPart->setColumnWidth(2,70);
    ui->tableViewPart->setColumnWidth(3,120);
    ui->tableViewPart->setColumnWidth(4,40);
    ui->tableViewPart->setColumnWidth(5,90);
    ui->tableViewPart->setColumnHidden(6,true);
    ui->tableViewPart->setColumnHidden(7,true);
    ui->tableViewPart->setColumnHidden(8,true);
    connect(ui->cmdUpd,SIGNAL(clicked()),modelPart,SLOT(refresh()));
    connect(ui->dateEditBeg,SIGNAL(dateChanged(QDate)),modelPart,SLOT(setDBeg(QDate)));
    connect(ui->dateEditEnd,SIGNAL(dateChanged(QDate)),modelPart,SLOT(setDEnd(QDate)));
    connect(ui->tableViewPart->selectionModel(),SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),mapper,SLOT(setCurrentModelIndex(QModelIndex)));
    connect(ui->tableViewPart->selectionModel(),SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),this,SLOT(updData(QModelIndex)));
    connect(ui->cmdCreateBig,SIGNAL(clicked()),this,SLOT(createLblBig()));
    connect(ui->cmdCreateSmall,SIGNAL(clicked()),this,SLOT(createLblSmall()));
}

MainWidget::~MainWidget()
{
    delete ui;
}

QString MainWidget::getTuList()
{
    QString str;
    for (int i=0; i<modelTu->rowCount(); i++)
        str+=modelTu->data(modelTu->index(i,0)).toString()+"\n";
    return str;
}

void MainWidget::updData(QModelIndex index)
{
    int id_part=ui->tableViewPart->model()->data(ui->tableViewPart->model()->index(index.row(),0),Qt::EditRole).toInt();
    modelTu->refresh(id_part);
    modelSert->refresh(id_part);
}

void MainWidget::createLblSmall()
{
    QString ved;
    for (int i=0; i<modelSert->rowCount(); i++)
        ved+=modelSert->data(modelSert->index(i,0)).toString()+", ";
    if (ved.length()>1) ved.truncate(ved.length()-2);
    QString strDat;
    strDat+=tr("Марка - ");
    strDat+=ui->lineEditMark->text()+"\n";
    strDat+=tr("Диаметр, мм - ");
    strDat+=QLocale().toString(ui->doubleSpinBoxDiam->value(),'f',1)+"\n";
    strDat+=tr("Партия № ");
    strDat+=ui->lineEditPart->text()+"\n";
    strDat+=tr("Дата изг. - ");
    strDat+=ui->dateEdit->date().toString("dd.MM.yyyy")+"\n";
    strDat+=tr("Тип - ");
    strDat+=ui->lineEditType->text()+tr(" ГОСТ 9467-75")+"\n";
    strDat+=ui->lineEditAws->text()+"\n";
    strDat+=tr("Одобрено - ");
    strDat+=ved+"\n";
    strDat+=getTuList();
    strDat+=tr("Масса нетто, кг - ");
    strDat+=QLocale().toString(ui->doubleSpinBoxMas->value(),'f',1);
    LblEngine e;
    e.createLblSmall(strDat,QString());
}

void MainWidget::createLblBig()
{
    int index=ui->tableViewPart->currentIndex().row();
    int id_part=ui->tableViewPart->model()->data(ui->tableViewPart->model()->index(index,0),Qt::EditRole).toInt();
    QString strDat;
    strDat+=tr("Марка - ");
    strDat+=ui->lineEditMark->text()+"\n";
    strDat+=tr("Диаметр, мм - ");
    strDat+=QLocale().toString(ui->doubleSpinBoxDiam->value(),'f',1)+"\n";
    strDat+=tr("Партия № ");
    strDat+=ui->lineEditPart->text()+"\n";
    strDat+=tr("Дата изг. - ");
    strDat+=ui->dateEdit->date().toString("dd.MM.yyyy")+"\n";
    strDat+=tr("Масса нетто, кг - ");
    strDat+=QLocale().toString(ui->doubleSpinBoxMas->value(),'f',1);
    LblEngine e;
    e.createlblBig(id_part,getTuList(),strDat);
}
