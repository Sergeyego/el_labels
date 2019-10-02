#ifndef LBLENGINE_H
#define LBLENGINE_H

#include <QObject>
#include <QSqlQuery>
#include <QMessageBox>
#include <QDate>
#include <QSqlError>
#include <QDebug>
#include <QDir>
#include "glabelslbl.h"

class LblEngine : public QObject
{
    Q_OBJECT
public:
    explicit LblEngine(QObject *parent = 0);
    void createLblSmall(QString text, QString barcode);
    void createlblBig(int id_part, QString tuList, QString infoList);
private:
    QString dir;
signals:
public slots:
};

#endif // LBLENGINE_H
