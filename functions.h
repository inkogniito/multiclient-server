#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <QByteArray>
#include <QMap>
#include <QString>
#include <QStringList>
#include <QSqlQuery>
#include <QVariant>

QByteArray reg(QString email, QString username, QString password);


QByteArray login(QString username, QString password);

QByteArray logout();

QByteArray invalid();

QByteArray help();

QByteArray parse(QString message);
#endif
