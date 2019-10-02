#include "mainwidget.h"
#include <QApplication>
#include "pgdialog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/images/logotip.png"));
    PgDialog d(QObject::tr("Печать этикеток"));
    if (d.exec()!=QDialog::Accepted) exit(1);
    MainWidget w;
    w.show();

    return a.exec();
}
