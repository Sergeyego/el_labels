#include "modelpart.h"

ModelPart::ModelPart(QObject *parent) :
    QSqlQueryModel(parent)
{
}

QVariant ModelPart::data(const QModelIndex &item, int role) const
{
    if (item.column()==2 && role==Qt::DisplayRole){
        return QSqlQueryModel::data(item,role).toDate().toString("dd.MM.yy");
    }
    return QSqlQueryModel::data(item,role);
}

void ModelPart::refresh()
{
    setQuery("select p.id, p.n_s, p.dat_part, e.marka, p.diam, i.nam, e.tip, a.nam, ep.mass_ed "
             "from parti as p "
             "inner join elrtr as e on p.id_el=e.id "
             "inner join istoch as i on p.id_ist=i.id "
             "inner join aws_types as a on a.id=e.id_aws_type "
             "inner join el_pack as ep on ep.id=p.id_pack "
             "where p.dat_part between '"+dBeg.toString("yyyy-MM-dd")+"' and '"+dEnd.toString("yyyy-MM-dd")+"' "
             "order by p.dat_part, p.n_s");
    if (lastError().isValid()){
        QMessageBox::critical(NULL,"Error",lastError().text(),QMessageBox::Cancel);
    } else {
        setHeaderData(1, Qt::Horizontal,tr("Партия"));
        setHeaderData(2, Qt::Horizontal,tr("Дата"));
        setHeaderData(3, Qt::Horizontal,tr("Марка"));
        setHeaderData(4, Qt::Horizontal,tr("Ф"));
        setHeaderData(5, Qt::Horizontal,tr("Источник"));
    }
}

void ModelPart::setDBeg(QDate val)
{
    dBeg=val;
}

void ModelPart::setDEnd(QDate val)
{
    dEnd=val;
}


ModelTu::ModelTu(QObject *parent) : QSqlQueryModel(parent)
{
}

void ModelTu::refresh(int id_part)
{
    //qDebug()<<id_part;
    QSqlQuery tuQuery;
        tuQuery.prepare("select nam "
                        "from zvd_get_tu_var((select dat_part from parti where id = :id1 ), "
                        "(select id_el from parti where id = :id2 ), "
                        "(select d.id from diam as d where d.diam = (select diam from parti where id = :id3 )), "
                        "(select id_var from parti where id = :id4 ) ) ");
        tuQuery.bindValue(":id1",id_part);
        tuQuery.bindValue(":id2",id_part);
        tuQuery.bindValue(":id3",id_part);
        tuQuery.bindValue(":id4",id_part);
    if (tuQuery.exec()){
        setQuery(tuQuery);
    } else {
        QMessageBox::critical(NULL,tr("Error"),tuQuery.lastError().text(),QMessageBox::Ok);
    }
}


ModelSert::ModelSert(QObject *parent) : QSqlQueryModel(parent)
{

}

void ModelSert::refresh(int id_parti)
{
    QString id_part=QString::number(id_parti);
    setQuery("select distinct z.ved_short "
             "from zvd_get_sert_var((select dat_part from parti where id = "+id_part+" ), "
             "(select id_el from parti where id = "+id_part+" ), "
             "(select d.id from diam as d where d.diam = (select diam from parti where id = "+id_part+" )), "
             "(select id_var from parti where id = "+id_part+" ) ) as z where z.en = true order by z.ved_short");
    if (lastError().isValid())
        QMessageBox::critical(NULL,"Error",lastError().text(),QMessageBox::Ok);
}
