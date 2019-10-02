#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include "modelpart.h"
#include <QDataWidgetMapper>
#include "lblengine.h"

namespace Ui {
class MainWidget;
}

class MainWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit MainWidget(QWidget *parent = 0);
    ~MainWidget();
    
private:
    Ui::MainWidget *ui;
    ModelPart *modelPart;
    QDataWidgetMapper *mapper;
    ModelTu *modelTu;
    ModelSert *modelSert;
    QString getTuList();

private slots:
    void updData(QModelIndex index);
    void createLblBig();
    void createLblSmall();
};

#endif // MAINWIDGET_H
