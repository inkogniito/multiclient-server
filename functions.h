#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <QByteArray>
#include <QMap>
#include <QString>
#include <QStringList>

QByteArray reg(QString username, QString password);

QByteArray login(QString username, QString password);

QByteArray logout();

QByteArray invalid();

QByteArray help();

QByteArray parse(QString message);
#endif
