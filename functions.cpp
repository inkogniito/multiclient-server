#include "functions.h"

QByteArray reg(QString email, QString username, QString password)
{
    return QByteArray("Регистрация\n");
}

QByteArray login(QString username, QString password)
{
    return QByteArray("Авторизация\n");
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
    return QByteArray("Как-нибудь потом помогу..\n");
}

QByteArray parse(QString request)
{
   QStringList req = request.left(request.length() - 2).split(" ");
   if (req[0]=="help")
       return help();
   if (req[0]=="logout")
       return logout();

    switch(req.size())
    {
        case 3:
            if (req[0] == "login")
                return login(req[1],req[2]);
        case 4:
            if (req[0] == "reg")
                return reg(req[1],req[2],req[3]);
    }
    return invalid();
}
