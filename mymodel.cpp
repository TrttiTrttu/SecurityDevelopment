#include "mymodel.h"

MyModel::MyModel(QObject *parent)
    : QAbstractListModel(parent)
    , mList(nullptr)
{
}

int MyModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid())
        return 0;

    return mList->items().size();
}

QVariant MyModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || !mList)
        return QVariant();

    const ListItem item = mList->items().at(index.row());
    switch (role) {
    case siteRole:
        return QVariant(item.site);
    case loginRole:
        return QVariant(item.login);
    case passwordRole:
        return QVariant(item.password);
    case deletedRole:
        return QVariant(item.deleted);
    }

    return QVariant();
}

bool MyModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!mList)
            return false;

        ListItem item = mList->items().at(index.row());
        switch (role) {
        case siteRole:
            item.site = value.toString();
            break;
        case loginRole:
            item.login = value.toString();
            break;
        case passwordRole:
            item.password = value.toString();
            break;
        case deletedRole:
            item.deleted = value.toBool();
            break;
        }

        if (mList->setItemAt(index.row(), item)) {
            emit dataChanged(index, index, QVector<int>() << role);
            return true;
        }
        return false;
}

Qt::ItemFlags MyModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

QHash<int, QByteArray> MyModel::roleNames() const
{
    QHash<int, QByteArray> names;
    names[siteRole] = "site";
    names[loginRole] = "login";
    names[passwordRole] = "password";
    names[deletedRole] = "deleted";
    return names;

}

ListController *MyModel::list() const
{
    return mList;
}

void MyModel::setList(ListController *list)
{
    beginResetModel();

        if (mList)
            mList->disconnect(this);

        mList = list;

        if (mList) {
            connect(mList, &ListController::preItemAppended, this, [=]() {
                const int index = mList->items().size();
                beginInsertRows(QModelIndex(), index, index);
            });
            connect(mList, &ListController::postItemAppended, this, [=]() {
                endInsertRows();
            });

            connect(mList, &ListController::preItemRemoved, this, [=](int index) {
                beginRemoveRows(QModelIndex(), index, index);
            });
            connect(mList, &ListController::postItemRemoved, this, [=]() {
                endRemoveRows();
            });
        }

    endResetModel();
}
