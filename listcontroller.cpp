#include "listcontroller.h"

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
    if (item.site == oldItem.site && item.login == oldItem.login && item.password == oldItem.password)
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

void ListController::removeCompletedItems()
{
//    for (int i = 0; i < mItems.size(); ) {
//        if (mItems.at(i).done) {
//            emit preItemRemoved(i);

//            mItems.removeAt(i);

//            emit postItemRemoved();
//        } else {
//            ++i;
//        }
//    }
}
