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

    if (row < 0 || row >= m_rects.size() || column != 0 || role != Qt::DisplayRole)
        return QVariant();

    return m_rects[column]->name().c_str();
}
