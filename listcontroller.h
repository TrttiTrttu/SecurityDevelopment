#ifndef LISTCONTROLLER_H
#define LISTCONTROLLER_H

#include <QObject>
#include <QVector>

struct ListItem
{
    QString site;
    QString login;
    QString password;
};

class ListController : public QObject
{
    Q_OBJECT
public:
    explicit ListController(QObject *parent = nullptr);

    QVector<ListItem> items() const;
    bool setItemAt(int index, const ListItem &item);

    QVector<ListItem> mItems;
    void appendItem(QVector<ListItem> items);

signals:
    void preItemAppended();
    void postItemAppended();

    void preItemRemoved(int index);
    void postItemRemoved();

public slots:

    void removeCompletedItems();

private:
//    QVector<ListItem> mItems;

};

#endif // LISTCONTROLLER_H
