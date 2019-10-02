#ifndef MODELPART_H
#define MODELPART_H

#include <QSqlQueryModel>
#include <QMessageBox>
#include <QDate>
#include <QSqlError>
#include <QSqlQuery>

class ModelPart : public QSqlQueryModel
{
    Q_OBJECT
public:
    explicit ModelPart(QObject *parent = 0);
    QVariant data(const QModelIndex &item, int role) const;

private:
    QDate dBeg;
    QDate dEnd;
    
signals:
    
public slots:
    void refresh();
    void setDBeg(QDate val);
    void setDEnd(QDate val);
    
};

class ModelTu : public QSqlQueryModel
{
    Q_OBJECT
public:
    explicit ModelTu(QObject *parent = 0);
    void refresh(int id_part);
};

class ModelSert : public QSqlQueryModel
{
    Q_OBJECT
public:
    explicit ModelSert(QObject *parent = 0);
    void refresh(int id_parti);
};

#endif // MODELPART_H
