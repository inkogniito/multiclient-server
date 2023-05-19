#include "database.h"
#include <QSqlQuery>
#include <QVariant>
#include <QCoreApplication>
QSqlDatabase& Database::getInstance()
{
    static QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
    QString databasePath = QCoreApplication::applicationDirPath() + "/database_file.sqlite";
    database.setDatabaseName(databasePath);

    if (!database.open()) {
        qDebug() << "Ошибка открытия базы данных!";
    } else {
        QSqlQuery query(database);
        query.exec("CREATE TABLE IF NOT EXISTS users "
                   "(email TEXT, username TEXT, password TEXT, "
                   "PRIMARY KEY (email, username))");

    }

    return database;
}
