#ifndef MYMODEL_H
#define MYMODEL_H

#include <QAbstractListModel>
#include <listcontroller.h>

class ListController;

class MyModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(ListController *list READ list WRITE setList)

public:
    explicit MyModel(QObject *parent = nullptr);

    enum {
        siteRole,
        loginRole,
        passwordRole,
        deletedRole
    };

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;

    virtual QHash<int, QByteArray> roleNames() const override;

    ListController *list() const;
    void setList(ListController *list);

private:
    ListController *mList;
};

#endif // MYMODEL_H
