#include <QCoreApplication>
#include <QDebug>
#include <dbmanager.h>

static const QString path = "example.db";

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    DbManager db(path);

    if (db.isOpen())
    {

        qDebug() << "End";
    }
    else
    {
        qDebug() << "No se ha podido resolver la conexion con la DB!";
    }

    return a.exec();
}
