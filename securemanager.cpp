#include "securemanager.h"

SecureManager::SecureManager(const QString path)
{
    jFile.setFileName(path);
}

QVector<ListItem> SecureManager::ParseJson()
{
    jFile.open(QIODevice::ReadOnly | QIODevice::Text);
    QString valStr = jFile.readAll();
    jFile.close();

    QJsonDocument jsonDoc = QJsonDocument::fromJson(valStr.toUtf8());
    QJsonObject jsonObject = jsonDoc.object();
    QJsonArray creds = jsonObject["creds"].toArray();

    QVector<ListItem> res;

    for (int i = 0; i < creds.count(); i++) {
            if (!creds.at(i).isObject()) {
                qDebug() << "Not Json";
                continue;
            }

            QJsonObject object = creds.at(i).toObject();


            if (object.contains("site") && object["site"].isString() \
                && object.contains("login") && object["login"].isString() \
                && object.contains("password") && object["password"].isString())
            {
                res.append({object["site"].toString(), object["login"].toString(), object["password"].toString()});
            }
        }

    return res;
}
