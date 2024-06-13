#include "constants.h"
#include "model.h"

#include <stdexcept>
#include <QColor>

//Model* Model::m_instanse = nullptr;
//
//Model& Model::instanse()
//{
//    if (!m_instanse)
//        m_instanse = new Model();
//
//    return *m_instanse;
//}
//
//Picture& Model::picture()
//{
//    return m_picture;
//}
//
//void Model::setPicture(Picture&& picture)
//{
//    m_picture = std::move(picture);
//}
//
//Dataset& Model::dataset()
//{
//    return m_dataset;
//}
//
//void Model::addRect(std::shared_ptr<Rectangle> rect)
//{
//    m_picture.addRect(rect);
//}
//
//void Model::removeRect(std::shared_ptr<Rectangle> rect)
//{
//    // TODO: add remove objects
//}
//
//void Model::loadDataset(const std::string& path)
//{
//    m_dataset = Dataset(path);
//}

//RectModel::RectModel(QObject* parent)
//    : QObject(parent)
//    , m_picture{}
//{
//    m_instanse = this;
//}

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

    if (row < 0 || row >= m_rects.size() || column != 0 || (role != Qt::DisplayRole && role != Qt::UserRole))
        return QVariant();

    auto it = m_rects.begin();
    std::advance(it, row);

    if (role == Qt::UserRole)
        return (*it).second->number();

    return (*it).second->name().c_str();
}

void RectModel::addRect(Rectangle* rect)
{
    beginInsertRows(QModelIndex(), m_rects.size(), m_rects.size() + 1);
    m_rects[rect->number()] = rect;
    endInsertRows();
}

void RectModel::selectRect(Rectangle* rect)
{
    auto it = std::find_if(m_rects.begin(), m_rects.end(),
        [rect](auto& existing) { return existing.second->number() == rect->number(); });

    //it->second->select();
}

void RectModel::deselectRect(Rectangle* rect)
{
    auto it = std::find_if(m_rects.begin(), m_rects.end(),
        [rect](auto& existing) { return existing.second->number() == rect->number(); });

    //it->second->deselect();
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
