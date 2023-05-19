#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>

class Database
{
public:
    static QSqlDatabase& getInstance();
};

#endif // DATABASE_H
