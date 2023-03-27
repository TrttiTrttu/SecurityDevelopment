#include "authmanager.h"

authManager::authManager(const QString path)
{
    this->jFile.setFileName(path);
}

bool authManager::checkPin(QByteArray key)
{
    qDebug() << "key: " << key.toBase64();
    QByteArray *buff = new QByteArray;
    bool res =  SecureManager::DecryptFile(key, this->jFile, *buff);

    delete buff;
    return res;
}

void authManager::onCheckPin(QString pin)
{
    qDebug() << pin;
    QByteArray key = SecureManager::GenerateKey(pin.toUtf8());
    bool res = checkPin(key);
    emit pinChecked(res);

    if (res) emit keyCreated(key);
}
