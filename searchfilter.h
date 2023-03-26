#ifndef SEARCHFILTER_H
#define SEARCHFILTER_H

#include <QSortFilterProxyModel>

class SearchFilter : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit SearchFilter(QObject *parent = nullptr);
     SearchFilter(QAbstractItemModel *source, int role);
public slots:
    void onSearchInputChanged(QString input);
};

#endif // SEARCHFILTER_H
