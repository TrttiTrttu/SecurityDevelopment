#ifndef SECUREMANAGER_H
#define SECUREMANAGER_H

#include <QObject>
#include <QFile>
#include <listcontroller.h>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <qDebug>

class SecureManager: public QObject
{
    Q_OBJECT

public:
    SecureManager(const QString path);
    QVector<ListItem> ParseJson();
    bool SaveJson();
    QVector<ListItem> *mItems;

private:
    QFile jFile;

public slots:
    void onEntryCreated();
};

#endif // SECUREMANAGER_H
