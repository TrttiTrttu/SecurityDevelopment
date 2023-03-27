#include "listcontroller.h"
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
    this->appendItem(site, login, password);
    emit entryCreated();
}

void ListController::onEntryDeleteClicked()
{
    this->removeCompletedItems();
    emit entryDeleted();
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
