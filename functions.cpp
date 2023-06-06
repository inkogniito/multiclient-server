#include "functions.h"
#include "database.h"
#include "mst.cpp"
#include <QByteArray>
#include <QDebug>
#include <QBuffer>
#include <QDataStream>
#include <vector>

QByteArray convert(const std::vector<int>& data) {
    QByteArray byteArray;
    QDataStream stream(&byteArray, QIODevice::WriteOnly);

    for (const auto& value : data) {
        stream << value;
    }

    return byteArray;
}


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

std::vector<int> mst(std::vector<std::vector<int>> graph)
{
    MinimumSpanningTreeFinder mstFinder;
    std::vector<MinimumSpanningTreeFinder::Edge> minimumSpanningTree = mstFinder.findMinimumSpanningTree(graph);
    std::vector<int> res;

    for (const auto& edge : minimumSpanningTree)
    {
        res.push_back(edge.from);
        res.push_back(edge.to);
        res.push_back(edge.weight);
    }

    return res;
}

QByteArray logout()
{
    return QByteArray("Выход произведен.\n");
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
        default:
            if (req[0]=="mst")
            {
                    if (req.size()%3==1)
                    {
                        std::vector<std::vector<int>> res;
                        std::vector<int> temp;

                        int i = 1;
                        while (i<=req.size()-3)
                        {
                            temp.push_back(req[i].toInt());
                            temp.push_back(req[i+1].toInt());
                            temp.push_back(req[i+2].toInt());
                            res.push_back(temp);
                            temp.clear();
                            i+=3;
                        }

                        QByteArray byteArray;
                        QDataStream stream(&byteArray, QIODevice::WriteOnly);
                        std::vector<int> data = mst(res);

                        for (int i = 0; i < data.size(); i++) {
                            byteArray.append(QByteArray::number(data[i]));
                            byteArray.append(" ");
                        }

                        QString str = "\n";
                        byteArray.push_back(str.toUtf8());
                        return byteArray;
                    }
            }
    }
    return invalid();
}
