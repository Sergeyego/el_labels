#include "lblengine.h"

LblEngine::LblEngine(QObject *parent) : QObject(parent)
{
    dir=QDir::currentPath();
}

void LblEngine::createLblSmall(QString text, QString barcode)
{
    QString templ=QString::fromLocal8Bit("SZSM-01");
    GlabelsLbl lbl;
    if (lbl.createLbl(templ,true)){
        lbl.newText(4,19,38,40,text,7,true);
        lbl.newImage(6.5,4,33,13,dir+"/images/simb.png");
        lbl.newImage(32,55,2.5,4.5,dir+"/images/rum.png");
        lbl.newImage(36,55,5.5,4.5,dir+"/images/zont.png");
        lbl.newImage(5.5,60,35,2.5,dir+"/images/site.png");
        if (!barcode.isEmpty()) {
            lbl.newBarcode(4,20.5,38,17,barcode);
        }
        lbl.saveFile("lbl.glabels");
    }
}

void LblEngine::createlblBig(int id_part, QString tuList, QString infoList)
{
    QSqlQuery query;
    query.prepare("select COALESCE(e.marka_sert, e.marka), p.diam, g.nam, pu.nam, COALESCE(p.ibco,d.nam), e.vl, e.pr, e.id_pic "
                  "from parti as p "
                  "inner join elrtr as e on p.id_el=e.id "
                  "inner join gost_types as g on e.id_gost_type=g.id "
                  "inner join purpose as pu on e.id_purpose=pu.id "
                  "inner join denominator as d on e.id_denominator=d.id "
                  "where p.id = :id");
    query.bindValue(":id",id_part);
    if (!query.exec()){
        QMessageBox::critical(NULL,tr("Ошибка"),query.lastError().text(),QMessageBox::Ok);
    }

    QSqlQuery queryAmp;
    queryAmp.prepare("(select distinct d.diam, a.bot, a.vert, a.ceil "
                     "from amp as a "
                     "inner join diam as d on a.id_diam = d.id "
                     "where a.id_el = (select id_el from parti where id = :id1 ) and d.diam = (select diam from parti where id = :id2 ) order by d.diam) "
                     "union "
                     "(select distinct d.diam, a.bot, a.vert, a.ceil "
                     "from amp as a "
                     "inner join diam as d on a.id_diam = d.id "
                     "where a.id_el = (select id_el from parti where id = :id3 ) and d.diam < (select diam from parti where id = :id4 ) order by d.diam DESC LIMIT 1) "
                     "union "
                     "(select distinct d.diam, a.bot, a.vert, a.ceil "
                     "from amp as a "
                     "inner join diam as d on a.id_diam = d.id "
                     "where a.id_el = (select id_el from parti where id = :id5 ) and d.diam > (select diam from parti where id = :id6 ) order by d.diam LIMIT 2) "
                     "order by diam LIMIT 4");
    for (int i=0; i<6; i++){
        queryAmp.bindValue(":id"+QString::number(i+1),id_part);
    }
    if (!queryAmp.exec()){
        QMessageBox::critical(NULL,tr("Ошибка"),queryAmp.lastError().text(),QMessageBox::Ok);
    }

    query.next();
    QString templ=QString::fromLocal8Bit("SZSM-03");
    GlabelsLbl lbl;
    if (lbl.createLbl(templ,false)){
        lbl.newRect(3,3,99,42);
        lbl.newImage(5.5,4,22,10,dir+"/images/simb.png");
        lbl.newLine(30,3,0,42);
        lbl.newLine(3,15,99,0);
        lbl.newLine(30,42,72,0);
        lbl.newLine(60,3,0,32);
        lbl.newLine(60,18,42,0);
        lbl.newLine(47,21,55,0);
        lbl.newLine(30,35,72,0);
        lbl.newLine(47,15,0,20);
        lbl.newLine(74,18,0,17);
        lbl.newLine(88,18,0,17);
        QString chis=query.value(2).toString();
        if (!chis.isEmpty() && chis!=QString("-")){
            chis+=QString::fromUtf8("-");
        } else {
            chis.clear();
        }
        chis+=query.value(0).toString()+QString::fromUtf8("-Ø")+QString::number(query.value(1).toDouble());
        if (!query.value(3).toString().isEmpty() && query.value(3).toString()!=QString("-"))
            chis+=QString::fromUtf8("-")+query.value(3).toString();
        lbl.newText(29.5,5,31,3,chis,6,true,(Qt::AlignHCenter | Qt::AlignVCenter));
        lbl.newLine(31,9,28,0);
        lbl.newText(30.5,10.5,29,3,query.value(4).toString(),6,true,(Qt::AlignHCenter | Qt::AlignVCenter));

        lbl.newText(61,4.5,40,9,tuList,6,true,(Qt::AlignLeft | Qt::AlignVCenter));
        lbl.newText(4,16,25,28,infoList,6,true,(Qt::AlignLeft | Qt::AlignVCenter));

        lbl.newText(60,15.4,42,2.2,tr("Рекоменд. значение тока (А) / Положение шва"),5,false,(Qt::AlignHCenter | Qt::AlignVCenter));
        lbl.newText(62,18.4,10,2.2,tr("Нижнее"),6,false,(Qt::AlignHCenter | Qt::AlignVCenter));
        lbl.newText(74.5,18.4,13,2.2,tr("Вертикаль."),6,false,(Qt::AlignHCenter | Qt::AlignVCenter));
        lbl.newText(88.4,18.4,13,2.2,tr("Потолочное"),6,false,(Qt::AlignHCenter | Qt::AlignVCenter));
        lbl.newText(49,17,9,2.2,tr("Ø, мм"),6,false,(Qt::AlignHCenter | Qt::AlignVCenter));
        int n=0;

        while (queryAmp.next()){
            lbl.newText(49,21.6+(3.5*n),9,2.2,queryAmp.value(0).toString(),6,false,(Qt::AlignHCenter | Qt::AlignVCenter));
            lbl.newText(62.5,21.6+(3.5*n),9,2.2,queryAmp.value(1).toString(),6,false,(Qt::AlignHCenter | Qt::AlignVCenter));
            lbl.newText(76.5,21.6+(3.5*n),9,2.2,queryAmp.value(2).toString(),6,false,(Qt::AlignHCenter | Qt::AlignVCenter));
            lbl.newText(90.5,21.6+(3.5*n),9,2.2,queryAmp.value(3).toString(),6,false,(Qt::AlignHCenter | Qt::AlignVCenter));
            if (n<4){
                lbl.newLine(47,24.5+(n*3.5),55,0);
            }
            n++;
        }

        lbl.newImage(33.5,20,10,10,dir+"/images/"+query.value(7).toString()+".png");

        lbl.newText(31,42.4,69,2.2,tr("Изготовлено: ООО \"СЗСМ\", Россия, Судиславль"),6,false,(Qt::AlignLeft | Qt::AlignVCenter));
        QString t;
        if (!query.value(5).isNull()){
            t+=tr("Допустимое содержание влаги перед использованием до ")+query.value(5).toString()+"%\n";
        }
        if (!query.value(6).isNull()){
            QString pr=query.value(6).toString();
            QString temp=pr.left(3);
            QString dop=pr.mid(3,2);
            QString ch=pr.mid(5,1);
            QString ok=pr.mid(6,2);
            t+=tr("Прокалка перед сваркой - ")+temp+/*tr("±")+dop+*/tr("°C ")+ch+tr(" час")+ok;
        }

        lbl.newText(31,36.2,69,5,t,6,false,(Qt::AlignLeft | Qt::AlignVCenter));

        lbl.saveFile("lbl.glabels");
    }
}

