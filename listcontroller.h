#ifndef LISTCONTROLLER_H
#define LISTCONTROLLER_H

#include <QObject>
#include <QVector>


struct ListItem
{
    QString site;
    QString login;
    QString password;
    bool deleted = false;
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
    void appendItem(QString site, QString login, QString password);

signals:
    void preItemAppended();
    void postItemAppended();

    void preItemRemoved(int index);
    void postItemRemoved();

    void entryCreated();
    void entryDeleted();

public slots:
    void onCheckCreateEntry(QString site, QString login, QString password);
    void onEntryDeleteClicked();
    void removeCompletedItems();
    void onJSONparsed(QVector<ListItem>  *data);

};

#endif // LISTCONTROLLER_H
