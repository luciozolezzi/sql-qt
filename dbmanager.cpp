#include <dbmanager.h>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QDebug>

DbManager::DbManager(const QString &path)
{
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(path);

    if (!m_db.open())
    {
        qDebug() << "Error: fallo la conexion a la base de datos";
    }
    else
    {
        qDebug() << "Database: conexion ok";
    }
}

DbManager::~DbManager()
{
    if (m_db.isOpen())
    {
        m_db.close();
    }
}

bool DbManager::isOpen() const
{
    return m_db.isOpen();
}

bool DbManager::createTable()
{
    bool success = false;

    QSqlQuery query;
    query.prepare("CREATE TABLE login(id INTEGER PRIMARY KEY, username TEXT,password TEXT);");

    if (!query.exec())
    {
        qDebug() << "No se puedo crear la tabla 'login'";
        success = false;
    }

    return success;
}

bool DbManager::addUser(const QString& username)
{
    bool success = false;

    if (!username.isEmpty())
    {
        QSqlQuery queryAdd;
        queryAdd.prepare("INSERT INTO login (username) VALUES (:username)");
        queryAdd.bindValue(":username", username);

        if(queryAdd.exec())
        {
            success = true;
        }
        else
        {
            qDebug() << "Error al añadir usuario: " << queryAdd.lastError();
        }
    }
    else
    {
        qDebug() << "Error al añadir usuario: no puede ser vacio el nombre";
    }

    return success;
}

bool DbManager::removeUsername(const QString& username)
{
    bool success = false;

    if (userExists(username))
    {
        QSqlQuery queryDelete;
        queryDelete.prepare("DELETE FROM login WHERE username = (:username)");
        queryDelete.bindValue(":username", username);
        success = queryDelete.exec();

        if(!success)
            qDebug() << "Eliminar usuario fallo: " << queryDelete.lastError();
        else
            qDebug() << "Se elimino el usuario correctamente " << username;


    }
    else
    {
        qDebug() << "Error al eliminar usuario: no existe el usuario";
    }

    return success;
}

void DbManager::printAllUsernames() const
{
    qDebug() << "Usuarios en la db:";
    QSqlQuery query("SELECT * FROM login");
    int idUsr = query.record().indexOf("username");
    while (query.next())
    {
        QString username = query.value(idUsr).toString();
        qDebug() << "===" << username;
    }
}

bool DbManager::userExists(const QString& name) const
{
    bool exists = false;

    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT username FROM login WHERE username = (:username)");
    checkQuery.bindValue(":username", name);

    if (checkQuery.exec())
    {
        if (checkQuery.next())
        {
            exists = true;
        }
    }
    else
    {
        qDebug() << "No se ha encontrado el usuario: " << checkQuery.lastError();
    }

    return exists;
}

bool DbManager::removeAllUsers()
{
    bool success = false;

    QSqlQuery removeQuery;
    removeQuery.prepare("DELETE FROM login");

    if (removeQuery.exec())
    {
        success = true;
    }
    else
    {
        qDebug() << "Eliminar todos los usuarios fallo: " << removeQuery.lastError();
    }

    return success;
}
