#ifndef SECUREMANAGER_H
#define SECUREMANAGER_H

#include <listcontroller.h>

#include <QObject>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <qDebug>
#include <QCryptographicHash>
#include <QBuffer>
#include <openssl/evp.h>


class SecureManager: public QObject
{
    Q_OBJECT

public:
    explicit SecureManager(QObject *parent = nullptr);
    SecureManager(const QString path);

    static QByteArray GenerateKey(const QByteArray pin);

    static bool DecryptFile(const QByteArray &key, QFile &jFile, QByteArray &buffer);
    static bool EncryptFile(const QByteArray &key, QFile &jFile , const QByteArray &json);

    static bool DecryptCredentials(const QByteArray &key, const QByteArray &in, QByteArray &out);
    static bool EncryptCredentials(const QByteArray &key, const QByteArray &in, QByteArray &out);


    QVector<ListItem> ParseJson();
    bool SaveJson();

    QVector<ListItem> *mItems;
    constexpr static unsigned char m_iv[17] = "32a3194275a9ebf1";
private:
    QFile jFile;
    QByteArray key;
signals:
    void JSONparsed(QVector<ListItem>  *data);

public slots:
    void onEntryCreated();
    void onEntryDeleted();

    void onKeyCreated(QByteArray key);
};

#endif // SECUREMANAGER_H

