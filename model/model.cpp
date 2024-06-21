#include "constants.h"
#include "model.h"

RectModel::RectModel(QObject* parent)
{}

int RectModel::rowCount(const QModelIndex& parent) const
{
    return m_rects.size();
}

int RectModel::columnCount(const QModelIndex& parent) const
{
    return 1;
}

QVariant RectModel::data(const QModelIndex& index, int role) const
{
    int row = index.row();
    int column = index.column();

    if (row < 0 || row >= m_rects.size() || column != 0 || (role != Qt::DisplayRole && role != Qt::UserRole && role != Qt::CheckStateRole))
        return QVariant();

    auto it = m_rects.begin();
    std::advance(it, row);

    if (role == Qt::UserRole)
        return (*it).second->number();

    if (role == Qt::CheckStateRole)
        return (*it).second->isVisible() ? Qt::Checked : Qt::Unchecked;

    return (*it).second->name().c_str();
}

bool RectModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (role != Qt::CheckStateRole || index.column() != 0)
        return false;

    auto rectId = data(index, Qt::UserRole).toInt();
    if (value == Qt::Checked)
        m_rects[rectId]->show();
    else if (value == Qt::Unchecked)
        m_rects[rectId]->hide();

    emit dataChanged(index, index);

    return true;
}

void RectModel::addRect(Rectangle* rect)
{
    beginInsertRows(QModelIndex(), m_rects.size(), m_rects.size() + 1);
    m_rects[rect->number()] = rect;
    endInsertRows();
}

void RectModel::deleteRect(Rectangle* rect)
{
    auto it = m_rects.find(rect->number());
    if (it == m_rects.end())
        return;

    beginRemoveRows(QModelIndex(), 0, m_rects.size() - 1);
    m_rects.erase(rect->number());
    endRemoveRows();
}

int RectModel::rowByRect(Rectangle* rect) const
{
    auto it = m_rects.find(rect->number());
    if (it == m_rects.end())
        return -1;

    return std::distance(m_rects.begin(), it);
}

Qt::ItemFlags RectModel::flags(const QModelIndex& index) const
{
    if (index.column() == 0)
    {
        Qt::ItemFlags flags;
        flags |= Qt::ItemIsSelectable;
        flags |= Qt::ItemIsEnabled;
        flags |= Qt::ItemIsUserCheckable;
        return flags;
    }

    return QAbstractItemModel::flags(index);
}
