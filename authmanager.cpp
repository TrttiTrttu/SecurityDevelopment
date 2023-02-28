#include "authmanager.h"


authManager::authManager(QObject *parent)
    : QObject{parent}
{

}

bool authManager::checkPin(QString pin)
{
    return pin == this->mPin;
}

void authManager::onCheckPin(QString pin)
{
    qDebug() << pin;
    bool res = checkPin(pin);
    emit pinChecked(res);
}
