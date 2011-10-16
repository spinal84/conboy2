#ifndef NOTESORTFILTERPROXYMODEL_H
#define NOTESORTFILTERPROXYMODEL_H

#include <QSortFilterProxyModel>

class NoteSortFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit NoteSortFilterProxyModel(QObject *parent = 0);

protected:
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const;

public slots:
    void sortByDate();
    void sortByTitle();

};

#endif // NOTESORTFILTERPROXYMODEL_H
