#include "listcontroller.h"
#include <securemanager.h>
#include <qDebug>

ListController::ListController(QObject *parent)
    : QObject{parent}
{

}

QVector<ListItem> ListController::items() const
{
    return mItems;
}

bool ListController::setItemAt(int index, const ListItem &item)
{
    if (index < 0 || index >= mItems.size())
        return false;

    const ListItem &oldItem = mItems.at(index);
    if (item.site == oldItem.site && item.login == oldItem.login && item.password == oldItem.password && item.deleted == oldItem.deleted)
        return false;

    mItems[index] = item;
    return true;
}

void ListController::appendItem(QVector<ListItem> items)
{
    for (int i = 0; i < items.count(); i++) {
        emit preItemAppended();
        mItems.append(items.at(i));
        emit postItemAppended();
    }
}

void ListController::appendItem(QString site, QString login, QString password)
{
    ListItem NewItem;
    NewItem.site = site;
    NewItem.login = login;
    NewItem.password = password;

    emit preItemAppended();
    mItems.append(NewItem);
    emit postItemAppended();
}

void ListController::onCheckCreateEntry(QString site, QString login, QString password)
{
    QByteArray enc_login;
    QByteArray enc_password;
    SecureManager::EncryptCredentials(this->key, login.toUtf8(), enc_login);
    SecureManager::EncryptCredentials(this->key, password.toUtf8(), enc_password);
    this->appendItem(site, enc_login, enc_password);
    emit entryCreated();
}

void ListController::onEntryDeleteClicked()
{
    this->removeCompletedItems();
    emit entryDeleted();
}

void ListController::onCopyToBuffer(int num, QString type)
{
    QByteArray enc_cred;
    if (type == "login") {
        enc_cred = this->mItems.at(num).login.toUtf8();
    }
    else {
        enc_cred = this->mItems.at(num).password.toUtf8();
    }

    QByteArray dec_cred;
    SecureManager::DecryptCredentials(this->key, enc_cred, dec_cred);

    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setText(dec_cred);
}

void ListController::removeCompletedItems()
{
    for (int i = 0; i < mItems.size(); ) {
        if (mItems.at(i).deleted) {
            emit preItemRemoved(i);

            mItems.removeAt(i);

            emit postItemRemoved();
        } else {
            ++i;
        }
    }
}

void ListController::onJSONparsed(QVector<ListItem> *data)
{
    this->appendItem(*data);
}

void ListController::onKeyCreated(QByteArray key)
{
    this->key = key;
}

