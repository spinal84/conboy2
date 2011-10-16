#include "notesortfilterproxymodel.h"
#include "notelistmodel.h"

NoteSortFilterProxyModel::NoteSortFilterProxyModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{
    setFilterCaseSensitivity(Qt::CaseInsensitive);
    setFilterRole(NoteListModel::TitleRole);

    setDynamicSortFilter(true);
    setSortCaseSensitivity(Qt::CaseInsensitive);
    setSortRole(NoteListModel::LastChangeDateRole);
    sort(0, Qt::DescendingOrder);
}

bool NoteSortFilterProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    QAbstractItemModel *model = sourceModel();

    bool leftFav = model->data(left, NoteListModel::FavoriteRole).toBool();
    bool rightFav = model->data(right, NoteListModel::FavoriteRole).toBool();

    // Only if both are favs or both are non-favs, we have to check the content of the other fields.
    // If one of the notes is a favorite, it should always be placed on top of the list.
    if (leftFav == rightFav) {
        // Tie. Sort with second criteria (date or string)
        QVariant leftData = model->data(left, sortRole());
        QVariant rightData = model->data(right, sortRole());

        if (leftData.type() == QVariant::DateTime) {
            return leftData.toDateTime() < rightData.toDateTime();
        } else {
            return QString::localeAwareCompare(leftData.toString(), rightData.toString()) < 0;
        }

    } else {
        // A favorite note should always go to the top, regardless of the sort order
        if (sortOrder() == Qt::AscendingOrder) {
            return leftFav;
        } else {
            return rightFav;
        }
    }
}

void NoteSortFilterProxyModel::sortByDate()
{
    setSortRole(NoteListModel::LastChangeDateRole);
    sort(0, Qt::DescendingOrder);
}

void NoteSortFilterProxyModel::sortByTitle()
{
    setSortRole(NoteListModel::TitleRole);
    sort(0, Qt::AscendingOrder);
}
