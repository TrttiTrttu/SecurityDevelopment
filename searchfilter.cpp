#include "searchfilter.h"

SearchFilter::SearchFilter(QObject *parent)
    : QSortFilterProxyModel{parent}
{
    this->setDynamicSortFilter(true);
}

SearchFilter::SearchFilter(QAbstractItemModel *source, int role)
{
    this->setSourceModel(source);
    this->setFilterRole(role);
    this->setDynamicSortFilter(true);
}

void SearchFilter::onSearchInputChanged(QString input)
{
    this->setFilterFixedString(input);
}
