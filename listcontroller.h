#ifndef LISTCONTROLLER_H
#define LISTCONTROLLER_H

#include <QObject>
#include <QVector>
#include <QClipboard>
#include "qguiapplication.h"

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

    QByteArray key;
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
    void onKeyCreated(QByteArray key);
    void onCopyToBuffer(int num, QString type);
};

#endif // LISTCONTROLLER_H
