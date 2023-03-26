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

bool SecureManager::SaveJson()
{
    qDebug() << this->jFile.fileName();
    QVector <ListItem> tmp = *this->mItems;

    for (int i = 0; i < tmp.count(); i++) {
        qDebug() << tmp.at(i).login;
    }
    QJsonDocument jsonDoc;
    QJsonObject root_obj = jsonDoc.object();
    QVariantMap root_map = root_obj.toVariantMap();
    QVariantList creds_list = root_map["creds"].toList();


    for (int i = 0; i < tmp.count(); i++) {
        QJsonObject itemObject;
        itemObject["site"] = tmp.at(i).site;
        itemObject["login"] = tmp.at(i).login;
        itemObject["password"] = tmp.at(i).password;
//        creds_list.insert(i, itemObject.toVariantMap());
        creds_list.append(itemObject.toVariantMap());
    }

    root_map.insert("creds", creds_list);

    if(!this->jFile.open(QIODevice::WriteOnly)){
        qDebug()<<"Failed to open on write";
        return false;
    }
    this->jFile.write(jsonDoc.fromVariant(root_map).toJson());
    this->jFile.close();
    return true;
}

void SecureManager::onEntryCreated()
{
    this->SaveJson();
}
