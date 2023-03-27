#include "securemanager.h"

SecureManager::SecureManager(QObject *parent)
    : QObject{parent}
{

}

SecureManager::SecureManager(const QString path)
{
    jFile.setFileName(path);
}

QVector<ListItem> SecureManager::ParseJson()
{
    QByteArray *enc_text = new QByteArray;
    qDebug() << "start key" << this->key.toBase64();
    SecureManager::DecryptFile(this->key, this->jFile, *enc_text);
    qDebug() << "enc_text: " << *enc_text;
    QJsonParseError err;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(*enc_text, &err);

    if (jsonDoc.isNull()) {
        qDebug() <<"Error when parsing JSON:" <<err.errorString();
//        return false;
    }

    if (!jsonDoc.isObject()) {
        qDebug() << "JSON is not array";
//        return false;
    }

    QJsonObject jsonObject = jsonDoc.object();
    QJsonArray creds = jsonObject["creds"].toArray();

    if (creds.isEmpty()) {
        qDebug() << "JSON is empty";
//        return false;
    }

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
//    qDebug() << this->jFile.fileName();
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
        creds_list.append(itemObject.toVariantMap());
    }

    root_map.insert("creds", creds_list);

    if(!this->jFile.open(QIODevice::WriteOnly)){
        qDebug()<<"Failed to open on write";
        return false;
    }
    qDebug() << "encryption is: " << EncryptFile(this->key, this->jFile, jsonDoc.fromVariant(root_map).toJson());
    return EncryptFile(this->key, this->jFile, jsonDoc.fromVariant(root_map).toJson());
//    this->jFile.write(jsonDoc.fromVariant(root_map).toJson());
//    this->jFile.close();
//    return true;
}



QByteArray SecureManager::GenerateKey(const QByteArray pin)
{
    QByteArray hash = QCryptographicHash::hash(pin, QCryptographicHash::Sha256);
    return hash;
}


bool SecureManager::DecryptFile(const QByteArray &key, QFile &jFile , QByteArray &buffer)
{
    if (jFile.isOpen()) jFile.close();
    if (! jFile.open(QFile::ReadOnly)) {
        qDebug()<<"Failed to open file:" << jFile.fileName();
        jFile.close();
        return false;
    }

    EVP_CIPHER_CTX *ctx;
    if (!(ctx = EVP_CIPHER_CTX_new())) {
        jFile.close();
        return false;
    }

    if (!EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, (unsigned char *)key.data(), m_iv)) {
        jFile.close();
        return false;
    }

    int decrypted_len = 0;

    while(! jFile.atEnd()) {
        char encoded_buf[256] = {0};
        unsigned char decrypted_buf[512] = {0};

        jFile.read(encoded_buf, 512);

        QByteArray decoded_buf = QByteArray::fromHex(QByteArray(encoded_buf));
        int decoded_len = decoded_buf.size();


        if (!EVP_DecryptUpdate(ctx, decrypted_buf, &decrypted_len, (unsigned char*)decoded_buf.data(), decoded_len)) {
            jFile.close();
            return false;
        }

        qDebug() << "decrypted_buf + decrypted_len" << decrypted_buf + decrypted_len;

        if (decoded_len < 256) {
            int tmplen;
            if (!EVP_DecryptFinal_ex(ctx, decrypted_buf + decrypted_len, &tmplen)) {
                EVP_CIPHER_CTX_free(ctx);
                jFile.close();
                return false;
            }
            decrypted_len += tmplen;
        }
        buffer += QByteArray((char*)decrypted_buf, decrypted_len);
        qDebug() << "end buffer: " << buffer;

    }
    EVP_CIPHER_CTX_free(ctx);
    jFile.close();
    return true;
}

bool SecureManager::EncryptFile(const QByteArray &key, QFile &jFile , const QByteArray &json)
{
    if (! jFile.open(QIODeviceBase::ReadWrite)) {
        qDebug()<<"Failed to open file:" << jFile.fileName();
        jFile.close();
        return false;
    }

    EVP_CIPHER_CTX *ctx;
    if (!(ctx = EVP_CIPHER_CTX_new())) {
        jFile.close();
        return false;
    }

    if (!EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, (unsigned char *)key.data(), m_iv)) {
        jFile.close();
        return false;
    }

    int encrypted_len = 0;
    for (qsizetype i = 0; i < json.size(); i += 128) {
        unsigned char encrypted_buf[128] = {0};

        QByteArray decoded_buf = json.mid(i, 128);
        int decoded_len =  decoded_buf.size();


        if (!EVP_EncryptUpdate(ctx, encrypted_buf, &encrypted_len, (unsigned char*)decoded_buf.data(), decoded_len)) {
            jFile.close();
            return false;
        }

        if (decoded_len < 128) {

            int tmplen;
            if (!EVP_EncryptFinal_ex(ctx, encrypted_buf + encrypted_len, &tmplen)) {
                EVP_CIPHER_CTX_free(ctx);
                jFile.close();
                return false;
            }
            encrypted_len += tmplen;
        }
        qDebug() << QByteArray((char*)encrypted_buf, encrypted_len).toHex();
        jFile.write(QByteArray((char*)encrypted_buf, encrypted_len).toHex());
    }

    EVP_CIPHER_CTX_free(ctx);
    jFile.close();
    return true;
}

void SecureManager::onEntryCreated()
{
    this->SaveJson();
}

void SecureManager::onEntryDeleted()
{
    this->SaveJson();
}

void SecureManager::onKeyCreated(QByteArray key)
{
    this->key = key;
    QVector<ListItem>  parsed = this->ParseJson();
    emit JSONparsed(&parsed);
}
