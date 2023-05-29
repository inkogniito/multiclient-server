#include "functions.h"
#include "database.h" // Добавьте эту строку

QByteArray reg(QString email, QString username, QString password)
{
    QSqlDatabase& db = Database::getInstance();
    if (!db.isOpen()) {
        return QByteArray("Ошибка подключения к базе данных!\n");
    }

    QSqlQuery query(db);
    query.prepare("INSERT INTO users (email, username, password) VALUES (?, ?, ?)");
    query.addBindValue(email);
    query.addBindValue(username);
    query.addBindValue(password);
    if (query.exec()) {
        return QByteArray("Регистрация успешна!\n");
    } else {
        return QByteArray("Ошибка при регистрации!\n");
    }
}

QByteArray login(QString username, QString password)
{
    QSqlDatabase& db = Database::getInstance();
    if (!db.isOpen()) {
        return QByteArray("Ошибка подключения к базе данных!\n");
    }

    QSqlQuery query(db);
    query.prepare("SELECT * FROM users WHERE username = ? AND password = ?");
    query.addBindValue(username);
    query.addBindValue(password);
    if (query.exec() && query.next()) {
        return QByteArray("Авторизация успешна!\n");
    } else {
        return QByteArray("Неправильный логин или пароль!\n");
    }
}


QByteArray logout()
{
    return QByteArray("Выход\n");
}

QByteArray invalid()
{
    return QByteArray("Ошибка!\n");
}

QByteArray help()
{
    return QByteArray("Cписок команд:\n1.reg - Регистрация(введите почту, имя пользователя и пароль через пробел.\n");
}

QByteArray parse(QString request, int authval)
{
   QStringList req = request.left(request.length() - 2).split(" ");
   if (authval==1)
   {
        if (req[0]=="help")
            return help();
        if (req[0]=="logout")
            return logout();
   }

    switch(req.size())
    {
        case 3:
            if (req[0] == "login" and authval == 0)
                return login(req[1],req[2]);
            else
                if (req[0] == "login" and authval == 1)
                    return "Вы уже авторизованы, воспользуйтесь командой logout.\n";
        case 4:
            if (req[0] == "reg" and authval == 0)
                return reg(req[1],req[2],req[3]);
            else
                if(req[0]=="reg" and authval == 1)
                    return "Вы уже авторизованы, воспользуйтесь командой logout.\n";
    }
    return invalid();
}
